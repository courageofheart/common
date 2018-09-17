
#include "gtlqueue.h"

#include <stdlib.h>

STQueue * queue_init()
{
	STQueue * pstQueue = NULL;

	pstQueue = DLink_Head_Init();

	return pstQueue;
}

STQueueNode * queue_back(STQueue *pstQueue)
{
	STQueueNode *pstNode = NULL;

	pstNode = DLink_Back(pstQueue);

	return pstNode;
}

STQueueNode * queue_front(STQueue *pstQueue)
{
	STQueueNode *pstNode = NULL;

	pstNode = DLink_First(pstQueue);

	return pstNode;
}

void queue_push(STQueue *pstQueue, STQueueNode *pstNode)
{
	DLink_PushBack(pstQueue, pstNode);
}

STQueueNode * queue_pop(STQueue *pstQueue)
{
	STQueueNode *pstNode = NULL;

	pstNode = DLink_PopFront(pstQueue);

	return pstNode;
}

void queue_release(STQueue **pstQueue)
{
	DLink_Release(pstQueue);
}




