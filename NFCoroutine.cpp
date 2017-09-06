#include <iostream>
#include "NFCoroutine.h"


void ExecuteBody(NFCoroutine *co)
{

    std::cout << "ExecuteBody " << co->nID << std::endl;

    co->func(co->arg);


    NFCoroutine* pParentCo = co->pSchdule->GetCoroutine(co->nParent);
    if (pParentCo)
    {
        pParentCo->nChildID = -1;
        //下一次切换后,就可以调度父协程--不用调度，这个函数执行完毕后，父协程自动接着往下走
    }

    co->state = FREE;
    co->pSchdule->RemoveRuningID(co->nID);

    std::cout << "func finished -- swap " << co->nID << " to " << co->nParent << std::endl;

    co->pSchdule->SetRunningID(co->nParent);

    co->nChildID = -1;
    co->nParent = -1;
}

NFCoroutineSchdule::NFCoroutineSchdule()
{
    mnRunningCoroutineID = -1;
    mnMaxIndex = 0;

    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
    {
        mxCoroutineList.push_back(new NFCoroutine(this, i));
    }
}

NFCoroutineSchdule::~NFCoroutineSchdule()
{
    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
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
        std::cout << this->mnRunningCoroutineID << " swap to " << id << std::endl;

        this->mnRunningCoroutineID = id;
        swapcontext(&(this->main),&(t->ctx));
    }
}

void NFCoroutineSchdule::Yield()
{
    if(this->mnRunningCoroutineID != -1 )
    {
        NFCoroutine *t = GetRunningCoroutine();
        t->state = SUSPEND;


        std::cout << "Yield " << this->mnRunningCoroutineID << " to -1" << std::endl;

        RemoveRuningID(this->mnRunningCoroutineID);

        this->mnRunningCoroutineID = -1;


        swapcontext(&(t->ctx), &(main));
    }
}

void NFCoroutineSchdule::Init(Function func, void *arg)
{
    mainFunc = func;
    mainArg = arg;

    NewMainCoroutine();
}

int NFCoroutineSchdule::Create(Function func, void *arg)
{
    //创建的时候，其实应该创建2个协程，一个开启新循环，因为之前0的那个blocking等待子协程返回

    NFCoroutine* pRunningCo = GetRunningCoroutine();
    if (pRunningCo->nParent >= 0
        && pRunningCo->nChildID >= 0)
    {
        return -1;
    }


    int id = CreateChildCo(func, arg);
    NewMainCoroutine();


    return id;
}


void NFCoroutineSchdule::ScheduleJob()
{
    if (mxRunningList.size() > 0)
    {
        int id = mxRunningList.front();
        mxRunningList.pop_front();

        NFCoroutine* pCoroutine = mxCoroutineList[id];

        //必须是子协程才可以调度
        //父协程在子协成结束后，也可以调度
        if(pCoroutine->state == SUSPEND
           && pCoroutine->nChildID < 0)
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

int NFCoroutineSchdule::GetRunningID()
{
    return mnRunningCoroutineID;
}

void NFCoroutineSchdule::SetRunningID(int id)
{
    mnRunningCoroutineID = id;
}

void NFCoroutineSchdule::RemoveRuningID(int id)
{
    mxRunningList.remove(id);
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


NFCoroutine* NFCoroutineSchdule::AllotCoroutine()
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

    return this->mxCoroutineList[id];
}

int NFCoroutineSchdule::CreateChildCo(Function func, void *arg)
{
    NFCoroutine *newCo = AllotCoroutine();

    newCo->state = CoroutineState::SUSPEND;
    newCo->func = func;
    newCo->arg = arg;

    getcontext(&(newCo->ctx));

    newCo->ctx.uc_stack.ss_sp = newCo->stack;
    newCo->ctx.uc_stack.ss_size = MAX_COROUTINE_STACK_SIZE;
    newCo->ctx.uc_stack.ss_flags = 0;

    NFCoroutine * t_running = GetRunningCoroutine();

    t_running->state = CoroutineState::SUSPEND;
    t_running->nChildID = newCo->nID;
    newCo->nParent = t_running->nID;
    newCo->ctx.uc_link = &(t_running->ctx);


    std::cout << "create new co " << newCo->nID << std::endl;

    makecontext(&(newCo->ctx), (void (*)(void)) (ExecuteBody), 1, newCo);



    mxRunningList.push_back(newCo->nID);

    return newCo->nID;
}

NFCoroutine* NFCoroutineSchdule::NewMainCoroutine()
{
    NFCoroutine* newCo = AllotCoroutine();

    newCo->state = CoroutineState::SUSPEND;
    newCo->func = mainFunc;
    newCo->arg = mainArg;

    getcontext(&(newCo->ctx));

    newCo->ctx.uc_stack.ss_sp = newCo->stack;
    newCo->ctx.uc_stack.ss_size = MAX_COROUTINE_STACK_SIZE;
    newCo->ctx.uc_stack.ss_flags = 0;
    newCo->ctx.uc_link = &(this->main);

    makecontext(&(newCo->ctx), (void (*)(void)) (ExecuteBody), 1, newCo);

    mxRunningList.push_back(newCo->nID);

    std::cout << "create NewMainCoroutine " << newCo->nID << std::endl;
}