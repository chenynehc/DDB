#ifndef _DLL_DDB_H_
#define _DLL_DDB_H_

#ifdef _DDB_LITE
#define DDB_API __declspec(dllexport)
#else
#define DDB_API __declspec(dllimport)
#endif

typedef struct tagListCell
{
	void * pItem;
	struct tagListCell * pPrev;
	struct tagListCell * pNext;
	
}LIST_CELL, * PTR_LIST_CELL;

extern "C" DDB_API BOOL DDB_Init(void);

extern "C" DDB_API void * DDB_GetTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell);

extern "C" DDB_API BOOL DDB_CancleTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer);

extern "C" DDB_API BOOL DDB_DoPublish(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer);

extern "C" DDB_API void * DDB_ReadTopic(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell);

extern "C" DDB_API BOOL DDB_ReadTopicDone(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell);

extern "C" DDB_API void * DDB_ReadOutTopic(const char * pTopicName, PTR_LIST_CELL pTopicCell);

extern "C" DDB_API BOOL DDB_OverflowHandleDone(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);

extern "C" DDB_API PTR_LIST_CELL DDB_RegistPublisher(const char * pTopicName, UWORD uwDataSize, UWORD uwMaxCount);

extern "C" DDB_API BOOL DDB_DeRegistPublisher(const char * pTopicName, PTR_LIST_CELL pTopicCell);

extern "C" DDB_API BOOL DDB_Subscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, PTR_LIST_CELL * ppSubscriberCell, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent);

extern "C" DDB_API BOOL DDB_DeSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);

extern "C" DDB_API BOOL DDB_ExclusiveSubscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent);

extern "C" DDB_API BOOL DDB_DeExclusiveSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell);


#endif//_DLL_DDB_H_

