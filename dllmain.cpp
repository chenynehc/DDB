// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "DList.h"
#include "topic_block.h"
#include "topic_service.h"

#define _DDB_LITE

#ifdef _DDB_LITE
#define DDB_API __declspec(dllexport)
#else
#define DDB_API __declspec(dllimport)
#endif

static CTopicService m_DDB;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" DDB_API BOOL DDB_Init(void)
{
	return m_DDB.Init();
}

extern "C" DDB_API void * DDB_GetTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
	return m_DDB.GetTopicBuffer(pTopicName, pTopicCell);
}

extern "C" DDB_API BOOL DDB_CancleTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer)
{
	return m_DDB.CancleTopicBuffer(pTopicName, pTopicCell, pTopicBuffer);
}

extern "C" DDB_API BOOL DDB_DoPublish(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer)
{
	return m_DDB.DoPublish(pTopicName, pTopicCell, pTopicBuffer);
}

extern "C" DDB_API void * DDB_ReadTopic(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
	return m_DDB.ReadTopic(pSubscriberName, pSubscriberCell, pTopicName, pTopicCell);
}

extern "C" DDB_API BOOL DDB_ReadTopicDone(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
	return m_DDB.ReadTopicDone(pSubscriberName, pSubscriberCell, pTopicName, pTopicCell);
}

extern "C" DDB_API void * DDB_ReadOutTopic(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
	return m_DDB.ReadOutTopic(pTopicName, pTopicCell);
}

extern "C" DDB_API BOOL DDB_OverflowHandleDone(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
	return m_DDB.OverflowHandleDone(pTopicName, pTopicCell, pSubscriberName, pSubscriberCell);
}

extern "C" DDB_API PTR_LIST_CELL DDB_RegistPublisher(const char * pTopicName, UWORD uwDataSize, UWORD uwMaxCount)
{
	return m_DDB.RegistPublisher(pTopicName, uwDataSize, uwMaxCount);
}

extern "C" DDB_API BOOL DDB_DeRegistPublisher(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
	return m_DDB.DeRegistPublisher(pTopicName, pTopicCell);
}

extern "C" DDB_API BOOL DDB_Subscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, PTR_LIST_CELL * ppSubscriberCell, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent)
{
	return m_DDB.Subscribe(pTopicName, ppTopicCell, pSubscriberName, ppSubscriberCell, phndNewNotifyEvent, phndOverflowNotifyEvent);
}

extern "C" DDB_API BOOL DDB_DeSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
	return m_DDB.DeSubcribe(pTopicName, pTopicCell, pSubscriberName, pSubscriberCell);
}

extern "C" DDB_API BOOL DDB_ExclusiveSubscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent)
{
	return m_DDB.ExclusiveSubscribe(pTopicName, ppTopicCell, pSubscriberName, phndNewNotifyEvent, phndOverflowNotifyEvent);
}

extern "C" DDB_API BOOL DDB_DeExclusiveSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
	return m_DDB.DeExclusiveSubcribe(pTopicName, pTopicCell);
}

