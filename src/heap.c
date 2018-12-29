
#include <stdlib.h>
#include <string.h>

#include "common/heap.h"

/********************************************************
   Func Name: heap_shift_up
Date Created: 2018-9-3
 Description: 新增节点
	   Input:   pstHead：头结点
	              index：插入下标
		         pvData：数据
				   func：比较函数
	  Output:   
      Return: 
     Caution: 新增叶子节点，与根节点比较，调整位置
*********************************************************/
static void heap_shift_up(STHeapHead *pstHead, int index, void *pvData, CompareFunc func)
{
	int sonIndex = index;
	//节点的索引从0开始，所以是(sonIndex-1)/2;
	int parentIndex = (sonIndex - 1) / 2;

	//func(pvData, pstHead->pstRoot[parentIndex]) --- 需要插入的节点和父节点进行比较
	//sonIndex ---因为需要跟根节点进行比较，所以不能用parentIndex
	while (sonIndex && func(pvData, pstHead->pstRoot[parentIndex]))
	{
		//父节点开始下移，此时原父节点处空下一个位置
		pstHead->pstRoot[sonIndex] = pstHead->pstRoot[parentIndex];
		sonIndex = parentIndex;
		parentIndex = (sonIndex - 1) / 2;
	}

	pstHead->pstRoot[sonIndex] = pvData;
}

/********************************************************
   Func Name: heap_shift_down
Date Created: 2018-9-3
 Description: 弹出节点
	   Input:   pstHead：头结点
	              index：删除下标
				   func：比较函数
	  Output:   
      Return: 
     Caution: 根节点弹出，导致左右叶子节点上移
*********************************************************/
static void heap_shift_down(STHeapHead *pstHead, int index, CompareFunc func)
{
	int parentIndex = index;
	//右孩子节点下标
	int sonIndex = (parentIndex + 1) * 2;

	//pstHead->used这个下标处没有任何元素，之所以使用<=是为了确认有没有右孩子节点
	while (sonIndex <= pstHead->used)
	{
		//sonIndex == pstHead->used 表示没有右孩子
		//sonIndex < pstHead->used 表示有右孩子
		//func(pstHead->pstRoot[sonIndex-1],pstHead->pstRoot[sonIndex]) --- 比较左孩子和右孩子谁的数值大
		sonIndex -= (sonIndex == pstHead->used) || func(pstHead->pstRoot[sonIndex - 1], pstHead->pstRoot[sonIndex]);
		//跟父节点数据交换
		pstHead->pstRoot[parentIndex] = pstHead->pstRoot[sonIndex];
		//数据清除
		pstHead->pstRoot[sonIndex] = NULL;
		parentIndex = sonIndex;
		sonIndex = (parentIndex + 1) * 2;
	}

	/*
	     parentIndex < pstHead->used - 1这种情况下，表示最深一层的叶子节点不是以完全二叉树的形态存在，
	假设最深层有4个叶子节点，分别为a,b,c,d(d即为最右最下的叶子节点)，在此场景下，有可能a空余出来，
	也有可能b空余出来，还有可能c空余出来，
	绝对不可能是d空余出来，先考虑d空余出来，d实际上不符合parentIndex < pstHead->used - 1，
	d如果空余出来，应该是parentIndex == pstHead->used - 1
	如果分别考虑a,b,c谁空余出来，比较复杂，不如将d这个元素拿出来，插入到a,b,c谁空余就插到谁那边，
	然后调用heap_shift_up()函数重新调整一下顺序即可，这样又是一个完全二叉树啦

	当前"parentIndex < pstHead->used - 1"也可以写成 NULL != pstHead->pstRoot[pstHead->used - 1] 说明最后一个节点数据有效

	*/
	if (parentIndex < pstHead->used - 1)
	{
		//将最右最下的节点插到空缺处
		heap_shift_up(pstHead, parentIndex, pstHead->pstRoot[pstHead->used - 1], func);
		pstHead->pstRoot[pstHead->used - 1] = NULL;
	}
	
}

/********************************************************
   Func Name: heap_init
Date Created: 2018-9-3
 Description: 初始化堆
	   Input: iSize：需要分配的内存空间
	  Output:   
      Return: 堆头结点
     Caution: 
*********************************************************/
STHeapHead * heap_init(uint32_t iSize)
{
	STHeapHead *head = NULL;
	void **pstTemp = NULL;

	head = malloc(sizeof(STHeapHead));
	if (NULL == head)
	{
		return NULL;
	}
	memset(head, 0, sizeof(STHeapHead));

	head->pstRoot = (void **)malloc(iSize * sizeof(void *));
	if (NULL == head->pstRoot)
	{
		return NULL;
	}
	memset(head->pstRoot, 0, iSize * sizeof(void *));

	head->size = iSize;

	return head;

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
int heap_reserve(STHeapHead *pstHead, uint32_t iSize)
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
int heap_push(STHeapHead *pstHead, void *pvData, CompareFunc func)
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
int heap_empty(STHeapHead *pstHead)
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
int heap_release(STHeapHead *pstHead, ReleaseFunc func)
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
int heap_clear(STHeapHead *pstHead)
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
void *heap_top(STHeapHead *pstHead)
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
void *heap_pop(STHeapHead *pstHead, CompareFunc func)
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

/********************************************************
   Func Name: heap_foreach
Date Created: 2018-12-29
 Description: 堆的遍历
	   Input:   pstHead：头结点
	                 cb：回调函数
	  Output:   
      Return: 
     Caution: 回调函数中不可以删除元素或者释放元素
*********************************************************/
void heap_foreach(STHeapHead *pstHead, doFunc cb)
{
	int i = 0;

	if (NULL == pstHead || NULL == cb)
	{
		return;
	}
	for (i = 0; i < pstHead->used; i++)
	{
		cb(pstHead->pstRoot[i]);
	}

}

