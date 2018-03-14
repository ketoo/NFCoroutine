#include "NFCoroutine.h"
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>
#include <unistd.h>

NFCoroutineSchedule scheduleModule;

void DoBusiness()
{
    int i = 0;
    std::cout << "---test " << i << std::endl;
    scheduleModule.Yield();

    i++;
    std::cout << "---test " << i << std::endl;

    scheduleModule.Yield();

    i++;
    std::cout << "---test " << i << std::endl;

    scheduleModule.Yield();
}

void update(void* arg)
{
    puts("---------");


    static bool b = false;
    if (!b)
    {
        b = true;
        DoBusiness();
    }

    return;

    int n = rand() % 10;

    if (n == 0 || n == 1 || n == 3 || n == 5 || n == 7 || n == 9)
    {
        DoBusiness();
    }
}


int main()
{
    srand (time(NULL));

    scheduleModule.Init(update);

    while (1)
    {

        sleep(1);
        scheduleModule.ScheduleJob();
    }

    std::cout << " mxMainCtx over " << std::endl;

    return 0;
}

