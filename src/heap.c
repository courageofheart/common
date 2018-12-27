

#include <stdlib.h>
#include <string.h>

#include "common/heap.h"

/********************************************************
   Func Name: heap_shift_up
Date Created: 2018-9-3
 Description: 节点上移
	   Input:   pstHead：头结点
	              index：插入下标
		         pvData：数据
				   func：比较函数
	  Output:   
      Return: 
     Caution: 
*********************************************************/
static void heap_shift_up(head_t *pstHead, int index, void *pvData, CompareFunc func)
{
	int sonIndex = index;
	//节点的索引从0开始，所以是(sonIndex-1)/2;
	int parentIndex = (sonIndex - 1) / 2;

	//func(pvData, pstHead->pstRoot[parentIndex]) --- 需要插入的节点和父节点进行比较
	//sonIndex ---因为需要跟根节点进行比较，所以不能用parentIndex
	while (sonIndex && func(pvData, pstHead->pstRoot[parentIndex]))
	{
		//父节点开始上移，此时空下一个位置
		pstHead->pstRoot[sonIndex] = pstHead->pstRoot[parentIndex];
		sonIndex = parentIndex;
		parentIndex = (sonIndex - 1) / 2;
	}

	pstHead->pstRoot[sonIndex] = pvData;
}

/********************************************************
   Func Name: heap_shift_down
Date Created: 2018-9-3
 Description: 节点下移
	   Input:   pstHead：头结点
	              index：删除下标
				   func：比较函数
	  Output:   
      Return: 
     Caution: 
*********************************************************/
static void heap_shift_down(head_t *pstHead, int index, CompareFunc func)
{
	int parentIndex = index;
	//右孩子节点下标
	int sonIndex = (parentIndex + 1) * 2;

	while (sonIndex <= pstHead->used)
	{
		//sonIndex == pstHead->used 判断是否有右孩子
		//func(pstHead->pstRoot[sonIndex-1],pstHead->pstRoot[sonIndex]) --- 比较左孩子和右孩子谁的数值大
		sonIndex -= (sonIndex == pstHead->used) || func(pstHead->pstRoot[sonIndex - 1], pstHead->pstRoot[sonIndex]);
		//跟父节点数据交换
		pstHead->pstRoot[parentIndex] = pstHead->pstRoot[sonIndex];
		//数据清除
		pstHead->pstRoot[sonIndex] = NULL;
		parentIndex = sonIndex;
		sonIndex = (parentIndex + 1) * 2;
	}

	//parentIndex < pstHead->used - 1 如果 最右最下的节点 已经被处理过了，就不需要再进行处理了
	//也可以写成 NULL != pstHead->pstRoot[pstHead->used - 1] 说明最后一个节点数据有效
	if (parentIndex < pstHead->used - 1)
	{
		//将最右最下的节点插到空缺处
		heap_shift_up(pstHead, parentIndex, pstHead->pstRoot[pstHead->used - 1], func);
		pstHead->pstRoot[pstHead->used - 1] = NULL;
	}
	
}

/********************************************************
   Func Name: heap_reserve
Date Created: 2018-9-3
 Description: 重新分配内存空间
	   Input:   pstHead：头结点
	              iSize：需要分配的内存空间
	  Output:   
      Return: error code
     Caution: 
*********************************************************/
int heap_reserve(head_t *pstHead,int iSize)
{
	int new_size = 0;
	void **pstTemp = NULL;

	if (NULL == pstHead)
	{
		return -1;
	}
	if (pstHead->size < iSize)
	{
		new_size = pstHead->size ? pstHead->size * 2 : 8;
		if (new_size < iSize)
		{
			new_size = iSize;
		}
		pstTemp = (void **)realloc(pstHead->pstRoot, new_size * sizeof(void *));
		if (NULL == pstTemp)
		{
			return -1;
		}
		memset(pstTemp + pstHead->size, 0, (new_size - pstHead->size) * sizeof(void *));
		pstHead->pstRoot = pstTemp;
		pstHead->size = new_size;
	}
	return 0;
}

/********************************************************
   Func Name: heap_push
Date Created: 2018-9-3
 Description: 堆压入一个元素
	   Input:   pstHead：头结点
	             pvData：数据
				   func：比较函数
	  Output:   
      Return: error code
     Caution: 
*********************************************************/
int heap_push(head_t *pstHead, void *pvData, CompareFunc func)
{
	if (NULL == pstHead || NULL == func)
	{
		return -1;
	}

	if (heap_reserve(pstHead, pstHead->used+1))
	{
		return -1;
	}

	heap_shift_up(pstHead, pstHead->used, pvData, func);
	pstHead->used++;

	return 0;
}

/********************************************************
   Func Name: head_empty
Date Created: 2018-9-3
 Description: 判断堆是否为空
	   Input:   pstHead：头结点
	  Output:   
      Return: 非0表示空，0表示非空
     Caution: 
*********************************************************/
int head_empty(head_t *pstHead)
{
	if (NULL == pstHead)
	{
		return 1;
	}

	return 0 == pstHead->used;
}

/********************************************************
   Func Name: head_release
Date Created: 2018-9-3
 Description: 堆的释放
	   Input:   pstHead：头结点
	               func：释放函数指针
	  Output:   
      Return: error code
     Caution: 会释放堆结构中的内存数据
*********************************************************/
int head_release(head_t *pstHead, ReleaseFunc func)
{
	int i = 0;

	if (NULL == pstHead || NULL == func)
	{
		return -1;
	}

	if (0 == pstHead->used)
	{
		return 0;
	}

	for (; i < pstHead->used; i++)
	{
		func(pstHead->pstRoot[i]);
		pstHead->pstRoot[i] = NULL;
	}

	free(pstHead->pstRoot);
	pstHead->pstRoot = NULL;

	pstHead->size = 0;
	pstHead->used = 0;

	return 0;
}

/********************************************************
   Func Name: head_release
Date Created: 2018-9-3
 Description: 堆的清空
	   Input:   pstHead：头结点
	  Output:   
      Return: error code
     Caution: 不会释放堆结构中的内存数据
*********************************************************/
int head_clear(head_t *pstHead)
{
	if (NULL == pstHead)
	{
		return -1;
	}

	if (pstHead->used)
	{
		free(pstHead->pstRoot);
		pstHead->pstRoot = NULL;

		pstHead->size = 0;
		pstHead->used = 0;
	}

	return 0;
}

/********************************************************
   Func Name: heap_top
Date Created: 2018-9-3
 Description: 获取堆顶元素
	   Input:   pstHead：头结点
	  Output:   
      Return: 堆顶元素
     Caution: 
*********************************************************/
void *heap_top(head_t *pstHead)
{
	void * pvRes = NULL;

	if (NULL == pstHead)
	{
		return NULL;
	}

	if (pstHead->used)
	{
		pvRes = *pstHead->pstRoot;
	}

	return pvRes;
}

/********************************************************
   Func Name: heap_pop
Date Created: 2018-9-3
 Description: 弹出堆顶元素
	   Input:   pstHead：头结点
	  Output:   
      Return: 堆顶元素
     Caution: 会在堆结构中清除堆顶元素，堆顶元素的内存由外部函数释放
*********************************************************/
void *heap_pop(head_t *pstHead, CompareFunc func)
{
	void * pvRes = NULL;

	if (NULL == pstHead || NULL == func)
	{
		return NULL;
	}

	if (pstHead->used)
	{
		pvRes = *pstHead->pstRoot;
		heap_shift_down(pstHead, 0, func);
		pstHead->used--;
	}

	return pvRes;
}

//void test_show(head_t *pstHead, ShowFunc func)
//{
//	int i = 0;
//	for (;i < pstHead->used; i++)
//	{
//		func(pstHead->pstRoot[i]);
//	}
//}


