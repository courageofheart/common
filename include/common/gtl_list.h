
#ifndef __GTL_LIST_H_
#define __GTL_LIST_H_

#include <stdlib.h>
#include <string.h>

/* 双向链表 */

/*
在ISO C程序中的头文件中使用typeof关键字，需要使用__typeof__代替typeof。

gcc对ANSI C标准进行了扩展，使用这些扩展时，编译器会抛出警告。使用__extension__关键字告诉gcc不要抛出警告

*/

#ifndef offsetof
#define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member) __extension__ ({\
const __typeof__(((type *)0)->member) *__mptr = (ptr);\
(type *)((char *)__mptr - offsetof(type,member));})
#endif

// "const __typeof__(((type *)0)->member) *__mptr = (ptr);"
//开发者使用时输入的参数有问题：ptr与member类型不匹配，编译时便会有warnning

#ifndef LIST_ENTRY
//ptr--节点指针类型。例如STGTListNode *
//type --数据类型  例如STStudent 这里是类型，不是变量
//member --成员属性
#define LIST_ENTRY(ptr, type, member) __extension__ ({\
const __typeof__(((type *)0)->member) *__nptr = (ptr);\
NULL == __nptr ? NULL : container_of(__nptr, type, member);\
})

#endif

//GTList_FOREACH 不允许删除节点
#ifndef GTList_FOREACH
#define GTList_FOREACH(pstHead, pstEntry, member) \
for(pstEntry = LIST_ENTRY(GTList_First(pstHead), __typeof__(*pstEntry), member); \
pstEntry != NULL; \
pstEntry = (pstEntry->member.pstNext)? LIST_ENTRY(pstEntry->member.pstNext, __typeof__(*pstEntry), member) : NULL)
#endif

typedef struct tagSTGTListNode
{
	struct tagSTGTListNode *pstPrev;
	struct tagSTGTListNode *pstNext;
}STGTListNode;

typedef struct tagSTGTListHead
{
	STGTListNode *pstFirstNode;
	STGTListNode *pstLastNode;
	int size;
}STGTListHead;

#ifdef __cplusplus
extern "C"
{
#endif
	//节点初始化
	static inline void GTList_Node_Init(STGTListNode *pstNode)
	{
		if (pstNode)
		{
			pstNode->pstPrev = NULL;
			pstNode->pstNext = NULL;
		}
	}

	//获取头结点
	static inline STGTListHead * GTList_Head_Init()
	{
		STGTListHead * pstHead = NULL;
		pstHead = (STGTListHead *)malloc(sizeof(STGTListHead));
		if (NULL == pstHead)
		{
			return NULL;
		}
		memset(pstHead, 0, sizeof(STGTListHead));

		return pstHead;
	}

	//释放双向链表
	static inline void GTList_Release(STGTListHead **pstHead)
	{
		STGTListHead * pstTemp = NULL;
		if (NULL == pstHead)
		{
			return;
		}
		pstTemp = *pstHead;
		memset(pstTemp, 0, sizeof(STGTListHead));
		free(pstTemp);
		pstTemp = NULL;
		*pstHead = NULL;
	}

	//获取首元素
	static inline STGTListNode * GTList_First(STGTListHead *pstHead)
	{
		STGTListNode *pstCurNode = NULL;

		if (NULL == pstHead)
		{
			return NULL;
		}

		pstCurNode = pstHead->pstFirstNode;

		return pstCurNode;
	}

	//获取尾元素
	static inline STGTListNode * GTList_Back(STGTListHead *pstHead)
	{
		STGTListNode *pstCurNode = NULL;

		if (NULL == pstHead)
		{
			return NULL;
		}

		pstCurNode = pstHead->pstLastNode;

		return pstCurNode;
	}

	//尾部添加
	static inline void GTList_PushBack(STGTListHead *pstHead, STGTListNode *pstNode)
	{
		STGTListNode *pstCurNode = NULL;

		if (NULL == pstHead || NULL == pstNode)
		{
			return;
		}

		GTList_Node_Init(pstNode);
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

	//头部添加
	static inline void GTList_PushFront(STGTListHead *pstHead, STGTListNode *pstNode)
	{
		STGTListNode *pstCurNode = NULL;

		if (NULL == pstHead || NULL == pstNode)
		{
			return;
		}

		GTList_Node_Init(pstNode);
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

	//首元素弹出
	static inline STGTListNode * GTList_PopFront(STGTListHead *pstHead)
	{
		STGTListNode *pstCurNode = NULL;

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
		}
		else
		{
			//more element
			pstHead->pstFirstNode = pstCurNode->pstNext;
			pstHead->pstFirstNode->pstPrev = NULL;
		}

		return pstCurNode;
	}

	//末尾元素弹出
	static inline STGTListNode * GTList_PopEnd(STGTListHead *pstHead)
	{
		STGTListNode *pstCurNode = NULL;

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

	//移除元素
	static inline void GTList_Remove(STGTListHead *pstHead, STGTListNode *pstNode)
	{
		STGTListNode *pstPrevNode = NULL;
		STGTListNode *pstNextNode = NULL;

		if (NULL == pstHead || NULL == pstNode)
		{
			return;
		}

		pstHead->size -= pstHead->size > 0 ? 1 : 0;

		//remove head
		if (pstHead->pstFirstNode == pstNode)
		{
			GTList_PopFront(pstHead);
			return;
		}

		//remove tail
		if (pstHead->pstLastNode == pstNode)
		{
			GTList_PopEnd(pstHead);
			return;
		}

		pstPrevNode = pstNode->pstPrev;
		pstNextNode = pstNode->pstNext;
		pstPrevNode->pstNext = pstNextNode;
		pstNextNode->pstPrev = pstPrevNode;
	}

	//获取前一个元素
	static inline STGTListNode * GTList_Prev(STGTListNode *pstNode)
	{
		if (pstNode)
		{
			return pstNode->pstPrev;
		}
		return NULL;
	}

	//获取后一个元素
	static inline STGTListNode * GTList_Next(STGTListNode *pstNode)
	{
		if (pstNode)
		{
			return pstNode->pstNext;
		}
		return NULL;
	}

	//是否为空（空返回1，非空返回0）
	static inline int GTList__Empty(STGTListHead *pstHead)
	{
		if (NULL == pstHead)
		{
			return 1;
		}

		return 0 == pstHead->size;
	}

	//获取当前元素个数
	static inline int GTList__Size(STGTListHead *pstHead)
	{
		if (NULL == pstHead)
		{
			return 0;
		}

		return pstHead->size;
	}

#ifdef __cplusplus
}
#endif

#endif
