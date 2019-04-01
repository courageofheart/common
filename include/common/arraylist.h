
#ifndef __GTC_ARRAYLIST_H_
#define __GTC_ARRAYLIST_H_

#define ARRAY_LIST_DEFAULT_CAPACITY 32

#ifndef GTC_MAX
#define GTC_MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef GTC_MIN
#define GTC_MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* 动态数组 */

#ifdef __cplusplus
extern "C"
{
#endif
	//清理回调函数
	typedef void(*gtc_arraylist_del_fn) (void *data);

	typedef struct {
		void **array;
		uint32_t used;          //已用元素个数
		uint32_t capacity;      //容量(单位：元素) 并非数组大小
		gtc_arraylist_del_fn del;
	} gtc_arraylist_t;

	//初始化数组
	static inline  gtc_arraylist_t *gtc_arraylist_new(gtc_arraylist_del_fn del)
	{
		gtc_arraylist_t *a_list = NULL;

		a_list = (gtc_arraylist_t *)calloc(1, sizeof(gtc_arraylist_t));
		if (!a_list)
		{
			return NULL;
		}

		a_list->capacity = ARRAY_LIST_DEFAULT_CAPACITY;
		a_list->used = 0;

		/* this could be NULL */
		a_list->del = del;
		a_list->array = (void **)calloc(a_list->capacity, sizeof(void *));
		if (!a_list->array)
		{
			free(a_list);
			a_list = NULL;
		}

		return a_list;
	}

	//释放数组
	static inline void gtc_arraylist_del(gtc_arraylist_t * a_list)
	{
		assert(a_list);

		uint32_t i = 0;

		if (a_list->del && a_list->array)
		{
			for (i = 0; i < a_list->used; i++)
			{
				if (a_list->array[i])
				{
					a_list->del(a_list->array[i]);
					a_list->array[i] = NULL;
				}
			}
		}

		if (a_list->array)
		{
			free(a_list->array);
			a_list->array = NULL;
		}

		a_list->capacity = 0;
		a_list->used = 0;

		free(a_list);
		a_list = NULL;
	}

	//尾部插入
	static inline int gtc_arraylist_push_back(gtc_arraylist_t * a_list, void *data)
	{
		assert(a_list);

		if (a_list->capacity - 1 < a_list->used + 1)
		{
			if (arraylist_expand_inner(a_list, a_list->capacity + 1))
			{
				return -1;
			}
		}

		a_list->array[a_list->used++] = data;

		return 0;
	}

	//下标赋值
	static inline int gtc_arraylist_set(gtc_arraylist_t * a_list, uint32_t idx, void *data)
	{
		assert(a_list);
		assert(idx <= a_list->used);

		//清除旧数据
		if (a_list->array[idx] && a_list->del)
		{
			a_list->del(a_list->array[idx]);
		}

		a_list->array[idx] = data;

		return 0;
	}

#define GTC_ARRAYLIST_LENGTH(a_list) (a_list->used)

#define GTC_ARRAYLIST_GET(a_list, i) \
	 ((i >= a_list->used) ? NULL : a_list->array[i])

#define GTC_ARRAYLIST_FOREACH(a_list, i, a_unit) \
	for(i = 0, a_unit = a_list->array[0]; (i < a_list->used) && (a_unit = a_list->array[i], 1) ; i++)

#ifdef __cplusplus
}
#endif

#endif
