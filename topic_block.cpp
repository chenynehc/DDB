
#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DList.h"
#include "topic_block.h"

//--------------------------------------------------------------------------//
//                                                                          //
// MACRO DEFINE                                                             //
//                                                                          //
//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//
//                                                                          //
// VARIABLE DEFINE                                                          //
//                                                                          //
//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//
//                                                                          //
// FUNCTION DEFINE                                                          //
//                                                                          //
//--------------------------------------------------------------------------//
CTopicBlock::CTopicBlock(void)
{
	Init();
}

CTopicBlock::~CTopicBlock(void)
{
	Init();
}

BOOL CTopicBlock::Init(void)
{
    BOOL RetVal = FALSE;
    
    memset(m_chTopicName, 0x00, sizeof(char)*TOPIC_NAME_LENGTH);
    m_TopicDataList.Free();
    m_uwTopicDataSize = 0U;
    m_uwTopicDataMaxCount = 0U;
    RetVal = FreeSubscriberList();   //instead of m_SubscriberList.Free()
    FreeExclusiveSubscriber();
    
    return RetVal;
}

void * CTopicBlock::NewDataBuffer(void)
{
    return new BYTE[m_uwTopicDataSize];
}

BOOL CTopicBlock::DeleteDataBuffer(void * pBuffer)
{
    BOOL RetVal = FALSE;
    
    if(pBuffer != NULL)
    {
        delete[] pBuffer;
        
        RetVal = TRUE;
    }
    
    return RetVal;
}

BOOL CTopicBlock::InsertNewAndRetireOldTopicData(void * pBuffer)
{
    BOOL RetVal = FALSE;
    
    if(pBuffer != NULL)
    {
        PTR_LIST_CELL pCell = m_TopicDataList.Insert(pBuffer);
        
        if(pCell != NULL)
        {
            BOOL OverMaxLen = (m_TopicDataList.GetCount() > m_uwTopicDataMaxCount) ? TRUE : FALSE;
            
            if(m_IsExclusive == FALSE)
            {
                PTR_LIST_CELL pIter = NULL;
                PTR_SUBSCRIBER_ITEM pSubscriberItem = NULL;
                
                RetVal = m_SubscriberList.ResetIter(&pIter);
                
                if(RetVal == TRUE)
                {
                    BOOL DataHeadPointed = FALSE;
                    
                    while(m_SubscriberList.IterRead(&pIter, (void * *)(&pSubscriberItem)) == TRUE)
                    {
                        if((m_TopicDataList.IsHead(pSubscriberItem->pTopicDataCell) == TRUE) && (pSubscriberItem->pTopicDataCell != NULL))
                        {
                            if(OverMaxLen == TRUE)
                            {
                                void * pDummyRead = NULL;
                                RetVal = m_TopicDataList.IterRead(&(pSubscriberItem->pTopicDataCell), &pDummyRead);    //step pTopicDataCell to next
                                
                                NotifyDataOverflowEvent(pSubscriberItem);
                            }
                            else
                            {
                                DataHeadPointed = TRUE;
                            }
                        }
                        else if(pSubscriberItem->pTopicDataCell == NULL)
                        {
                            RetVal = m_TopicDataList.GoEndIter(&(pSubscriberItem->pTopicDataCell));
                                
                            if(m_TopicDataList.IsHead(pSubscriberItem->pTopicDataCell) == TRUE)
                            {
                                DataHeadPointed = TRUE;
                            }
                        }
                        else
                        {
                            
                        }
                        
                        NotifyNewDataEvent(pSubscriberItem);
                    }
                    
                    if(DataHeadPointed == FALSE)
                    {
                        RetVal = DeleteHeadTopicData();
                    }
                }
            }
            else
            {
                if(OverMaxLen == TRUE)
                {
                    RetVal = DeleteHeadTopicData();
                    
                    NotifyDataOverflowEvent(&m_ExclusiveSubcriber);
                }
                
                NotifyNewDataEvent(&m_ExclusiveSubcriber);
            }
        }
    }
    
    return RetVal;
}

void * CTopicBlock::GetCurrentData(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
    void * pRetVal = NULL;
    
    if(pSubscriberCell != NULL)
    {
        PTR_SUBSCRIBER_ITEM pSubscriberItem = PTR_SUBSCRIBER_ITEM(pSubscriberCell->pItem);
        if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
        {
            if(pSubscriberItem->pTopicDataCell != NULL)
            {
                pRetVal = pSubscriberItem->pTopicDataCell->pItem;
                if(m_TopicDataList.IsTail(pSubscriberItem->pTopicDataCell) == TRUE)
                {
                    UnnotifyNewDataEvent(pSubscriberItem);
                }
            }
        }
    }
    else
    {
        if(pSubscriberName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            PTR_SUBSCRIBER_ITEM pSubscriberItem = NULL;
            
            Ret = m_SubscriberList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_SubscriberList.IterRead(&pIter, (void * *)(&pSubscriberItem)) == TRUE)
                {
                    if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
                    {
                        if(pSubscriberItem->pTopicDataCell != NULL)
                        {
                            pRetVal = pSubscriberItem->pTopicDataCell->pItem;
                            if(m_TopicDataList.IsTail(pSubscriberItem->pTopicDataCell) == TRUE)
                            {
                                UnnotifyNewDataEvent(pSubscriberItem);
                            }
                        }
                        
                        break;
                    }
                }
            }
        }
    }
    
    return pRetVal;
}

BOOL CTopicBlock::StepNextData(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
    BOOL RetVal = FALSE;
    
    if(pSubscriberCell != NULL)
    {
        PTR_SUBSCRIBER_ITEM pSubscriberItem = PTR_SUBSCRIBER_ITEM(pSubscriberCell->pItem);
        if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
        {
            void * pDummyRead = NULL;
            RetVal = m_TopicDataList.IterRead(&(pSubscriberItem->pTopicDataCell), &pDummyRead);
        }
    }
    else
    {
        if(pSubscriberName != NULL)
        {
            PTR_LIST_CELL pIter = NULL;
            PTR_SUBSCRIBER_ITEM pSubscriberItem = NULL;
            
            RetVal = m_SubscriberList.ResetIter(&pIter);
            
            if(RetVal == TRUE)
            {
                while(m_SubscriberList.IterRead(&pIter, (void * *)(&pSubscriberItem)) == TRUE)
                {
                    if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
                    {
                        void * pDummyRead = NULL;
                        RetVal = m_TopicDataList.IterRead(&(pSubscriberItem->pTopicDataCell), &pDummyRead);
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

void * CTopicBlock::ExclusiveGetCurrentData(void)
{
    void * pRetVal = NULL;
    
    if(m_IsExclusive == TRUE)
    {
        m_TopicDataList.ReadOut(&pRetVal);
        if(m_TopicDataList.GetCount() == 0)
        {
            UnnotifyNewDataEvent(&m_ExclusiveSubcriber);
        }
    }
    
    return pRetVal;
}

BOOL CTopicBlock::OverflowHandled(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
    BOOL RetVal = FALSE;
    
    if(m_IsExclusive == FALSE)
    {
        if(pSubscriberCell != NULL)
        {
            PTR_SUBSCRIBER_ITEM pSubscriberItem = PTR_SUBSCRIBER_ITEM(pSubscriberCell->pItem);
            if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
            {
                RetVal = UnnotifyDataOverflowEvent(pSubscriberItem);
            }
        }
        else
        {
            if(pSubscriberName != NULL)
            {
                PTR_LIST_CELL pIter = NULL;
                PTR_SUBSCRIBER_ITEM pSubscriberItem = NULL;
                
                RetVal = m_SubscriberList.ResetIter(&pIter);
                
                if(RetVal == TRUE)
                {
                    while(m_SubscriberList.IterRead(&pIter, (void * *)(&pSubscriberItem)) == TRUE)
                    {
                        if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
                        {
                            RetVal = UnnotifyDataOverflowEvent(pSubscriberItem);
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        RetVal = UnnotifyDataOverflowEvent(&m_ExclusiveSubcriber);
    }
    
    return RetVal;
}
 
BOOL CTopicBlock::SetTopicName(const char * pTopicName)
{
    BOOL RetVal = FALSE;
    
    if(strlen(pTopicName) <= TOPIC_NAME_LENGTH)
    {
        strcpy_s(m_chTopicName, TOPIC_NAME_LENGTH, pTopicName);
        RetVal = TRUE;
    }
    
    return RetVal;
}

BOOL CTopicBlock::GetTopicName(char * pTopicName)
{
    BOOL RetVal = FALSE;
    
    if(pTopicName != NULL)
    {
        strcpy_s(pTopicName, TOPIC_NAME_LENGTH, m_chTopicName);
        RetVal = TRUE;
    }
    
    return RetVal;
}

char * CTopicBlock::GetTopicNamePtr(void)
{    
    return m_chTopicName;
}

void CTopicBlock::SetTopicDataSize(UWORD uwDataSize)
{
    m_uwTopicDataSize = uwDataSize;
}

void CTopicBlock::SetTopicDataMaxCount(UWORD uwMaxCount)
{
    m_uwTopicDataMaxCount = (uwMaxCount > 1) ? uwMaxCount : 1;
}

PTR_LIST_CELL CTopicBlock::InsertNewSubscriber(const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent)
{
    PTR_LIST_CELL pRetVal = NULL;
    
    PTR_SUBSCRIBER_ITEM pSubscriberItem = new SUBSCRIBER_ITEM;
    
    if(pSubscriberItem != NULL)
    {
        strcpy_s(pSubscriberItem->chSubscriberName, SUBSCRIBER_NAME_LENGTH, pSubscriberName);
        m_TopicDataList.ResetIter(&(pSubscriberItem->pTopicDataCell));
        CreatNewDataEvent(pSubscriberItem);
        CreatDataOverflowEvent(pSubscriberItem);
        
        pRetVal = m_SubscriberList.Insert(pSubscriberItem);
        
        if(pRetVal != NULL)
        {
            if(phndNewNotifyEvent != NULL)
            {
                * phndNewNotifyEvent = pSubscriberItem->hndNewNotifyEvent;
            }
            
            if(phndOverflowNotifyEvent != NULL)
            {
                * phndOverflowNotifyEvent = pSubscriberItem->hndOverflowNotifyEvent;
            }
        }
        else
        {
            CloseNewDataEvent(pSubscriberItem);
            CloseDataOverflowEvent(pSubscriberItem);
            
            delete pSubscriberItem;
        }
    }
    
    return pRetVal;
}

BOOL CTopicBlock::DeleteSubscriber(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
    BOOL RetVal = FALSE;
    
    if(pSubscriberCell != NULL)
    {
        PTR_SUBSCRIBER_ITEM pSubscriberItem = PTR_SUBSCRIBER_ITEM(pSubscriberCell->pItem);
        if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
        {
            CloseNewDataEvent(pSubscriberItem);
            CloseDataOverflowEvent(pSubscriberItem);
            
            RetVal = m_SubscriberList.RemoveCell(pSubscriberCell);
        }
    }
    else
    {
        if(pSubscriberName != NULL)
        {
            PTR_LIST_CELL pIter = NULL;
            PTR_SUBSCRIBER_ITEM pSubscriberItem = NULL;
            
            RetVal = m_SubscriberList.ResetIter(&pIter);
            
            if(RetVal == TRUE)
            {
                while(m_SubscriberList.IterRead(&pIter, (void * *)(&pSubscriberItem)) == TRUE)
                {
                    if(strcmp(pSubscriberItem->chSubscriberName, pSubscriberName) == 0)
                    {
                        CloseNewDataEvent(pSubscriberItem);
                        CloseDataOverflowEvent(pSubscriberItem);
                        
                        RetVal = m_SubscriberList.RemoveCell(pIter);
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

BOOL CTopicBlock::LogonExclusiveSubscriber(const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent)
{
    BOOL RetVal = FALSE;
    
    if(m_IsExclusive == FALSE)
    {
        RetVal = FreeSubscriberList();
        
        strcpy_s(m_ExclusiveSubcriber.chSubscriberName, SUBSCRIBER_NAME_LENGTH, pSubscriberName);
        m_TopicDataList.ResetIter(&(m_ExclusiveSubcriber.pTopicDataCell));  //actually no use
        CreatNewDataEvent(&m_ExclusiveSubcriber);
        CreatDataOverflowEvent(&m_ExclusiveSubcriber);
        m_IsExclusive = TRUE;
        
        if(phndNewNotifyEvent != NULL)
        {
            * phndNewNotifyEvent = m_ExclusiveSubcriber.hndNewNotifyEvent;
        }
        
        if(phndOverflowNotifyEvent != NULL)
        {
            * phndOverflowNotifyEvent = m_ExclusiveSubcriber.hndOverflowNotifyEvent;
        }
    }
    
    return RetVal;
}

BOOL CTopicBlock::DeleteExclusiveSubscriber(void)
{
    BOOL RetVal = FALSE;
    
    RetVal = FreeSubscriberList();
    FreeExclusiveSubscriber();
    
    return TRUE;
}

BOOL CTopicBlock::DeleteHeadTopicData(void)
{
    BOOL RetVal = FALSE;
    
    void * pDummyRead = NULL;
    RetVal = m_TopicDataList.ReadOut(&pDummyRead);
    delete[] pDummyRead;
    
    return RetVal;
}

BOOL CTopicBlock::NotifyNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndNewNotifyEvent = pSubscriberItem->hndNewNotifyEvent;
    if(hndNewNotifyEvent != NULL)
    {
        ::SetEvent(hndNewNotifyEvent);
    }
    
    return RetVal;
}

BOOL CTopicBlock::NotifyDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndOverflowNotifyEvent = pSubscriberItem->hndOverflowNotifyEvent;
    if(hndOverflowNotifyEvent != NULL)
    {
        ::SetEvent(hndOverflowNotifyEvent);
    }
    
    return RetVal;
}

BOOL CTopicBlock::UnnotifyNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndNewNotifyEvent = pSubscriberItem->hndNewNotifyEvent;
    if(hndNewNotifyEvent != NULL)
    {
        ::ResetEvent(hndNewNotifyEvent);
    }
    
    return RetVal;
}

BOOL CTopicBlock::UnnotifyDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndOverflowNotifyEvent = pSubscriberItem->hndOverflowNotifyEvent;
    if(hndOverflowNotifyEvent != NULL)
    {
        ::ResetEvent(hndOverflowNotifyEvent);
    }
    
    return RetVal;
}

BOOL CTopicBlock::CreatNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndNewNotifyEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
    if(hndNewNotifyEvent != NULL)
    {
        pSubscriberItem->hndNewNotifyEvent = hndNewNotifyEvent;
    }
    
    return RetVal;
}

BOOL CTopicBlock::CreatDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndOverflowNotifyEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
    if(hndOverflowNotifyEvent != NULL)
    {
        pSubscriberItem->hndOverflowNotifyEvent = hndOverflowNotifyEvent;
    }
    
    return RetVal;
}

BOOL CTopicBlock::CloseNewDataEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndNewNotifyEvent = pSubscriberItem->hndNewNotifyEvent;
    if(hndNewNotifyEvent != NULL)
    {
        ::CloseHandle(hndNewNotifyEvent);
    }
    
    return RetVal;
}

BOOL CTopicBlock::CloseDataOverflowEvent(PTR_SUBSCRIBER_ITEM pSubscriberItem)
{
    BOOL RetVal = FALSE;
    
    HANDLE hndOverflowNotifyEvent = pSubscriberItem->hndOverflowNotifyEvent;
    if(hndOverflowNotifyEvent != NULL)
    {
        ::CloseHandle(hndOverflowNotifyEvent);
    }
    
    return RetVal;
}

BOOL CTopicBlock::FreeSubscriberList(void)
{
    BOOL RetVal = FALSE;
    
    PTR_SUBSCRIBER_ITEM pSubscriberItem = NULL;
    
    while(m_SubscriberList.ReadOut((void * *)(&pSubscriberItem)) == TRUE)
    {
        CloseNewDataEvent(pSubscriberItem);
        CloseDataOverflowEvent(pSubscriberItem);
        
        delete pSubscriberItem;
    }
    
    if(m_SubscriberList.GetCount() == 0)
    {
        RetVal = TRUE;
    }
    
    return RetVal;
}

void CTopicBlock::FreeExclusiveSubscriber(void)
{
    if(m_IsExclusive == TRUE)
    {
        memset(m_ExclusiveSubcriber.chSubscriberName, 0x00, sizeof(char)*SUBSCRIBER_NAME_LENGTH);
        m_ExclusiveSubcriber.pTopicDataCell = NULL;
        CloseNewDataEvent(&m_ExclusiveSubcriber);
        CloseDataOverflowEvent(&m_ExclusiveSubcriber);
        m_ExclusiveSubcriber.hndNewNotifyEvent = NULL;
        m_ExclusiveSubcriber.hndOverflowNotifyEvent = NULL;
        
        m_IsExclusive = FALSE;
    }
    else
    {
        memset(m_ExclusiveSubcriber.chSubscriberName, 0x00, sizeof(char)*SUBSCRIBER_NAME_LENGTH);
        m_ExclusiveSubcriber.pTopicDataCell = NULL;
        m_ExclusiveSubcriber.hndNewNotifyEvent = NULL;
        m_ExclusiveSubcriber.hndOverflowNotifyEvent = NULL;
        
        m_IsExclusive = FALSE;
    }
}

