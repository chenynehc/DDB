#ifndef _TOPIC_BLOCK_H_
#define _TOPIC_BLOCK_H_

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif


//--------------------------------------------------------------------------//
//                                                                          //
// MACRO DEFINE                                                             //
//                                                                          //
//--------------------------------------------------------------------------//
#define TOPIC_NAME_LENGTH           32U

#define SUBSCRIBER_NAME_LENGTH      32U
typedef struct tagSubscriberItem
{
	char chSubscriberName[SUBSCRIBER_NAME_LENGTH];
    PTR_LIST_CELL pTopicDataCell;
    HANDLE hndNewNotifyEvent;
    HANDLE hndOverflowNotifyEvent;
	
}SUBSCRIBER_ITEM, * PTR_SUBSCRIBER_ITEM;

//--------------------------------------------------------------------------//
//                                                                          //
// CLASS DEFINE                                                             //
//                                                                          //
//--------------------------------------------------------------------------//
class CTopicBlock
{
public:
    CTopicBlock(void);
	
	virtual ~CTopicBlock(void);

public:
    BOOL Init(void);
    
    void * NewDataBuffer(void);
    
    BOOL DeleteDataBuffer(void * pBuffer);
    
    BOOL InsertNewAndRetireOldTopicData(void * pBuffer);
    
public:
    void * GetCurrentData(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);
    
    BOOL StepNextData(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);
    
public:
    void * ExclusiveGetCurrentData(void);

public:
    BOOL OverflowHandled(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);

public:
    BOOL SetTopicName(const char * pTopicName);
    
    BOOL GetTopicName(char * pTopicName);
    
    char * GetTopicNamePtr(void);
    
    void SetTopicDataSize(UWORD uwDataSize);
    
    void SetTopicDataMaxCount(UWORD uwMaxCount);
    
public:
    PTR_LIST_CELL InsertNewSubscriber(const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent);
    
    BOOL DeleteSubscriber(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);
    
    BOOL LogonExclusiveSubscriber(const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent);
    
    BOOL DeleteExclusiveSubscriber(void);
    
private:    
    BOOL DeleteHeadTopicData(void);
    
    BOOL NotifyNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
    BOOL NotifyDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
    BOOL UnnotifyNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
    BOOL UnnotifyDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);

    BOOL CreatNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
    BOOL CreatDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
    BOOL CloseNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
    BOOL CloseDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem);
    
private:
    BOOL FreeSubscriberList(void);
    
    void FreeExclusiveSubscriber(void);
    
private:
    char m_chTopicName[TOPIC_NAME_LENGTH];
    
private:
	CDList m_TopicDataList;
    
    UWORD m_uwTopicDataSize;
    
    UWORD m_uwTopicDataMaxCount;

private:    
	CDList m_SubscriberList;
    
private:
    BOOL m_IsExclusive;
    
    SUBSCRIBER_ITEM m_ExclusiveSubcriber;   //the pTopicDataCell of m_ExclusiveSubcriber has no use
    
};

#endif//_TOPIC_BLOCK_H_



