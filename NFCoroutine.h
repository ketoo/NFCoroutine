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
class NFCoroutineSchdule;

typedef bool (*Function)(void *arg);

static void ExecuteBody(NFCoroutine *ps);

class NFCoroutine
{
public:
    NFCoroutine(NFCoroutineSchdule* p, int id)
    {
        pSchdule = p;
        state = CoroutineState::FREE;
        nID = id;
        nChildID = -1;
        nParent = -1;
    }

    Function func;
    void *arg;
    enum CoroutineState state;
    int nID;
    int nChildID;
    int nParent;
    NFCoroutineSchdule* pSchdule;

    ucontext_t ctx;
    char stack[MAX_COROUTINE_STACK_SIZE];
};

class NFCoroutineSchdule
{
public:


    NFCoroutineSchdule();
    
    virtual ~NFCoroutineSchdule();

    int  Create(Function func, void *arg);
    void Init(Function func, void *arg);

    void Yield();

    void Resume(int id);

    void ScheduleJob();


    int GetRunningID();
    void SetRunningID(int id);
    void RemoveRuningID(int id);

    NFCoroutine* GetCoroutine(int id);
    NFCoroutine* GetRunningCoroutine();

protected:
    int  CreateChildCo(Function func, void *arg);

    NFCoroutine* AllotCoroutine();
    NFCoroutine* NewMainCoroutine();

protected:
    ucontext_t main;
    int mnRunningCoroutineID;
    std::vector<NFCoroutine*> mxCoroutineList;
    std::list<int> mxRunningList;
    int mnMaxIndex;

    //init
    Function mainFunc;
    void* mainArg;
};


#endif
