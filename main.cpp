#include "NFCoroutine.h"
#include <iostream>
#include <zconf.h>

NFCoroutineSchedule scheduleModule;

void DoBusiness()
{
    scheduleModule.StartCoroutine();

    int i = 0;
    std::cout << "---test " << i << std::endl;
    scheduleModule.Yield();

    i++;
    std::cout << "---test " << i << std::endl;

    scheduleModule.Yield();

    i++;
    std::cout << "---test " << i << std::endl;


}

void update(void* arg)
{
    puts("---------");

    sleep(1);

    DoBusiness();
}


int main()
{
    srand((unsigned)time(0));

    scheduleModule.Init(update);

    while (1)
    {
        scheduleModule.ScheduleJob();
    }

    std::cout << " mxMainCtx over " << std::endl;

    return 0;
}

