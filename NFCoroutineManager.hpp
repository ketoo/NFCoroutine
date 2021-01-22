#pragma once
#include <experimental/coroutine>
#include <iostream>
#include <thread>
#include <memory>
#include <list>
#include "concurrentqueue.h"

class NFCoroutineManager;
class NFCoroutineTask;
class NFCoroutineHandler;

typedef NFCoroutineTask (*NFCoroutineAsyncFunc)();
void AddHandler(NFCoroutineManager* manager, NFCoroutineHandler* handler);

class NFCoroutineHandler
{
public:
    NFCoroutineAsyncFunc asyncFunc;
    std::experimental::coroutine_handle<> coroutineHandler;

    NFCoroutineHandler() = default;
};

struct NFCoroutineTask
{
    struct promise_type {
        static NFCoroutineTask get_return_object_on_allocation_failure() { return {}; }
        NFCoroutineTask get_return_object() { return {}; }
        std::experimental::suspend_never initial_suspend() { return {}; }
        std::experimental::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

class NFCoroutineAwaitable
{
public:
    NFCoroutineAwaitable(NFCoroutineManager* manager, NFCoroutineHandler* handler)
    {
        coroutineManager = manager;
        coroutineHandler = handler;
    }

    NFCoroutineManager* coroutineManager = nullptr;
    NFCoroutineHandler* coroutineHandler = nullptr;

    virtual bool await_ready() const { return false; }
    virtual void await_resume() {}

    void await_suspend(std::experimental::coroutine_handle<> handle)
    {
        coroutineHandler->coroutineHandler = handle;
        AddHandler(coroutineManager, coroutineHandler);
    }
};

class NFCoroutineManager
{
public:
    NFCoroutineManager()
    {
        workerThread = std::thread(NFCoroutineManager::ThreadWorker, this);
        workerThread.detach();
    }

    NFCoroutineAwaitable StartCoroutine(NFCoroutineAsyncFunc func)
    {
        NFCoroutineHandler* data = new NFCoroutineHandler();
        data->asyncFunc = func;
        return NFCoroutineAwaitable{this, data};
    }

    void Execute()
    {
        NFCoroutineHandler* data = nullptr;
        if (this->mainThreadHandles.try_dequeue(data))
        {
            data->coroutineHandler.resume();
        }
    }

    void Enqueue(NFCoroutineHandler* handler)
    {
        workerThreadHandles.try_enqueue(handler);
    }
protected:
    static void ThreadWorker(NFCoroutineManager* manager)
    {
        while (true)
        {
            NFCoroutineHandler* data = nullptr;
            if (manager->workerThreadHandles.try_dequeue(data))
            {
                data->asyncFunc();
                manager->mainThreadHandles.try_enqueue(data);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

private:
    std::thread workerThread;
    moodycamel::ConcurrentQueue<NFCoroutineHandler*> mainThreadHandles;
    moodycamel::ConcurrentQueue<NFCoroutineHandler*> workerThreadHandles;
};

void AddHandler(NFCoroutineManager* manager, NFCoroutineHandler* handler)
{
    manager->Enqueue(handler);
}