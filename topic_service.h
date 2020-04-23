#ifndef _TOPIC_SERVICE_H_
#define _TOPIC_SERVICE_H_

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


//--------------------------------------------------------------------------//
//                                                                          //
// CLASS DEFINE                                                             //
//                                                                          //
//--------------------------------------------------------------------------//
class CTopicService
{
public:
    CTopicService(void);
	
	virtual ~CTopicService(void);

public:
    BOOL Init(void);
    
    void * GetTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell);
    
    BOOL CancleTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer);
    
    BOOL DoPublish(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer);
    
    void * ReadTopic(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell);
    
    BOOL ReadTopicDone(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell);
    
    void * ReadOutTopic(const char * pTopicName, PTR_LIST_CELL pTopicCell);
    
public:
    BOOL OverflowHandleDone(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);
    
public:
    PTR_LIST_CELL RegistPublisher(const char * pTopicName, UWORD uwDataSize, UWORD uwMaxCount);
    
    BOOL DeRegistPublisher(const char * pTopicName, PTR_LIST_CELL pTopicCell);
    
    BOOL Subscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, PTR_LIST_CELL * ppSubscriberCell, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent);
    
    BOOL DeSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell);
    
    BOOL ExclusiveSubscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent);
    
    BOOL DeExclusiveSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell);
    
private:
    BOOL FreeTopicList(void);    

private:
	CDList m_TopicList;
    
};

#endif//_TOPIC_SERVICE_H_



