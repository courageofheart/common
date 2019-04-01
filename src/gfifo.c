
#include "common/gfifo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/********************************************************
   Func Name: gtc_fifo_roundup
Date Created: 2019-4-1
 Description: ��չ
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
 Description: ��ʼ��
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
		size��ֵ�����ڵ����ߴ�������size�����Ļ�������2������չ�������ں�һ���������
	�����ĺô����Զ���--��kfifo->sizeȡģ�������ת��Ϊ�����㣬���£�
	fifo->in % fifo->size ����ת��Ϊ fifo->in & (fifo->size �C 1)
	��kfifo_alloc�����У�ʹ��size & (size �C 1)���ж�size �Ƿ�Ϊ2�ݣ��������Ϊ�棬���ʾsize����2���ݣ�Ȼ�����roundup_pow_of_two��֮������չΪ2���ݡ�
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
 Description: �ͷ�
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
 Description: ���
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
	�����ʵ��д��������ݴ�С
		(fifo->in - fifo->out)  �Ѿ�ʹ�ÿռ��С
		fifo->capacity - (fifo->in - fifo->out)  ����ʹ�ÿռ��С
		len  ��Ҫд�����ݴ�С
	*/
	if (fifo_size - (fifo->in - fifo->out) < element_size)
	{
		return -1;
	}

	//������ڶ���in����������ݵĴ�С
	rest_in = fifo_size - (fifo->in & (fifo_size - 1));

	if (0 == rest_in)
	{
		//���ݲ������ͷ��
		pos = 0;
		//pos = fifo->array[0];
	}
	else
	{
		//ȷ��in������Բ���һ��Ԫ��(�������ǰ��)
		assert(rest_in >= element_size);

		//�������ݵ�in����
		pos = (fifo->in & (fifo_size - 1)) / element_size;// (unsigned char *)fifo->array + );
	}

	fifo->array[pos] = data;

	//����in
	fifo->in += element_size;
	fifo->size++;

	return 0;
}

/********************************************************
   Func Name: gtc_fifo_pop
Date Created: 2019-4-1
 Description: ����
	   Input: 
	  Output: 
	  Return: ����Ԫ��
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

	//�ɶ�����С��һ��Ԫ��
	if (fifo->in - fifo->out < element_size)
	{
		return -1;
	}

	/*
	������ڶ���out����������ݵĴ�С
		fifo->in & (fifo->capacity - 1) ��ͬ�� fifo->in % fifo->capacity
		fifo->capacity - (fifo->out & (fifo->capacity - 1))  ��ʾout����ɶ����ݵĳ���
	*/
	rest_out = fifo_size - (fifo->out & (fifo_size - 1));

	if (0 == rest_out)
	{
		//�����ܴ�ͷ����ȡ������
		pos = 0;
		//*data = fifo->array[0];
	}
	else
	{
		//ֱ�Ӵ�out���濽��һ��Ԫ��
		assert(rest_out >= element_size);
		pos = (fifo->out & (fifo_size - 1)) / element_size;
		//*data = (unsigned char *)fifo->array + (fifo->out & (fifo_size - 1));
	}
	*data = fifo->array[pos];

	//����out
	fifo->out += element_size;
	fifo->size--;

	return 0;
}

/********************************************************
   Func Name: gtc_fifo_pop
Date Created: 2019-4-1
 Description: ����
	   Input: 
	  Output: 
	  Return: ����Ԫ��
	 Caution: 
*********************************************************/
const void * gtc_fifo_front(gtc_fifo_t *fifo)
{
	assert(fifo);

	const unsigned int element_size = sizeof(void *);
	unsigned int rest_out = 0;
	unsigned int pos = 0;
	const unsigned int fifo_size = fifo->capacity * element_size;

	//�ɶ�����С��һ��Ԫ��
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
		//ֱ�Ӵ�out���濽��һ��Ԫ��
		assert(rest_out >= element_size);
		pos = (fifo->out & (fifo_size - 1)) / element_size;
	}

	return fifo->array[pos];
}
