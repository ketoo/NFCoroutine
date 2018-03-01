#include "NFCoroutine.h"
#include <iostream>
#include <zconf.h>
#include <thread>

NFCoroutineManager scheduleModule;

void DoBusiness()
{
    scheduleModule.StartCoroutine();

    int i = 0;
    std::cout << "---test " << i << std::endl;
    scheduleModule.YieldCo();

    i++;
    std::cout << "---test " << i << std::endl;

    scheduleModule.YieldCo();

    i++;
    std::cout << "---test " << i << std::endl;


}

void update(void* arg)
{
    puts("---------");

    sleep(1);

    DoBusiness();
}

void call_from_thread()
{
    std::cout << "Hello, thread" << std::endl;
}

int main()
{
    //std::thread t1(call_from_thread);
    //t1.join();


    srand((unsigned)time(0));

    scheduleModule.Init(update);

    while (1)
    {
        scheduleModule.ScheduleJob();
    }

    std::cout << " mxMainCtx over " << std::endl;

    return 0;
}

