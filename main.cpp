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



std::string strBuff;
NFCoroutineSchdule scheduleModule;
void sendMessage(std::string str)
{
    strBuff.clear();
    strBuff.append(str.c_str(), str.length());
}

int readBuff(char* buff)
{
    int nLen = strBuff.length();
    if (nLen > 0)
    {
        memccpy(buff, strBuff.c_str(), 0, nLen);
    }

    return nLen;
}

void DoBusiness2(void * arg)
{
    std::cout << "DoBusiness 2" << std::endl;
}

std::string readData;
bool test(void *arg)
{
    std::cout << "DoBusiness 1" << std::endl;

    while (1)
    {
        char buff[1024] = {0};
        int n = readBuff(buff);
        if (n > 0)
        {
            if (readData.length() == 3)
            {

                std::cout << "func end running: " << scheduleModule.GetRunningID() << std::endl;

                break;
            }

            readData.append(buff, n);
        }


        scheduleModule.Yield();
    }


    return false;
}


void DoBusiness1(void * arg)
{

    static int count = 0;
    sendMessage("1");

    if (count < 4)
    {
        count++;

        int iooo = time(0);

        std::cout << "count: " << count << std::endl;
        std::cout << "DoBusiness1 running: " << scheduleModule.GetRunningID() << " random " << iooo << std::endl;

        scheduleModule.Create(test, &scheduleModule);
        scheduleModule.Yield();

        std::cout << "count: " << count << std::endl;
        std::cout << "=========after DoBusiness1 running: " << scheduleModule.GetRunningID() << " random " << iooo << std::endl;
    }
}

bool update(void * arg)
{
    puts("---------");

    sleep(1);

    std::cout << "update begin running: " << scheduleModule.GetRunningID() << std::endl;


    //DoBusiness1(arg);

    std::cout << "update end running: " << scheduleModule.GetRunningID() << std::endl;

    return false;
}



int main()
{

    scheduleModule.Init(update, &scheduleModule);

    while (1)
    {
        scheduleModule.ScheduleJob();
    }

    std::cout << " main over " << std::endl;

    return 0;
}

