
#include "common/gtl_list.h"

#include <stdlib.h>
#include <string.h>

StListHead * Link_Head_Init()
{
	StListHead * pstHead = NULL;
	pstHead = (StListHead *)malloc(sizeof(StListHead));
	if (NULL == pstHead)
	{
		return NULL;
	}
	memset(pstHead, 0, sizeof(StListHead));

	return pstHead;
}

void Link_Release(StListHead **pstHead)
{
	StListHead * pstTemp = NULL;
	if (NULL == pstHead)
	{
		return ;
	}
	pstTemp = *pstHead;
	memset(pstTemp, 0, sizeof(StListHead));
	free(pstTemp);
	pstTemp = NULL;
	*pstHead = NULL;
}

StListNode * Link_First(StListHead *pstHead)
{
	StListNode *pstCurNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	pstCurNode = pstHead->pstFirstNode;

	return pstCurNode;
}

void Link_PushBack(StListHead *pstHead, StListNode *pstNode)
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

StListNode * Link_PopFront(StListHead *pstHead)
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

