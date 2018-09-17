
#ifdef TEST

#include "gtl.h"
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_aes.h"
#include "doublelink.h"
#include "gtlqueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct tagSTStudent
{
	int age;
	STQueueNode stNode;
}STStudent;

void test()
{
	int i = 0;

	//头结点
	STQueue *head = queue_init();
	STStudent arr[5];
	STStudent * pstStu = NULL;
	arr[0].age = 2;
	arr[1].age = 3;
	arr[2].age = 4;
	arr[3].age = 5;
	arr[4].age = 6;
	queue_push(head, &arr[0].stNode);
	queue_push(head, &arr[1].stNode);
	queue_push(head, &arr[2].stNode);
	queue_push(head, &arr[3].stNode);
	queue_push(head, &arr[4].stNode);

	//遍历
	for (i = 0;i<5;i++)
	{
		pstStu = LIST_ENTRY(queue_pop(head), STStudent, stNode);
		printf("age is %d\n", pstStu->age);
	}

	DLink_Release(&head);

}

int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
