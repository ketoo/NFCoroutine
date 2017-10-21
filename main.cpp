#include <iostream>
#include <chrono>
#include <thread>
#include "NFCoroutineManager.h"


NFCoroutineManager mxCoroutineManager;

void DoBusiness()
{
    mxCoroutineManager.StartCoroutine();

    int i = 0;
    std::cout << "---test " << i << std::endl;
    mxCoroutineManager.Yield();

    i++;
    std::cout << "---test " << i << std::endl;

    mxCoroutineManager.Yield();

    i++;
    std::cout << "---test " << i << std::endl;


}

void CoroutineExecute(void* arg)
{
    std::cout << "CoroutineExecute" << std::endl;
    DoBusiness();

    //NFCPluginManager::Instance()->Execute();
}

int main()
{

    mxCoroutineManager.Init(CoroutineExecute);

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mxCoroutineManager.ScheduleJob();
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}