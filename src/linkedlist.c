
#include "linkedlist.h"

StListNode * List_First(StListHead *pstHead)
{
	StListNode *pstCurNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	pstCurNode = pstHead->pstFirstNode;

	return pstCurNode;
}

void List_PushBack(StListHead *pstHead, StListNode *pstNode)
{
	StListNode *pstCurNode = NULL;

	if (NULL == pstHead || NULL == pstNode)
	{
		return;
	}
	pstCurNode = pstHead->pstFirstNode;
	if (NULL == pstCurNode)
	{
		pstHead->pstFirstNode = pstNode;
		return;
	}

	while (pstCurNode->pstNext)
	{
		pstCurNode = pstCurNode->pstNext;
	}

	pstCurNode->pstNext = pstNode;
}

StListNode * List_PopFront(StListHead *pstHead)
{
	StListNode *pstCurNode = NULL;
	StListNode *pstNextNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	pstCurNode = pstHead->pstFirstNode;
	if (NULL == pstCurNode)
	{
		return NULL;
	}

	pstNextNode = pstCurNode->pstNext;
	pstHead->pstFirstNode = pstNextNode;

	return pstCurNode;
}

