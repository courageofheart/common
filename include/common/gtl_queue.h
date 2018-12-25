
#ifndef __GTL_QUEUE_H_
#define __GTL_QUEUE_H_

#include "common/gtl_list.h"

typedef STGTListNode STGTQueueNode;
typedef STGTListHead STGTQueueHead;

#ifdef __cplusplus
extern "C"
{
#endif

	static inline STGTQueueHead * GTQueue_Init()
	{
		STGTQueueHead * pstHead = NULL;

		pstHead = GTList_Head_Init();

		return pstHead;
	}

	static inline STGTQueueNode * GTQueue_Front(STGTQueueHead *pstHead)
	{
		STGTQueueNode *node = NULL;

		if (NULL == pstHead)
		{
			return NULL;
		}

		node = GTList_First(pstHead);

		return node;
	}

	static inline void GTQueue_Push(STGTQueueHead *pstHead, STGTQueueNode *pstNode)
	{
		if (NULL == pstHead || NULL == pstNode)
		{
			return;
		}
		GTList_PushBack(pstHead, pstNode);
	}

	static inline STGTQueueNode * GTQueue_Pop(STGTQueueHead *pstHead)
	{
		if (NULL == pstHead)
		{
			return;
		}
		return GTList_PopFront(pstHead);
	}

	static inline int GTQueue_Empty(STGTQueueHead *pstHead)
	{
		if (NULL == pstHead)
		{
			return 1;
		}
		return GTList__Empty(pstHead);
	}

	static inline int GTQueue_Size(STGTQueueHead *pstHead)
	{
		if (NULL == pstHead)
		{
			return 0;
		}
		return GTList__Size(pstHead);
	}

#ifdef __cplusplus
}
#endif


#endif




