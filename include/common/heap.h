
#ifndef __HEAP_H_
#define __HEAP_H_

#include <stdint.h>

/* 堆结构 */

/*
int (*CompareFunc)(void *left, void *right);：回调函数说明

如果left>right，返回true，表示最大堆
如果left<right，返回true，表示最小堆
最大堆还是最小堆可以看heap_shift_down函数的实现

*/

#ifdef __cplusplus
extern "C"
{

#endif

	typedef struct tagSTHeapHead
	{
		void **pstRoot;                 //堆数据数组
		int size;                       //结构大小
		int used;                       //已经使用的空间（本质上是下一个节点的下标，类似于stl中的end()方法）
	}STHeapHead;

	typedef int(*CompareFunc)(void *, void *);

	typedef void(*ReleaseFunc)(void *);

	//初始化
	STHeapHead * heap_init(uint32_t iSize);

	//重新分配内存空间
	int heap_reserve(STHeapHead *pstHead, uint32_t iSize);

	//堆压入一个元素
	int heap_push(STHeapHead *pstHead, void *pvData, CompareFunc func);

	//判断堆是否为空
	int heap_empty(STHeapHead *pstHead);

	//堆的释放
	int heap_release(STHeapHead *pstHead, ReleaseFunc func);

	//获取堆顶元素
	void *heap_top(STHeapHead *pstHead);

	//弹出堆顶元素
	void *heap_pop(STHeapHead *pstHead, CompareFunc func);

	//堆的清空
	int heap_clear(STHeapHead *pstHead);

	typedef void(*doFunc)(void *);
	//堆的遍历
	void heap_foreach(STHeapHead *pstHead, doFunc cb);


#ifdef __cplusplus
}
#endif 

#endif



