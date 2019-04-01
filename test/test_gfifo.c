

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/gfifo.h"


struct TestData
{
	int num;
};

void del_fn(void *data)
{
	if (data)
	{
		struct TestData * p = (struct TestData *)data;
		printf("-----[%d]------\n", p->num);
		free(p);
	}
}


void test()
{
	int errorCode = 0;
	int i = 0;
	gtc_fifo_t * fifo = gtc_fifo_new(10000, del_fn);
	struct TestData *data = NULL;

	if (!fifo)
	{
		printf("-gtc_fifo_new() failed .---\n");
		return;
	}

	for (i = 0; i < 1000000; i++)
	{
		data = (struct TestData *)malloc(sizeof(struct TestData));
		data->num = i + 1;

		errorCode = gtc_fifo_put(fifo, data);
		if (errorCode)
		{
			printf("-gtc_fifo_put() failed --[%d].---\n", errorCode);
			break;
		}
	}

	const struct TestData *tmp = (const struct TestData *)gtc_fifo_front(fifo);
	if (tmp)
	{
		printf("---front[%d]---length[%u]--\n", tmp->num, GTC_FIFO_LENGTH(fifo));
	}


	gtc_fifo_del(fifo);
	

}

int main()
{
	test();
	printf("-----ok------\n");
	//getchar();
	return 0;
}


