
#ifdef TEST

#include "common/paramhelper.h"
#include "common/stringhelper.h"
#include "common/crypto_des.h"
#include "common/timehelper.h"
#include "common/crypto_aes.h"
#include "common/crypto_sha.h"
#include "common/regularhelper.h"
#include "common/base64.h"
#include "common/gtl_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct tagSTStudent
{
	STGTListNode stNode;
	int age;
	int no;
}STStudent;

void test()
{
	int i = 0;
	time_t t_start, t_end;
	STGTListHead * head = NULL;
	STStudent *pstStu = NULL;

	time(&t_start);

	head = GTList_Head_Init();

	for (i = 0; i < 1024 * 1024 * 200; i++)
	{
		STStudent *pstu = malloc(sizeof(STStudent));
		pstu->no = i;
		pstu->age = i / 100;
		GTList_PushBack(head, &pstu->stNode);
	}

	while (!GTList__Empty(head))
	{
		pstStu = LIST_ENTRY(GTList_PopEnd(head), STStudent, stNode);
		if (pstStu)
		{
			free(pstStu);
			pstStu = NULL;
		}
	}
	time(&t_end);

	printf("start[%ld]--------end[%ld] . \n", t_start, t_end);
	printf("ok . \n");

}
int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
