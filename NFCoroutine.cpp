
#include "NFCoroutine.h"


void ExecuteBody(NFCoroutineSchdule *ps)
{
    int id = ps->GetRunningID();

    if(id != -1)
    {
        NFCoroutine *t = ps->GetCoroutine(id);

        t->func(t->arg);

        t->state = FREE;
    }
}

NFCoroutineSchdule::NFCoroutineSchdule()
{
    mnRunningCoroutineID = -1;
    mnLastCoroutineID = -1;
    mnMaxIndex = 0;

    for (int i = 0; i < MAX_COROUTINE_SIZE; i++)
    {
        mxCoroutineList.push_back(new NFCoroutine(i));
    }
}

NFCoroutineSchdule::~NFCoroutineSchdule()
{
    for (int i = 0; i < MAX_COROUTINE_SIZE; i++)
    {
        delete mxCoroutineList[i];
    }
}

void NFCoroutineSchdule::Resume(int id)
{
    if(id < 0 || id >= this->mnMaxIndex)
    {
        return;
    }

    NFCoroutine *t = GetCoroutine(id);

    if (t->state == SUSPEND)
    {
        this->mnRunningCoroutineID = id;
        if (this->mnLastCoroutineID < 0)
        {
            swapcontext(&(this->main),&(t->ctx));
        }
        else
        {
            NFCoroutine * last_running = GetLastCoroutine();
            swapcontext(&(last_running->ctx),&(t->ctx));
        }
    }
}

void NFCoroutineSchdule::Yield()
{
    if(this->mnRunningCoroutineID != -1 )
    {
        NFCoroutine *t = GetRunningCoroutine();
        t->state = SUSPEND;

        mnRunningCoroutineID = -1;
        mnLastCoroutineID = t->nID;

        ScheduleJob();
    }
}


int NFCoroutineSchdule::Create(Function func, void *arg)
{
    int id = 0;
    for(; id < this->mnMaxIndex; ++id )
    {
        if(mxCoroutineList[id]->state == FREE)
        {
            break;
        }
    }
    
    if (id == this->mnMaxIndex)
    {
        this->mnMaxIndex++;
    }

    NFCoroutine *t = this->mxCoroutineList[id];

    t->state = RUNNING;
    t->func = func;
    t->arg = arg;

    getcontext(&(t->ctx));
    
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = MAX_TASK_STACK_SZIE;
    t->ctx.uc_stack.ss_flags = 0;

    if (this->mnRunningCoroutineID >= 0)
    {
        NFCoroutine * t_running = GetRunningCoroutine();
        t_running->state = CoroutineState::SUSPEND;
        t_running->nChildID = id;

        t->ctx.uc_link = &(t_running->ctx);

        this->mnLastCoroutineID = this->mnRunningCoroutineID;
        this->mnRunningCoroutineID = id;

        makecontext(&(t->ctx), (void (*)(void)) (ExecuteBody), 1, this);

    }
    else
    {
        t->ctx.uc_link = &(this->main);
        this->mnLastCoroutineID = this->mnRunningCoroutineID;

        this->mnRunningCoroutineID = id;
        makecontext(&(t->ctx), (void (*)(void)) (ExecuteBody), 1, this);

    }

    ScheduleJob();

    return id;
}


void NFCoroutineSchdule::ScheduleJob()
{
    int nNextRunningCoroutine = this->mnLastCoroutineID + 1;
    if (this->mnMaxIndex == nNextRunningCoroutine)
    {
        nNextRunningCoroutine = 0;
    }

    for(; nNextRunningCoroutine < this->mnMaxIndex; ++nNextRunningCoroutine)
    {
        NFCoroutine* pCoroutine = mxCoroutineList[nNextRunningCoroutine];
        if(pCoroutine->state == SUSPEND
                && pCoroutine->nChildID < 0)
        {
            Resume(nNextRunningCoroutine);
        }
    }
}

int NFCoroutineSchdule::GetRunningID()
{
    return mnRunningCoroutineID;
}

void NFCoroutineSchdule::SetRunningID(int id)
{
    mnRunningCoroutineID = id;
}


NFCoroutine* NFCoroutineSchdule::GetCoroutine(int id)
{
    if (id >= 0 && id < mnMaxIndex)
    {
        return mxCoroutineList[id];
    }

    return NULL;
}

NFCoroutine* NFCoroutineSchdule::GetRunningCoroutine()
{
    if (mnRunningCoroutineID < 0)
    {
        return NULL;
    }

    return mxCoroutineList[mnRunningCoroutineID];
}

NFCoroutine* NFCoroutineSchdule::GetLastCoroutine()
{
    if (mnLastCoroutineID < 0)
    {
        return NULL;
    }

    return mxCoroutineList[mnLastCoroutineID];
}

int NFCoroutineSchdule::GetLastRunningID()
{
    return mnLastCoroutineID;
}

void NFCoroutineSchdule::SetLastRunningID(int id)
{
    mnLastCoroutineID = id;
}