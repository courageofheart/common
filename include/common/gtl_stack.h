//
//#ifndef __GTL_STACK_H_
//#define __GTL_STACK_H_
//
//#include "common/gtl_list.h"
//
//#include <stdlib.h>
//
//typedef STGTListNode STGTStackNode;
//typedef STGTListHead STGTStackHead;
//
//#ifdef __cplusplus
//extern "C"
//{
//#endif
//	static inline STGTStackHead * GTStack_Init()
//	{
//		STGTStackHead * pstHead = NULL;
//
//		pstHead = GTList_Head_Init();
//
//		return pstHead;
//	}
//
//	static inline void GTStack_Push(STGTStackHead *pstHead, STGTStackNode *pstNode)
//	{
//		if (NULL == pstHead || NULL == pstNode)
//		{
//			return;
//		}
//		GTList_PushBack(pstHead, pstNode);
//	}
//
//	static inline STGTStackNode * GTStack_Pop(STGTStackHead *pstHead)
//	{
//		if (NULL == pstHead)
//		{
//			return NULL;
//		}
//		return GTList_PopEnd(pstHead);
//	}
//
//	static inline STGTStackNode * GTStack_Top(STGTStackHead *pstHead)
//	{
//		STGTStackNode *node = NULL;
//
//		if (NULL == pstHead)
//		{
//			return NULL;
//		}
//
//		node = GTList_First(pstHead);
//
//		return node;
//	}
//
//	static inline int GTStack_Empty(STGTStackHead *pstHead)
//	{
//		if (NULL == pstHead)
//		{
//			return 1;
//		}
//		return GTList__Empty(pstHead);
//	}
//
//	static inline int GTStack_Size(STGTStackHead *pstHead)
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
//#endif
//
//
//
//
//
//
//
