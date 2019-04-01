
#include "common/gfifo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/********************************************************
   Func Name: gtc_fifo_roundup
Date Created: 2019-4-1
 Description: 扩展
	   Input: 
	  Output: 
	  Return: size
	 Caution: 
*********************************************************/
static unsigned int gtc_fifo_roundup(unsigned int x)
{
	unsigned int i = 0;
	unsigned int y = 1;

	if (!x)
	{
		return 1;
	}

	for (i = x; i != 0; )
	{
		i >>= 1;
		y <<= 1;
	}

	return y;

}

/********************************************************
   Func Name: gfifo_init
Date Created: 2019-4-1
 Description: 初始化
	   Input: 
	  Output: 
	  Return: 
	 Caution: 
*********************************************************/
gtc_fifo_t * gtc_fifo_new(unsigned int size, gtc_fifo_del_fn del)
{
	gtc_fifo_t * fifo = NULL;

	fifo = (gtc_fifo_t *)calloc(1, sizeof(gtc_fifo_t));
	if (!fifo)
	{
		return NULL;
	}

	/*
		size的值总是在调用者传进来的size参数的基础上向2的幂扩展，这是内核一贯的做法。
	这样的好处不言而喻--对kfifo->size取模运算可以转化为与运算，如下：
	fifo->in % fifo->size 可以转化为 fifo->in & (fifo->size – 1)
	在kfifo_alloc函数中，使用size & (size – 1)来判断size 是否为2幂，如果条件为真，则表示size不是2的幂，然后调用roundup_pow_of_two将之向上扩展为2的幂。
	*/
	size *= sizeof(void *);
	if (size & (size - 1))
	{
		size = gtc_fifo_roundup(size);
	}

	fifo->capacity = size / sizeof(void *);
	/* this could be NULL */
	fifo->del = del;
	fifo->in = 0;
	fifo->out = 0;
	fifo->size = 0;

	fifo->array = (void **)calloc(fifo->capacity, sizeof(void *));
	if (!fifo->array)
	{
		free(fifo);
		fifo = NULL;
	}

	return fifo;
}

/********************************************************
   Func Name: gtc_fifo_del
Date Created: 2019-4-1
 Description: 释放
	   Input: 
	  Output: 
	  Return: 
	 Caution: 
*********************************************************/
void gtc_fifo_del(gtc_fifo_t * fifo)
{
	if (!fifo)
	{
		return;
	}

	unsigned int i = 0;

	if (fifo->del && fifo->array)
	{
		for (i = 0; i < fifo->size; i++)
		{
			if (fifo->array[i])
			{
				fifo->del(fifo->array[i]);
				fifo->array[i] = NULL;
			}
		}
	}

	if (fifo->array)
	{
		free(fifo->array);
		fifo->array = NULL;
	}

	fifo->capacity = 0;
	fifo->del = NULL;
	fifo->in = 0;
	fifo->out = 0;
	fifo->size = 0;

	free(fifo);
	fifo = NULL;
}

/********************************************************
   Func Name: gtc_fifo_put
Date Created: 2019-4-1
 Description: 入队
	   Input: 
	  Output: 
	  Return: error code
	 Caution: 
*********************************************************/
int gtc_fifo_put(gtc_fifo_t *fifo, void * data)
{
	assert(fifo);

	const unsigned int element_size = sizeof(void *);
	unsigned int rest_in = 0;
	const unsigned int fifo_size = fifo->capacity * element_size;
	unsigned int pos = 0;
	/*
	计算出实际写入队列数据大小
		(fifo->in - fifo->out)  已经使用空间大小
		fifo->capacity - (fifo->in - fifo->out)  可以使用空间大小
		len  需要写入数据大小
	*/
	if (fifo_size - (fifo->in - fifo->out) < element_size)
	{
		return -1;
	}

	//计算出在队列in后面插入数据的大小
	rest_in = fifo_size - (fifo->in & (fifo_size - 1));

	if (0 == rest_in)
	{
		//数据插入队列头部
		pos = 0;
		//pos = fifo->array[0];
	}
	else
	{
		//确保in后面可以插入一个元素(而不能是半个)
		assert(rest_in >= element_size);

		//拷贝数据到in后面
		pos = (fifo->in & (fifo_size - 1)) / element_size;// (unsigned char *)fifo->array + );
	}

	fifo->array[pos] = data;

	//更新in
	fifo->in += element_size;
	fifo->size++;

	return 0;
}

/********************************************************
   Func Name: gtc_fifo_pop
Date Created: 2019-4-1
 Description: 出队
	   Input: 
	  Output: 
	  Return: 出队元素
	 Caution: 
*********************************************************/
int gtc_fifo_pop(gtc_fifo_t *fifo, void ** data)
{
	assert(fifo);
	assert(data);

	const unsigned int element_size = sizeof(void *);
	unsigned int rest_out = 0;
	const unsigned int fifo_size = fifo->capacity * element_size;
	unsigned int pos = 0;

	//可读数据小于一个元素
	if (fifo->in - fifo->out < element_size)
	{
		return -1;
	}

	/*
	计算出在队列out后面插入数据的大小
		fifo->in & (fifo->capacity - 1) 等同于 fifo->in % fifo->capacity
		fifo->capacity - (fifo->out & (fifo->capacity - 1))  表示out后面可读数据的长度
	*/
	rest_out = fifo_size - (fifo->out & (fifo_size - 1));

	if (0 == rest_out)
	{
		//拷贝能从头部获取的数据
		pos = 0;
		//*data = fifo->array[0];
	}
	else
	{
		//直接从out后面拷贝一个元素
		assert(rest_out >= element_size);
		pos = (fifo->out & (fifo_size - 1)) / element_size;
		//*data = (unsigned char *)fifo->array + (fifo->out & (fifo_size - 1));
	}
	*data = fifo->array[pos];

	//更新out
	fifo->out += element_size;
	fifo->size--;

	return 0;
}

/********************************************************
   Func Name: gtc_fifo_pop
Date Created: 2019-4-1
 Description: 出队
	   Input: 
	  Output: 
	  Return: 出队元素
	 Caution: 
*********************************************************/
const void * gtc_fifo_front(gtc_fifo_t *fifo)
{
	assert(fifo);

	const unsigned int element_size = sizeof(void *);
	unsigned int rest_out = 0;
	unsigned int pos = 0;
	const unsigned int fifo_size = fifo->capacity * element_size;

	//可读数据小于一个元素
	if (fifo->in - fifo->out < element_size)
	{
		return NULL;
	}

	rest_out = fifo_size - (fifo->out & (fifo_size - 1));

	if (0 == rest_out)
	{
		pos = 0;
	}
	else
	{
		//直接从out后面拷贝一个元素
		assert(rest_out >= element_size);
		pos = (fifo->out & (fifo_size - 1)) / element_size;
	}

	return fifo->array[pos];
}
