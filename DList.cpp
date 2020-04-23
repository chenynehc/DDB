
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
CDList::CDList(void)
{
	::InitializeCriticalSection(&m_CS);
    
	Init();
}

CDList::~CDList(void)
{
	Init();
    
    ::DeleteCriticalSection(&m_CS);
}

void CDList::Init(void)
{
	m_sList.pHead = NULL;
	m_sList.pTail = NULL;
	m_sList.uwCount = 0;
}

BOOL CDList::Free(void)
{
    BOOL RetVal = FALSE;
    
    __try
    {
        ::EnterCriticalSection(&m_CS);
        
        while(m_sList.pTail != NULL)
        {
            PTR_LIST_CELL pCell = m_sList.pTail;
            
            if(m_sList.pTail->pPrev != NULL)
            {
                m_sList.pTail = pCell->pPrev;
                delete[] pCell->pItem;
                delete pCell;
            }
            else
            {
                delete[] pCell->pItem;
                delete pCell;
                
                m_sList.pTail = NULL;
            }
        }
        
        m_sList.pHead = NULL;
        m_sList.uwCount = 0;
        
        RetVal = TRUE;
    }
    __finally
    {
        ::LeaveCriticalSection(&m_CS);
    }
    
    return RetVal;
}

PTR_LIST_CELL CDList::Insert(void * pItem)
{
	PTR_LIST_CELL pCell = NULL;
    
    if(pItem != NULL)
    {
        pCell = new LIST_CELL;
        
        if(pCell != NULL)
        {
            pCell->pItem = pItem;
            
            __try
            {
                ::EnterCriticalSection(&m_CS);
                
                if(m_sList.uwCount == 0)
                {
                    m_sList.pHead = pCell;
                    m_sList.pTail = pCell;
                    pCell->pPrev = NULL;
                }
                else
                {
                    m_sList.pTail->pNext = pCell;
                    pCell->pPrev = m_sList.pTail;
                    m_sList.pTail = pCell;
                }
                
                pCell->pNext = NULL;
                
                m_sList.uwCount++;
            }
            __finally
            {
                ::LeaveCriticalSection(&m_CS);
            }
            
        }
    }
	
	return pCell;
}

BOOL CDList::ReadOut(void * * ppItem)
{
	BOOL RetVal = FALSE;
	
	if(ppItem != NULL)
	{
		__try
        {
            ::EnterCriticalSection(&m_CS);
            
            if(m_sList.uwCount > 0)
            {
                PTR_LIST_CELL pCell = m_sList.pHead;
                * ppItem = pCell->pItem;
                
                if(m_sList.uwCount == 1)
                {
                    m_sList.pHead = NULL;
                    m_sList.pTail = NULL;
                }
                else
                {				
                    pCell->pNext->pPrev = NULL;
                    m_sList.pHead = pCell->pNext;
                }
                
                delete pCell;
                
                m_sList.uwCount--;
                
                RetVal = TRUE;
            }
            else
            {
                RetVal = FALSE;
            }
        }
        __finally
        {
            ::LeaveCriticalSection(&m_CS);
        }
	}
	else
	{
		RetVal = FALSE;
	}
	
	return RetVal;
}

BOOL CDList::RemoveCell(PTR_LIST_CELL pIter)
{
    BOOL RetVal = FALSE;
    
    if(pIter != NULL)
    {
        __try
        {
            ::EnterCriticalSection(&m_CS);
        
            if(pIter->pPrev != NULL)
            {
                pIter->pPrev->pNext = pIter->pNext;
            }
            else
            {
                if(m_sList.pHead == pIter)
                {
                    m_sList.pHead = pIter->pNext;
                }
                else
                {
                    //impossible
                }
            }
            
            if(pIter->pNext != NULL)
            {
                pIter->pNext->pPrev = pIter->pPrev;
            }
            else
            {
                if(m_sList.pTail == pIter)
                {
                    m_sList.pTail = pIter->pPrev;
                }
                else
                {
                    //impossible
                }
            }
            
            delete[] pIter->pItem;
            delete pIter;
            
            RetVal = TRUE;
        }
        __finally
        {
            ::LeaveCriticalSection(&m_CS);
        }
    }
    
    return RetVal;
}

BOOL CDList::IterRead(PTR_LIST_CELL * ppIter, void * * ppItem)
{
	BOOL RetVal = FALSE;
	
	if((ppIter != NULL) && (* ppIter != NULL) && (ppItem != NULL))
	{		
		__try
        {
            ::EnterCriticalSection(&m_CS);
        
            * ppItem = (* ppIter)->pItem;
            
            * ppIter = (* ppIter)->pNext;
            
            RetVal = TRUE;
        }
        __finally
        {
            ::LeaveCriticalSection(&m_CS);
        }
	}
	else
	{
		RetVal = FALSE;
	}
	
	return RetVal;
}

BOOL CDList::StepIter(PTR_LIST_CELL * ppIter)
{
    BOOL RetVal = FALSE;
	
	if((ppIter != NULL) && (* ppIter != NULL))
	{		
		__try
        {
            ::EnterCriticalSection(&m_CS);
        
            * ppIter = (* ppIter)->pNext;
            
            RetVal = TRUE;
        }
        __finally
        {
            ::LeaveCriticalSection(&m_CS);
        }
	}
	else
	{
		RetVal = FALSE;
	}
	
	return RetVal;
}

BOOL CDList::ResetIter(PTR_LIST_CELL * ppIter)
{
    BOOL RetVal = FALSE;
    
    if(ppIter != NULL)
	{		
		__try
        {
            ::EnterCriticalSection(&m_CS);
        
            * ppIter = m_sList.pHead;
            
            RetVal = TRUE;
        }
        __finally
        {
            ::LeaveCriticalSection(&m_CS);
        }
	}
	else
	{
		RetVal = FALSE;
	}
    
    return RetVal;
}

BOOL CDList::GoEndIter(PTR_LIST_CELL * ppIter)
{
    BOOL RetVal = FALSE;
    
    if(ppIter != NULL)
	{		
		__try
        {
            ::EnterCriticalSection(&m_CS);
        
            * ppIter = m_sList.pTail;
            
            RetVal = TRUE;
        }
        __finally
        {
            ::LeaveCriticalSection(&m_CS);
        }
	}
	else
	{
		RetVal = FALSE;
	}
    
    return RetVal;
}

BOOL CDList::IsHead(PTR_LIST_CELL pIter)
{
    return ((m_sList.pHead == pIter) ? TRUE : FALSE);
}

BOOL CDList::IsTail(PTR_LIST_CELL pIter)
{
    return ((m_sList.pTail == pIter) ? TRUE : FALSE);
}

UWORD CDList::GetCount(void)
{
	return m_sList.uwCount;
}


