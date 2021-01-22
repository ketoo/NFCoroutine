#include <time.h>
#include <iostream>
#include "NFCoroutineManager.hpp"
#include <chrono>

void setValue(uint value)
{
    std::cout << " setValue " << value << ", thread id " << std::this_thread::get_id() << std::endl;
}

void addValue(uint value)
{
    std::cout << " addValue " << value << ", thread id " << std::this_thread::get_id() << std::endl;
}

NFCoroutineTask doSomeThing()
{
    std::cout << " doSomeThing " << " thread id " << std::this_thread::get_id() << std::endl;
}

NFCoroutineTask  loadDataFromMysql()
{
    std::cout << time(0) << std::endl;
    std::cout << " loadDataFromMysql 11"  << " thread id " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds (3));

    std::cout << time(0) << std::endl;
    std::cout << " loadDataFromMysql 22"  << " thread id " << std::this_thread::get_id() << std::endl;
}

NFCoroutineTask updateEx(NFCoroutineManager* manager)
{
    std::cout << time(0) << std::endl;
    std::cout << " startCoroutine  "  << " thread id " << std::this_thread::get_id() << std::endl;

    co_await manager->StartCoroutine(loadDataFromMysql);

    static int value = 0;
    setValue(value++);

    std::cout << " startCoroutine(doSomeThing) "  << " thread id " << std::this_thread::get_id() << std::endl;

    co_await manager->StartCoroutine(doSomeThing);

    addValue(value++);
}

int main(int argc, char* argv[])
{
    std::cout << "hardware_concurrency=" << std::thread::hardware_concurrency() << std::endl;

    std::cout << "1" << " thread id " << std::this_thread::get_id() << std::endl;

    NFCoroutineManager manager;

    int count = 0;
    while (true)
    {
        count++;
        if (count % 1000 == 1)
        {
            updateEx(&manager);
        }

        manager.Execute();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
};
