
#include "common/gtl_doublelist.h"

#include <stdlib.h>
#include <string.h>

static void DLink_Node_Init(STDLinkNode *pstNode)
{
	if (pstNode)
	{
		pstNode->pstPrev = NULL;
		pstNode->pstNext = NULL;
	}
}

STDLinkHead * DLink_Head_Init()
{
	STDLinkHead * pstHead = NULL;
	pstHead = (STDLinkHead *)malloc(sizeof(STDLinkHead));
	if (NULL == pstHead)
	{
		return NULL;
	}
	memset(pstHead, 0, sizeof(STDLinkHead));

	return pstHead;
}

void DLink_Release(STDLinkHead **pstHead)
{
	STDLinkHead * pstTemp = NULL;
	if (NULL == pstHead)
	{
		return ;
	}
	pstTemp = *pstHead;
	memset(pstTemp, 0, sizeof(STDLinkHead));
	free(pstTemp);
	pstTemp = NULL;
	*pstHead = NULL;
}

STDLinkNode * DLink_First(STDLinkHead *pstHead)
{
	STDLinkNode *pstCurNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	pstCurNode = pstHead->pstFirstNode;

	return pstCurNode;
}

STDLinkNode * DLink_Back(STDLinkHead *pstHead)
{
	STDLinkNode *pstCurNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	pstCurNode = pstHead->pstLastNode;

	return pstCurNode;
}

void DLink_PushBack(STDLinkHead *pstHead, STDLinkNode *pstNode)
{
	STDLinkNode *pstCurNode = NULL;

	if (NULL == pstHead || NULL == pstNode)
	{
		return;
	}

	DLink_Node_Init(pstNode);
	pstHead->size++;

	pstCurNode = pstHead->pstFirstNode;
	if (NULL == pstCurNode)
	{
		//当前列表为空
		pstHead->pstFirstNode = pstNode;
		pstHead->pstLastNode = pstNode;
		return;
	}

	//双向链表尾部插入，无需遍历
	pstNode->pstPrev = pstHead->pstLastNode;
	pstHead->pstLastNode->pstNext = pstNode;
	pstHead->pstLastNode = pstNode;
}

void DLink_PushFront(STDLinkHead *pstHead, STDLinkNode *pstNode)
{
	STDLinkNode *pstCurNode = NULL;

	if (NULL == pstHead || NULL == pstNode)
	{
		return;
	}

	DLink_Node_Init(pstNode);
	pstHead->size++;

	pstCurNode = pstHead->pstFirstNode;
	if (NULL == pstCurNode)
	{
		//当前列表为空
		pstHead->pstFirstNode = pstNode;
		pstHead->pstLastNode = pstNode;
		return;
	}

	//list not null
	pstHead->pstFirstNode = pstNode;
	pstNode->pstNext = pstCurNode;
	pstCurNode->pstPrev = pstNode;

}

STDLinkNode * DLink_PopFront(STDLinkHead *pstHead)
{
	STDLinkNode *pstCurNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	//empty
	pstCurNode = pstHead->pstFirstNode;
	if (NULL == pstCurNode)
	{
		return NULL;
	}

	pstHead->size -= pstHead->size > 0 ? 1 : 0;
	
	if (pstHead->pstLastNode == pstCurNode)
	{
		//one element
		pstHead->pstLastNode = NULL;
		pstHead->pstFirstNode = NULL;
	}else
	{
		//more element
		pstHead->pstFirstNode = pstCurNode->pstNext;
		pstHead->pstFirstNode->pstPrev = NULL;
	}

	return pstCurNode;

}

STDLinkNode * DLink_PopEnd(STDLinkHead *pstHead)
{
	STDLinkNode *pstCurNode = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	//empty
	pstCurNode = pstHead->pstLastNode;
	if (NULL == pstCurNode)
	{
		return NULL;
	}

	pstHead->size -= pstHead->size > 0 ? 1 : 0;

	if (pstHead->pstFirstNode == pstCurNode)
	{
		//one element
		pstHead->pstFirstNode = NULL;
		pstHead->pstLastNode = NULL;
	}
	else
	{
		//more element
		pstHead->pstLastNode = pstCurNode->pstPrev;
		pstHead->pstLastNode->pstNext = NULL;
	}

	return pstCurNode;

}

void DLink_Remove(STDLinkHead *pstHead, STDLinkNode *pstNode)
{
	STDLinkNode *pstPrevNode = NULL;
	STDLinkNode *pstNextNode = NULL;

	if (NULL == pstHead || NULL == pstNode)
	{
		return;
	}

	pstHead->size -= pstHead->size > 0 ? 1 : 0;

	//remove head
	if (pstHead->pstFirstNode == pstNode)
	{
		DLink_PopFront(pstHead);
		return;
	}

	//remove tail
	if (pstHead->pstLastNode == pstNode)
	{
		DLink_PopEnd(pstHead);
		return;
	}

	pstPrevNode = pstNode->pstPrev;
	pstNextNode = pstNode->pstNext;
	pstPrevNode->pstNext = pstNextNode;
	pstNextNode->pstPrev = pstPrevNode;

}

STDLinkNode * DLink_Prev(STDLinkNode *pstNode)
{
	if (pstNode)
	{
		return pstNode->pstPrev;
	}
	return NULL;
}

STDLinkNode * DLink_Next(STDLinkNode *pstNode)
{
	if (pstNode)
	{
		return pstNode->pstNext;
	}
	return NULL;
}

int DLink_Empty(STDLinkHead *pstHead)
{
	if (pstHead)
	{
		return NULL == pstHead->pstFirstNode ? 0 : 1;
	}

	return 0;
}

int DLink__Size(STDLinkHead *pstHead)
{
	if (NULL == pstHead)
	{
		return 0;
	}

	return pstHead->size;
}

int DLink__Empty(STDLinkHead *pstHead)
{
	if (NULL == pstHead)
	{
		return 0;
	}

	return 0 == pstHead->size;
}







