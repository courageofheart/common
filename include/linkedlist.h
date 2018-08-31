
#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include <stdlib.h>

#define offsetof(type, member) ((size_t)&((type *)0)->member)

#define container_of(ptr, type, member) ({\
const typeof(((type *)0)->member) *__mptr = (ptr);\
(type *)((char *)__mptr - offsetof(type,member));})

// "const typeof(((type *)0)->member) *__mptr = (ptr);"
//������ʹ��ʱ����Ĳ��������⣺ptr��member���Ͳ�ƥ�䣬����ʱ�����warnning

typedef struct tagStListNode
{
	struct tagStListNode *pstNext;
}StListNode;

typedef struct tagStListHead
{
	StListNode *pstFirstNode;
}StListHead;

#define LIST_ENTRY(ptr, type, member) \
container_of(ptr, type, member)


#ifdef __cplusplus
extern "C"
{
#endif
	StListNode * List_First(StListHead *pstHead);

	void List_PushBack(StListHead *pstHead, StListNode *pstNode);

	StListNode * List_PopFront(StListHead *pstHead);
	
#define FOREACH(pstHead, pstEntry, member) \
for(pstEntry = LIST_ENTRY(List_First(pstHead), typeof(*pstEntry), member); \
pstEntry != NULL; \
pstEntry = (pstEntry->member.pstNext)? LIST_ENTRY(pstEntry->member.pstNext, typeof(*pstEntry), member) : NULL)


#ifdef __cplusplus
}
#endif

#endif


