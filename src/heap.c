
#include <stdlib.h>
#include <string.h>

#include "common/heap.h"

/********************************************************
   Func Name: heap_shift_up
Date Created: 2018-9-3
 Description: �����ڵ�
	   Input:   pstHead��ͷ���
	              index�������±�
		         pvData������
				   func���ȽϺ���
	  Output:   
      Return: 
     Caution: ����Ҷ�ӽڵ㣬����ڵ�Ƚϣ�����λ��
*********************************************************/
static void heap_shift_up(STHeapHead *pstHead, int index, void *pvData, CompareFunc func)
{
	int sonIndex = index;
	//�ڵ��������0��ʼ��������(sonIndex-1)/2;
	int parentIndex = (sonIndex - 1) / 2;

	//func(pvData, pstHead->pstRoot[parentIndex]) --- ��Ҫ����Ľڵ�͸��ڵ���бȽ�
	//sonIndex ---��Ϊ��Ҫ�����ڵ���бȽϣ����Բ�����parentIndex
	while (sonIndex && func(pvData, pstHead->pstRoot[parentIndex]))
	{
		//���ڵ㿪ʼ���ƣ���ʱԭ���ڵ㴦����һ��λ��
		pstHead->pstRoot[sonIndex] = pstHead->pstRoot[parentIndex];
		sonIndex = parentIndex;
		parentIndex = (sonIndex - 1) / 2;
	}

	pstHead->pstRoot[sonIndex] = pvData;
}

/********************************************************
   Func Name: heap_shift_down
Date Created: 2018-9-3
 Description: �����ڵ�
	   Input:   pstHead��ͷ���
	              index��ɾ���±�
				   func���ȽϺ���
	  Output:   
      Return: 
     Caution: ���ڵ㵯������������Ҷ�ӽڵ�����
*********************************************************/
static void heap_shift_down(STHeapHead *pstHead, int index, CompareFunc func)
{
	int parentIndex = index;
	//�Һ��ӽڵ��±�
	int sonIndex = (parentIndex + 1) * 2;

	//pstHead->used����±괦û���κ�Ԫ�أ�֮����ʹ��<=��Ϊ��ȷ����û���Һ��ӽڵ�
	while (sonIndex <= pstHead->used)
	{
		//sonIndex == pstHead->used ��ʾû���Һ���
		//sonIndex < pstHead->used ��ʾ���Һ���
		//func(pstHead->pstRoot[sonIndex-1],pstHead->pstRoot[sonIndex]) --- �Ƚ����Ӻ��Һ���˭����ֵ��
		sonIndex -= (sonIndex == pstHead->used) || func(pstHead->pstRoot[sonIndex - 1], pstHead->pstRoot[sonIndex]);
		//�����ڵ����ݽ���
		pstHead->pstRoot[parentIndex] = pstHead->pstRoot[sonIndex];
		//�������
		pstHead->pstRoot[sonIndex] = NULL;
		parentIndex = sonIndex;
		sonIndex = (parentIndex + 1) * 2;
	}

	/*
	     parentIndex < pstHead->used - 1��������£���ʾ����һ���Ҷ�ӽڵ㲻������ȫ����������̬���ڣ�
	�����������4��Ҷ�ӽڵ㣬�ֱ�Ϊa,b,c,d(d��Ϊ�������µ�Ҷ�ӽڵ�)���ڴ˳����£��п���a���������
	Ҳ�п���b������������п���c���������
	���Բ�������d����������ȿ���d���������dʵ���ϲ�����parentIndex < pstHead->used - 1��
	d������������Ӧ����parentIndex == pstHead->used - 1
	����ֱ���a,b,c˭����������Ƚϸ��ӣ����罫d���Ԫ���ó��������뵽a,b,c˭����Ͳ嵽˭�Ǳߣ�
	Ȼ�����heap_shift_up()�������µ���һ��˳�򼴿ɣ���������һ����ȫ��������

	��ǰ"parentIndex < pstHead->used - 1"Ҳ����д�� NULL != pstHead->pstRoot[pstHead->used - 1] ˵�����һ���ڵ�������Ч

	*/
	if (parentIndex < pstHead->used - 1)
	{
		//���������µĽڵ�嵽��ȱ��
		heap_shift_up(pstHead, parentIndex, pstHead->pstRoot[pstHead->used - 1], func);
		pstHead->pstRoot[pstHead->used - 1] = NULL;
	}
	
}

/********************************************************
   Func Name: heap_init
Date Created: 2018-9-3
 Description: ��ʼ����
	   Input: iSize����Ҫ������ڴ�ռ�
	  Output:   
      Return: ��ͷ���
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
 Description: ���·����ڴ�ռ�
	   Input:   pstHead��ͷ���
	              iSize����Ҫ������ڴ�ռ�
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
 Description: ��ѹ��һ��Ԫ��
	   Input:   pstHead��ͷ���
	             pvData������
				   func���ȽϺ���
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
 Description: �ж϶��Ƿ�Ϊ��
	   Input:   pstHead��ͷ���
	  Output:   
      Return: ��0��ʾ�գ�0��ʾ�ǿ�
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
 Description: �ѵ��ͷ�
	   Input:   pstHead��ͷ���
	               func���ͷź���ָ��
	  Output:   
      Return: error code
     Caution: ���ͷŶѽṹ�е��ڴ�����
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
 Description: �ѵ����
	   Input:   pstHead��ͷ���
	  Output:   
      Return: error code
     Caution: �����ͷŶѽṹ�е��ڴ�����
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
 Description: ��ȡ�Ѷ�Ԫ��
	   Input:   pstHead��ͷ���
	  Output:   
      Return: �Ѷ�Ԫ��
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
 Description: �����Ѷ�Ԫ��
	   Input:   pstHead��ͷ���
	  Output:   
      Return: �Ѷ�Ԫ��
     Caution: ���ڶѽṹ������Ѷ�Ԫ�أ��Ѷ�Ԫ�ص��ڴ����ⲿ�����ͷ�
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
 Description: �ѵı���
	   Input:   pstHead��ͷ���
	                 cb���ص�����
	  Output:   
      Return: 
     Caution: �ص������в�����ɾ��Ԫ�ػ����ͷ�Ԫ��
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

