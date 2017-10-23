// -------------------------------------------------------------------------
//    @FileName			:    NFCPluginManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#include "NFCPluginManager.h"


void CoroutineExecute(void* arg)
{
	NFCPluginManager::Instance()->Execute();
}

NFCPluginManager::NFCPluginManager()
{
   mnAppID = 0;
}

NFCPluginManager::~NFCPluginManager()
{

}

bool NFCPluginManager::Awake()
{
	return true;
}

inline bool NFCPluginManager::Init()
{

	NFCoroutineManager::Instance()->Init(CoroutineExecute);

	return true;
}

void NFCPluginManager::ExecuteCoScheduler()
{
	NFCoroutineManager::Instance()->ScheduleJob();
}

void NFCPluginManager::StartCoroutine()
{
	NFCoroutineManager::Instance()->StartCoroutine();
}

void NFCPluginManager::StartCoroutine(CoroutineFunction func)
{
	NFCoroutineManager::Instance()->StartCoroutine(func);
}

void NFCPluginManager::Yield()
{
	NFCoroutineManager::Instance()->Yield();
}

bool NFCPluginManager::Execute()
{
	return false;
}
