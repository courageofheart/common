
#ifndef __GTL_LIST_H_
#define __GTL_LIST_H_

#include <stdlib.h>
#include <string.h>

/* ˫������ */

/*
��ISO C�����е�ͷ�ļ���ʹ��typeof�ؼ��֣���Ҫʹ��__typeof__����typeof��

gcc��ANSI C��׼��������չ��ʹ����Щ��չʱ�����������׳����档ʹ��__extension__�ؼ��ָ���gcc��Ҫ�׳�����

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
//������ʹ��ʱ����Ĳ��������⣺ptr��member���Ͳ�ƥ�䣬����ʱ�����warnning

#ifndef LIST_ENTRY
//ptr--�ڵ�ָ�����͡�����STGTListNode *
//type --��������  ����STStudent ���������ͣ����Ǳ���
//member --��Ա����
#define LIST_ENTRY(ptr, type, member) __extension__ ({\
const __typeof__(((type *)0)->member) *__nptr = (ptr);\
NULL == __nptr ? NULL : container_of(__nptr, type, member);\
})

#endif

//GTList_FOREACH ������ɾ���ڵ�
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
	//�ڵ��ʼ��
	static inline void GTList_Node_Init(STGTListNode *pstNode)
	{
		if (pstNode)
		{
			pstNode->pstPrev = NULL;
			pstNode->pstNext = NULL;
		}
	}

	//��ȡͷ���
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

	//�ͷ�˫������
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

	//��ȡ��Ԫ��
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

	//��ȡβԪ��
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

	//β�����
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
			//��ǰ�б�Ϊ��
			pstHead->pstFirstNode = pstNode;
			pstHead->pstLastNode = pstNode;
			return;
		}

		//˫������β�����룬�������
		pstNode->pstPrev = pstHead->pstLastNode;
		pstHead->pstLastNode->pstNext = pstNode;
		pstHead->pstLastNode = pstNode;
	}

	//ͷ�����
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
			//��ǰ�б�Ϊ��
			pstHead->pstFirstNode = pstNode;
			pstHead->pstLastNode = pstNode;
			return;
		}

		//list not null
		pstHead->pstFirstNode = pstNode;
		pstNode->pstNext = pstCurNode;
		pstCurNode->pstPrev = pstNode;
	}

	//��Ԫ�ص���
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

	//ĩβԪ�ص���
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

	//�Ƴ�Ԫ��
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

	//��ȡǰһ��Ԫ��
	static inline STGTListNode * GTList_Prev(STGTListNode *pstNode)
	{
		if (pstNode)
		{
			return pstNode->pstPrev;
		}
		return NULL;
	}

	//��ȡ��һ��Ԫ��
	static inline STGTListNode * GTList_Next(STGTListNode *pstNode)
	{
		if (pstNode)
		{
			return pstNode->pstNext;
		}
		return NULL;
	}

	//�Ƿ�Ϊ�գ��շ���1���ǿշ���0��
	static inline int GTList__Empty(STGTListHead *pstHead)
	{
		if (NULL == pstHead)
		{
			return 1;
		}

		return 0 == pstHead->size;
	}

	//��ȡ��ǰԪ�ظ���
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
