#ifndef NF_COROUTINE_MDULE
#define NF_COROUTINE_MDULE

#ifdef __APPLE__
#define _XOPEN_SOURCE
#endif

#include <ucontext.h>
#include <vector>
#include <list>

#define MAX_COROUTINE_STACK_SIZE (1024 * 128)
#define MAX_COROUTINE_CAPACITY   (1024 * 128)

enum CoroutineState
{
    FREE,
    SUSPEND
};

class NFCoroutine;

class NFCoroutineSchedule;

typedef bool (* Function)(void* arg);

static void ExecuteBody(NFCoroutine* ps);

class NFCoroutine
{
public:
    NFCoroutine(NFCoroutineSchedule* p, int id)
    {
        pSchdule = p;
        state = CoroutineState::FREE;
        nID = id;
        nChildID = -1;
        nParent = -1;
    }

    Function func;
    void* arg;
    enum CoroutineState state;
    int nID;
    int nChildID;
    int nParent;
    NFCoroutineSchedule* pSchdule;

    ucontext_t ctx;
    char stack[MAX_COROUTINE_STACK_SIZE];
};

class NFCoroutineSchedule
{
public:

    NFCoroutineSchedule();

    virtual ~NFCoroutineSchedule();

    int Create(Function func, void* arg);

    void Init(Function func, void* arg);

    void Yield();
    void Yield(float time);

    void Resume(int id);

    void ScheduleJob();


    int GetRunningID();

    void SetRunningID(int id);

    void RemoveRunningID(int id);

    NFCoroutine* GetCoroutine(int id);

    NFCoroutine* GetRunningCoroutine();

protected:
    int CreateChildCo(Function func, void* arg);

    NFCoroutine* AllotCoroutine();

    NFCoroutine* NewMainCoroutine();

protected:
    Function mxMainFunc;
    void* mpMainArg;

    ucontext_t mxMainCtx;
    int mnRunningCoroutineID;

    std::vector<NFCoroutine*> mxCoroutineList;
    std::list<int> mxRunningList;


    int mnMaxIndex;

};


#endif
