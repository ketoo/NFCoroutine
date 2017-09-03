#ifndef MY_UTHREAD_H
#define MY_UTHREAD_H

#ifdef __APPLE__
#define _XOPEN_SOURCE
#endif 

#include <ucontext.h>
#include <vector>

#define MAX_TASK_STACK_SZIE (1024*128)
#define MAX_COROUTINE_SIZE   1024 * 128

enum CoroutineState
{
    FREE,
    RUNNING,
    SUSPEND
};

class NFCoroutineSchdule;

typedef bool (*Function)(void *arg);

static void ExecuteBody(NFCoroutineSchdule *ps);

class NFCoroutine
{
public:
    NFCoroutine(int id)
    {
        state = CoroutineState::FREE;
        nID = id;
        nChildID = -1;
    }

    ucontext_t ctx;
    Function func;
    void *arg;
    enum CoroutineState state;
    int nID;
    int nChildID;
    char stack[MAX_TASK_STACK_SZIE];
};

class NFCoroutineSchdule
{
public:


    NFCoroutineSchdule();
    
    virtual ~NFCoroutineSchdule();

    int  Create(Function func, void *arg);

    void Yield();

    void Resume(int id);

    void ScheduleJob();


    int GetRunningID();
    void SetRunningID(int id);
    NFCoroutine* GetCoroutine(int id);
    NFCoroutine* GetRunningCoroutine();

    int GetLastRunningID();
    void SetLastRunningID(int id);
    NFCoroutine* GetLastCoroutine();

protected:
    ucontext_t main;
    int mnRunningCoroutineID;
    int mnLastCoroutineID;
    std::vector<NFCoroutine*> mxCoroutineList;
    int mnMaxIndex;//使用了的数量，减少调度时的循环
};


#endif
