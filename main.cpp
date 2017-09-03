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

                return true;
            }

            readData.append(buff, n);
        }


        std::cout << "before running: " << scheduleModule.GetRunningID() << std::endl;
        scheduleModule.Yield();
    }


    return false;
}


void DoBusiness1(void * arg)
{
    puts("22");

    sendMessage("1");

    std::cout << "DoBusiness1 running: " << scheduleModule.GetRunningID() << std::endl;

    scheduleModule.Create(test, &scheduleModule);

    std::cout << "after DoBusiness1 running: " << scheduleModule.GetRunningID() << std::endl;

}

bool update(void * arg)
{
    while (1)
    {
        std::cout << "update begin running: " << scheduleModule.GetRunningID() << std::endl;


        DoBusiness1(arg);

        std::cout << "update end running: " << scheduleModule.GetRunningID() << std::endl;

    }

    return false;
}



int main()
{

    scheduleModule.Create(update, &scheduleModule);

    while (1)
    {
        scheduleModule.ScheduleJob();
    }


    return 0;
}

