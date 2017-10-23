// -------------------------------------------------------------------------
//    @FileName			:    NFCPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFC_PLUGIN_MANAGER_H
#define NFC_PLUGIN_MANAGER_H

#include <map>
#include <string>
#include <time.h>
#include "NFCoroutineManager.h"
#include "NFSingleton.hpp"

void CoroutineExecute(void* arg);

class NFCPluginManager
	: public NFSingleton<NFCPluginManager>
{
public:
    NFCPluginManager();
    virtual ~NFCPluginManager();

	virtual bool Awake();

	virtual bool Init();

	virtual bool Execute();



	virtual void ExecuteCoScheduler();

	virtual void StartCoroutine();

	virtual void StartCoroutine(CoroutineFunction func);

	virtual void Yield();


private:
    int mnAppID;

};

#endif
