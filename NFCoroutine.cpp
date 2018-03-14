#include <iostream>
#include "NFCoroutine.h"

void ExecuteBody(NFCoroutine* co)
{

    std::cout << "ExecuteBody " << co->nID << std::endl;

    co->func(co->arg);

    co->state = FREE;



    co->pSchdule->RemoveRunningID(co->nID);

    std::cout << "Free " << co->nID << std::endl;

    co->pSchdule->Yield();

}

NFCoroutineSchedule::NFCoroutineSchedule()
{
    mnRunningCoroutineID = -1;
    mnMaxIndex = 0;
    mnMainID = -1;

    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
    {
        mxCoroutineList.push_back(new NFCoroutine(this, i));
    }
}

NFCoroutineSchedule::~NFCoroutineSchedule()
{
    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
    {
        delete mxCoroutineList[i];
    }
}

void NFCoroutineSchedule::Resume(int id)
{
    if (id < 0 || id >= this->mnMaxIndex)
    {
        return;
    }

    NFCoroutine* t = GetCoroutine(id);
    if (t->state == SUSPEND)
    {
        std::cout << this->mnRunningCoroutineID << " swap to " << id << std::endl;
        this->mnRunningCoroutineID = id;
        swapcontext(&(this->mxMainCtx), &(t->ctx));
    }
}

void NFCoroutineSchedule::Yield()
{
    if (this->mnRunningCoroutineID != -1)
    {
        NFCoroutine* t = GetRunningCoroutine();

        if (mnMainID == t->nID)
        {
            mnMainID = -1;
            //NewMainCoroutine();
        }
/*
        if (t->state == SUSPEND)
        {
            if (!t->bYielded)
            {
                t->bYielded = true;

                NewMainCoroutine();
            }
        }
        */

        std::cout << "Yield " << this->mnRunningCoroutineID << " to -1" << std::endl;

        this->mnRunningCoroutineID = -1;

        swapcontext(&(t->ctx), &(mxMainCtx));
    }
}

void NFCoroutineSchedule::Init(Function func)
{
    mxMainFunc = func;
    mpMainArg = this;

    NewMainCoroutine();
}

void NFCoroutineSchedule::ScheduleJob()
{
    std::cout << "ScheduleJob, mainID = " << mnMainID << std::endl;

    if (mxRunningList.size() > 0 && mnMainID >= 0)
    {
        int id = mxRunningList.front();
        mxRunningList.pop_front();

        NFCoroutine* pCoroutine = mxCoroutineList[id];

        if (pCoroutine->state == SUSPEND)
        {
            mxRunningList.push_back(id);

            Resume(id);
        }
    }
    else
    {
        NewMainCoroutine();
    }
}

int NFCoroutineSchedule::GetRunningID()
{
    return mnRunningCoroutineID;
}

void NFCoroutineSchedule::SetRunningID(int id)
{
    mnRunningCoroutineID = id;
}

void NFCoroutineSchedule::RemoveRunningID(int id)
{
    const int lastID = mxRunningList.back();
    if (lastID == id)
    {
        mxRunningList.pop_back();
    }
    else
    {
        mxRunningList.remove(id);
    }
}

NFCoroutine* NFCoroutineSchedule::GetCoroutine(int id)
{
    if (id >= 0 && id < mnMaxIndex)
    {
        return mxCoroutineList[id];
    }

    return NULL;
}

NFCoroutine* NFCoroutineSchedule::GetRunningCoroutine()
{
    if (mnRunningCoroutineID < 0)
    {
        return NULL;
    }

    return mxCoroutineList[mnRunningCoroutineID];
}


NFCoroutine* NFCoroutineSchedule::AllotCoroutine()
{
    int id = 0;
    for (; id < this->mnMaxIndex; ++id)
    {
        if (mxCoroutineList[id]->state == FREE)
        {
            break;
        }
    }

    if (id == this->mnMaxIndex)
    {
        this->mnMaxIndex++;
    }

    return this->mxCoroutineList[id];
}

void NFCoroutineSchedule::NewMainCoroutine()
{

    NFCoroutine* newCo = AllotCoroutine();
    if (newCo == NULL)
    {
        return;
    }

    mxRunningList.push_back(newCo->nID);
    mnMainID = newCo->nID;

    std::cout << "create NewMainCoroutine " << newCo->nID << std::endl;

    newCo->state = CoroutineState::SUSPEND;
    newCo->func = mxMainFunc;
    newCo->arg = mpMainArg;
    newCo->bYielded = false;

    getcontext(&(newCo->ctx));

    newCo->ctx.uc_stack.ss_sp = newCo->stack;
    newCo->ctx.uc_stack.ss_size = MAX_COROUTINE_STACK_SIZE;
    newCo->ctx.uc_stack.ss_flags = 0;
    newCo->ctx.uc_link = &(this->mxMainCtx);

    makecontext(&(newCo->ctx), (void (*)(void)) (ExecuteBody), 1, newCo);
}