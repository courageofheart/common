//
//#ifndef __GTL_QUEUE_H_
//#define __GTL_QUEUE_H_
//
//#include "common/dlink.h"
//
//typedef gtc_dlink_node_t gtc_queue_node_t;
//typedef gtc_dlink_t gtc_queue_t;
//
//#ifdef __cplusplus
//extern "C"
//{
//#endif
//
//	static inline gtc_queue_t * gtc_queue_new()
//	{
//		return gtc_dlink_new();
//	}
//
//	static inline gtc_queue_node_t * gtc_queue_front(gtc_queue_t *a_list)
//	{
//		return 
//	}
//
//	static inline void GTQueue_Push(STGTQueueHead *pstHead, STGTQueueNode *pstNode)
//	{
//		if (NULL == pstHead || NULL == pstNode)
//		{
//			return;
//		}
//		GTList_PushBack(pstHead, pstNode);
//	}
//
//	static inline STGTQueueNode * GTQueue_Pop(STGTQueueHead *pstHead)
//	{
//		if (NULL == pstHead)
//		{
//			return;
//		}
//		return GTList_PopFront(pstHead);
//	}
//
//	static inline int GTQueue_Empty(STGTQueueHead *pstHead)
//	{
//		if (NULL == pstHead)
//		{
//			return 1;
//		}
//		return GTList__Empty(pstHead);
//	}
//
//	static inline int GTQueue_Size(STGTQueueHead *pstHead)
//	{
//		if (NULL == pstHead)
//		{
//			return 0;
//		}
//		return GTList__Size(pstHead);
//	}
//
//#ifdef __cplusplus
//}
//#endif
//
//
//#endif
//
//
//
//
