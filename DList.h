#ifndef _DLIST_H_
#define _DLIST_H_

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
typedef struct tagListCell
{
	void * pItem;
	struct tagListCell * pPrev;
	struct tagListCell * pNext;
	
}LIST_CELL, * PTR_LIST_CELL;

typedef struct tagList
{
	PTR_LIST_CELL pHead;
	PTR_LIST_CELL pTail;
	UWORD uwCount;
	
}LIST, * PTR_LIST;

//--------------------------------------------------------------------------//
//                                                                          //
// CLASS DEFINE                                                             //
//                                                                          //
//--------------------------------------------------------------------------//
class CDList
{
public:
    CDList(void);
	
	virtual ~CDList(void);

public:
	void Init(void);

    BOOL Free(void);
	
    PTR_LIST_CELL Insert(void * pItem);
	
    BOOL ReadOut(void * * ppItem);
    
    BOOL RemoveCell(PTR_LIST_CELL pIter);
    
    BOOL IterRead(PTR_LIST_CELL * ppIter, void * * ppItem);
    
    BOOL StepIter(PTR_LIST_CELL * ppIter);
    
    BOOL ResetIter(PTR_LIST_CELL * ppIter);
    
    BOOL GoEndIter(PTR_LIST_CELL * ppIter);
    
    BOOL IsHead(PTR_LIST_CELL pIter);
    
    BOOL IsTail(PTR_LIST_CELL pIter);
    
    UWORD GetCount(void);
	
private:
	LIST m_sList;
    
private:
    CRITICAL_SECTION m_CS;

};

#endif//_DLIST_H_



