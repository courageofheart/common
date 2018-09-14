
#ifdef TEST

#include "gtl.h"
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_aes.h"
#include "doublelink.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tagSTStudent
{
	int age;
	STDLinkNode stNode;
}STStudent;

void test()
{
	//头结点
	STDLinkHead *head = DLink_Head_Init();
	STStudent arr[5];
	STStudent * pstStu = NULL;
	arr[0].age = 2;
	arr[1].age = 3;
	arr[2].age = 4;
	arr[3].age = 5;
	arr[4].age = 6;
	DLink_PushBack(head, &arr[0].stNode);
	DLink_PushBack(head, &arr[1].stNode);
	DLink_PushBack(head, &arr[2].stNode);
	DLink_PushBack(head, &arr[3].stNode);
	DLink_PushBack(head, &arr[4].stNode);

	//遍历
	DLINK_FOREACH(head, pstStu, stNode)
	{
		printf("age is %d\n",pstStu->age);
	}

	DLink_Release(&head);

}

int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
