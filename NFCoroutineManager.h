// -------------------------------------------------------------------------
//    @FileName			:    NFCoroutineManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCoroutineManager
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NF_COROUTINE_MANAGER_H
#define NF_COROUTINE_MANAGER_H

#ifdef __APPLE__
#define _XOPEN_SOURCE
#endif

#include <iostream>
#include <ucontext.h>
#include <vector>
#include <list>


#define NF_TEST

#ifdef NF_TEST
#define NF_PLATFORM_WIN 1
#define NF_PLATFORM_LINUX 2
#define NF_PLATFORM_APPLE 3

#define  NF_PLATFORM NF_PLATFORM_APPLE

typedef void (* CoroutineFunction)(void* arg);
#else
#include "NFComm/NFPluginModule/NFIModule.h"
#endif


#define MAX_COROUTINE_STACK_SIZE (1024 * 128)
#define MAX_COROUTINE_CAPACITY   (1024 * 128)

enum CoroutineState
{
    FREE,
    SUSPEND
};


class NFCoroutine;

class NFCoroutineManager;

class NFCoroutine
{
public:
    NFCoroutine(NFCoroutineManager* p, int id)
    {
        pSchdule = p;
        state = CoroutineState::FREE;
        nID = id;
    }

    CoroutineFunction func;
    void* arg;
    enum CoroutineState state;
    int nID;
    NFCoroutineManager* pSchdule;

#if NF_PLATFORM != NF_PLATFORM_WIN
    ucontext_t ctx;
#endif

    char stack[MAX_COROUTINE_STACK_SIZE];
};

class NFCoroutineManager
{
public:

    NFCoroutineManager();

    virtual ~NFCoroutineManager();

    void Init(CoroutineFunction func);

    void StartCoroutine();
    void StartCoroutine(CoroutineFunction func);
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
    CoroutineFunction mxMainFunc;
    void* mpMainArg;

#if NF_PLATFORM != NF_PLATFORM_WIN
    ucontext_t mxMainCtx;
#endif

    int mnRunningCoroutineID;

    std::vector<NFCoroutine*> mxCoroutineList;
    std::list<int> mxRunningList;

    int mnMaxIndex;

};


#endif
