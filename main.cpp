#include "NFCoroutineManager.h"
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>
#include <unistd.h>

NFCoroutineManager scheduleModule;

void DoBusiness(bool b)
{
    int i = 0;
    std::cout << "---test " << i << std::endl;
    scheduleModule.Yield();

    if (b)
    {
        scheduleModule.Yield(1000 * 5);
    }

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
        DoBusiness(b);
    }

return;
    int n = rand() % 10;

    if (n == 0 || n == 1 || n == 3 || n == 5 || n == 7 || n == 9)
    {
        DoBusiness(false);
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

