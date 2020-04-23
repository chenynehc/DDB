
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
#include "topic_service.h"

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
CTopicService::CTopicService(void)
{
	Init();
}

CTopicService::~CTopicService(void)
{
	Init();
}

BOOL CTopicService::Init(void)
{
    return FreeTopicList();
}

BOOL CTopicService::FreeTopicList(void)
{
    BOOL RetVal = FALSE;
    
    CTopicBlock * pTopicBlock = NULL;
    
    while(m_TopicList.ReadOut((void * *)(&pTopicBlock)) == TRUE)
    {
        pTopicBlock->Init();
        
        delete pTopicBlock;
    }
    
    if(m_TopicList.GetCount() == 0)
    {
        RetVal = TRUE;
    }
    
    return RetVal;
}

void * CTopicService::GetTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
    void * pRetVal = NULL;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            pRetVal = pTopicBlock->NewDataBuffer();
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        pRetVal = pTopicBlock->NewDataBuffer();
                        break;
                    }
                }
            }
        }
    }
    
    return pRetVal;
}

BOOL CTopicService::CancleTopicBuffer(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->DeleteDataBuffer(pTopicBuffer);
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->DeleteDataBuffer(pTopicBuffer);
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

BOOL CTopicService::DoPublish(const char * pTopicName, PTR_LIST_CELL pTopicCell, void * pTopicBuffer)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->InsertNewAndRetireOldTopicData(pTopicBuffer);
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->InsertNewAndRetireOldTopicData(pTopicBuffer);
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

void * CTopicService::ReadTopic(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
    void * pRetVal = NULL;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            pRetVal = pTopicBlock->GetCurrentData(pSubscriberName, pSubscriberCell);
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        pRetVal = pTopicBlock->GetCurrentData(pSubscriberName, pSubscriberCell);
                        break;
                    }
                }
            }
        }
    }
    
    return pRetVal;
}

BOOL CTopicService::ReadTopicDone(const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell, const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->StepNextData(pSubscriberName, pSubscriberCell);
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->StepNextData(pSubscriberName, pSubscriberCell);
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

void * CTopicService::ReadOutTopic(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
    void * pRetVal = NULL;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            pRetVal = pTopicBlock->ExclusiveGetCurrentData();
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        pRetVal = pTopicBlock->ExclusiveGetCurrentData();
                        break;
                    }
                }
            }
        }
    }
    
    return pRetVal;
}

BOOL CTopicService::OverflowHandleDone(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->OverflowHandled(pSubscriberName, pSubscriberCell);
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->OverflowHandled(pSubscriberName, pSubscriberCell);
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

PTR_LIST_CELL CTopicService::RegistPublisher(const char * pTopicName, UWORD uwDataSize, UWORD uwMaxCount)
{
    PTR_LIST_CELL pTopicCell = NULL;
    
    if((pTopicName != NULL) && (uwDataSize > 0) && (uwMaxCount > 0))
    {
        CTopicBlock * pTopicBlock = new CTopicBlock;
        
        pTopicBlock->SetTopicName(pTopicName);
        
        pTopicBlock->SetTopicDataSize(uwDataSize);
        
        pTopicBlock->SetTopicDataMaxCount(uwMaxCount);
        
        pTopicCell = m_TopicList.Insert(pTopicBlock);
    }
    
    return pTopicCell;
}

BOOL CTopicService::DeRegistPublisher(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->Init();
            
            if(RetVal == TRUE)
            {
                RetVal = m_TopicList.RemoveCell(pTopicCell);
            }
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->Init();
            
                        if(RetVal == TRUE)
                        {
                            RetVal = m_TopicList.RemoveCell(pTopicCell);
                        }
                        
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

BOOL CTopicService::Subscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, PTR_LIST_CELL * ppSubscriberCell, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent)
{
    BOOL RetVal = FALSE;
    
    if(pTopicName != NULL)
    {
        BOOL Ret = FALSE;
        PTR_LIST_CELL pIter = NULL;
        CTopicBlock * pTopicBlock = NULL;
        
        Ret = m_TopicList.ResetIter(&pIter);
        
        if(Ret == TRUE)
        {
            while(pIter != NULL)
            {
                pTopicBlock = (CTopicBlock *)(pIter->pItem);
                if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                {
                    * ppTopicCell = pIter;
                    
                    * ppSubscriberCell = pTopicBlock->InsertNewSubscriber(pSubscriberName, phndNewNotifyEvent, phndOverflowNotifyEvent);
                    
                    RetVal = TRUE;
                    
                    break;
                }
                else
                {
                    m_TopicList.StepIter(&pIter);
                }
            }
        }
    }
    
    return RetVal;
}

BOOL CTopicService::DeSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell, const char * pSubscriberName, PTR_LIST_CELL pSubscriberCell)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->DeleteSubscriber(pSubscriberName, pSubscriberCell);
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->DeleteSubscriber(pSubscriberName, pSubscriberCell);
                        
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

BOOL CTopicService::ExclusiveSubscribe(const char * pTopicName, PTR_LIST_CELL * ppTopicCell, const char * pSubscriberName, HANDLE * phndNewNotifyEvent, HANDLE * phndOverflowNotifyEvent)
{
    BOOL RetVal = FALSE;
    
    if(pTopicName != NULL)
    {
        BOOL Ret = FALSE;
        PTR_LIST_CELL pIter = NULL;
        CTopicBlock * pTopicBlock = NULL;
        
        Ret = m_TopicList.ResetIter(&pIter);
        
        if(Ret == TRUE)
        {
            while(pIter != NULL)
            {
                pTopicBlock = (CTopicBlock *)(pIter->pItem);
                if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                {
                    * ppTopicCell = pIter;
                    
                    RetVal = pTopicBlock->LogonExclusiveSubscriber(pSubscriberName, phndNewNotifyEvent, phndOverflowNotifyEvent);

                    break;
                }
                else
                {
                    m_TopicList.StepIter(&pIter);
                }
            }
        }
    }
    
    return RetVal;
}

BOOL CTopicService::DeExclusiveSubcribe(const char * pTopicName, PTR_LIST_CELL pTopicCell)
{
    BOOL RetVal = FALSE;
    
    if(pTopicCell != NULL)
    {
        CTopicBlock * pTopicBlock = (CTopicBlock *)(pTopicCell->pItem);
        if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
        {
            RetVal = pTopicBlock->DeleteExclusiveSubscriber();
        }
    }
    else
    {
        if(pTopicName != NULL)
        {
            BOOL Ret = FALSE;
            PTR_LIST_CELL pIter = NULL;
            CTopicBlock * pTopicBlock = NULL;
            
            Ret = m_TopicList.ResetIter(&pIter);
            
            if(Ret == TRUE)
            {
                while(m_TopicList.IterRead(&pIter, (void * *)(&pTopicBlock)) == TRUE)
                {
                    if(strcmp(pTopicBlock->GetTopicNamePtr(), pTopicName) == 0)
                    {
                        RetVal = pTopicBlock->DeleteExclusiveSubscriber();
                        
                        break;
                    }
                }
            }
        }
    }
    
    return RetVal;
}

