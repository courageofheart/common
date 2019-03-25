
#ifndef __GTC_DLINK_H_
#define __GTC_DLINK_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
//ptr--节点指针类型。例如gtc_dlink_node_t *
//type --数据类型  例如STStudent 这里是类型，不是变量
//member --成员属性
#define LIST_ENTRY(ptr, type, member) __extension__ ({\
const __typeof__(((type *)0)->member) *__nptr = (ptr);\
NULL == __nptr ? NULL : container_of(__nptr, type, member);\
})

#endif

/*
链表的遍历
a_list  gtc_dlink_t指针
pos  用户自定义结构指针(用户允许操作pos变量)
node 用户自定义结构指针(用户禁止操作node变量)
member 成员属性
*/
#ifndef GTC_DLINK_FOREACH
#define GTC_DLINK_FOREACH(a_list, pos, node, member) \
for(pos = LIST_ENTRY(a_list->front, __typeof__(*pos), member), node = LIST_ENTRY(pos->member.next, __typeof__(*pos), member); \
pos != NULL; \
pos = node, node = (node ? LIST_ENTRY(node->member.next, __typeof__(*pos), member):NULL))
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct gtc_dlink_node
	{
		struct gtc_dlink_node *prev;
		struct gtc_dlink_node *next;
	}gtc_dlink_node_t;

	typedef struct
	{
		gtc_dlink_node_t *front;
		gtc_dlink_node_t *back;
		int cnt;
	}gtc_dlink_t;

#define DLINK_FRONT(a_list, type, member) LIST_ENTRY(a_list->front, type, member)

#define DLINK_BACK(a_list, type, member) LIST_ENTRY(a_list->back, type, member)

#define DLINK_PREV(ptr, type, member)({\
const __typeof__(((type *)0)->member) *__nptr = (ptr);\
NULL == __nptr ? NULL : LIST_ENTRY(__nptr->prev, type, member);\
})

#define DLINK_NEXT(ptr, type, member)({\
const __typeof__(((type *)0)->member) *__nptr = (ptr);\
NULL == __nptr ? NULL : LIST_ENTRY(__nptr->next, type, member);\
})

#define DLINK_LENGTH(a_list) (a_list->cnt)

	//初始化链表
	static inline gtc_dlink_t *gtc_dlink_new()
	{
		gtc_dlink_t *a_list = NULL;

		a_list = (gtc_dlink_t *)calloc(1, sizeof(gtc_dlink_t));
		if (!a_list)
		{
			return NULL;
		}

		a_list->cnt = 0;
		a_list->front = NULL;
		a_list->back = NULL;

		return a_list;
	}

	//释放链表
	static inline void gtc_dlink_del(gtc_dlink_t * a_list)
	{
		assert(a_list);

		a_list->cnt = 0;
		a_list->front = NULL;
		a_list->back = NULL;

		free(a_list);
		a_list = NULL;
	}

	//尾部插入元素
	static inline  void gtc_dlink_push_back(gtc_dlink_t * a_list, gtc_dlink_node_t *p_node)
	{
		assert(a_list);
		assert(p_node);

		if (!a_list->cnt)
		{
			a_list->front = p_node;
			a_list->back = p_node;

			a_list->cnt++;

			return;
		}

		a_list->cnt++;

		p_node->prev = a_list->back;
		p_node->next = NULL;

		a_list->back->next = p_node;
		a_list->back = p_node;
	}

	//头部插入元素
	static inline void gtc_dlink_push_front(gtc_dlink_t * a_list, gtc_dlink_node_t *p_node)
	{
		assert(a_list);
		assert(p_node);

		if (!a_list->cnt)
		{
			a_list->front = p_node;
			a_list->back = p_node;

			a_list->cnt++;

			return;
		}

		a_list->cnt++;

		p_node->prev = NULL;
		p_node->next = a_list->front;
		a_list->front->prev = p_node;
		a_list->front = p_node;
	}

	//释放一个元素
	static inline void gtc_dlink_erase(gtc_dlink_t * a_list, gtc_dlink_node_t *p_node)
	{
		assert(a_list);
		assert(p_node);

		gtc_dlink_node_t * prevNode = NULL;
		gtc_dlink_node_t * nextNode = NULL;

		if (a_list->cnt)
		{
			a_list->cnt--;
		}
		prevNode = p_node->prev;
		nextNode = p_node->next;

		if (prevNode)
		{
			prevNode->next = nextNode;
		}
		if (nextNode)
		{
			nextNode->prev = prevNode;
		}
	}

	//尾部弹出一个元素
	static inline void gtc_dlink_pop_back(gtc_dlink_t * a_list)
	{
		assert(a_list);

		gtc_dlink_node_t * prevNode = NULL;

		if (a_list->cnt)
		{
			a_list->cnt--;
		}

		if (a_list->back)
		{
			prevNode = a_list->back->prev;
			if (prevNode)
			{
				prevNode->next = NULL;
			}
		}
	}

	//头部弹出一个元素
	static inline void gtc_dlink_pop_front(gtc_dlink_t * a_list)
	{
		assert(a_list);

		gtc_dlink_node_t * nextNode = NULL;

		if (a_list->cnt)
		{
			a_list->cnt--;
		}

		if (a_list->front)
		{
			nextNode = a_list->front->next;
			if (nextNode)
			{
				nextNode->prev = NULL;
			}
		}
	}

#ifdef __cplusplus
}
#endif

#endif
