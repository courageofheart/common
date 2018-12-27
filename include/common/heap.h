
#ifndef __HEAP_H_
#define __HEAP_H_

/* 堆结构 */

#ifdef __cplusplus
extern "C"
{

#endif

	typedef struct tagSTHeapHead
	{
		void **pstRoot;                 //堆数据数组
		int size;                       //结构大小
		int used;                       //已经使用的空间
	}head_t;

	typedef int(*CompareFunc)(void *, void *);

	typedef void(*ReleaseFunc)(void *);

	//重新分配内存空间
	int heap_reserve(head_t *pstHead, int iSize);

	//堆压入一个元素
	int heap_push(head_t *pstHead, void *pvData, CompareFunc func);

	//判断堆是否为空
	int head_empty(head_t *pstHead);

	//堆的释放
	int head_release(head_t *pstHead, ReleaseFunc func);

	//获取堆顶元素
	void *heap_top(head_t *pstHead);

	//弹出堆顶元素
	void *heap_pop(head_t *pstHead, CompareFunc func);

	//堆的清空
	int head_clear(head_t *pstHead);

	//typedef void(*ShowFunc)(void *);

	//void test_show(head_t *pstHead, ShowFunc func);


#ifdef __cplusplus
}
#endif 

#endif



