#ifndef NF_COROUTINE_MDULE
#define NF_COROUTINE_MDULE

#ifdef __APPLE__
#define _XOPEN_SOURCE
#endif

#include <ucontext.h>
#include <vector>
#include <list>

#define MAX_COROUTINE_STACK_SIZE (1024 * 128)
#define MAX_COROUTINE_CAPACITY   (1024 * 1)

enum CoroutineState
{
    FREE,
    SUSPEND
};


class NFCoroutine;

class NFCoroutineSchedule;

typedef void (* Function)(void* arg);

static void ExecuteBody(NFCoroutine* ps);

class NFCoroutine
{
public:
    NFCoroutine(NFCoroutineSchedule* p, int id)
    {
        pSchdule = p;
        state = CoroutineState::FREE;
        nID = id;
        bYielded = false;
    }

    Function func;
    void* arg;
    enum CoroutineState state;
    int nID;
    bool bYielded;
    NFCoroutineSchedule* pSchdule;

    ucontext_t ctx;
    char stack[MAX_COROUTINE_STACK_SIZE];
};

class NFCoroutineSchedule
{
public:

    NFCoroutineSchedule();

    virtual ~NFCoroutineSchedule();

    void Init(Function func);

    void RemoveRunningID(int id);

    void Yield();

    void ScheduleJob();

protected:

    void NewMainCoroutine();

    void Resume(int id);

    int GetRunningID();
    void SetRunningID(int id);

    NFCoroutine* AllotCoroutine();

    NFCoroutine* GetCoroutine(int id);
    NFCoroutine* GetRunningCoroutine();



protected:
    Function mxMainFunc;
    void* mpMainArg;

    ucontext_t mxMainCtx;
    int mnRunningCoroutineID;

    std::vector<NFCoroutine*> mxCoroutineList;
    std::list<int> mxRunningList;


    int mnMaxIndex;
    int mnMainID;

};


#endif
