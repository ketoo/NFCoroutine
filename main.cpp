#include "NFCoroutine.h"
#include <stdio.h>
#include <iostream>
#include <zconf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include<sys/ioctl.h>


//inet_addr()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MIN 1    //随机数产生的范围
#define MAX 5
int readBuf()
{

    float r = (rand() / float(RAND_MAX + 1));
    int n = MIN + (int)(MAX * r);
    return std::abs(n);
}

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

    //test(NULL);

    //scheduleModule.StartCoroutine();
    //scheduleModule.Yield();
}

void update(void* arg)
{
    puts("---------");

    sleep(1);

    std::cout << "update begin running: " << scheduleModule.GetRunningID() << std::endl;


    DoBusiness();

    std::cout << "update end running: " << scheduleModule.GetRunningID() << std::endl;
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

