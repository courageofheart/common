
#ifdef TEST

#include "gtl.h"
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_aes.h"
#include "crypto_sha.h"
#include "regularhelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void deal(char *pcData, int *regVector, size_t size, void *userArg)
{
	char gcData[1024] = { 0 };
	int i = 0;

	for (i = 1; i < size; i++)
	{
		//regVector[2 * i]表示开始位置
		//regVector[2 * i + 1] 表示结束位置
		strncpy(gcData, pcData + regVector[2 * i], regVector[2 * i + 1] - regVector[2 * i]);
		printf("key is %s\n", gcData);
	}
	printf("\n", gcData);
}

void test()
{
	char *p = NULL;
	//char str[] = "http://1.203.80.138:8001/tts?user_id=speech&domain=1&language=zh&audiotype=6&rate=1&speed=5&text=asr error goodbye";
	char str[] = "tabceftsfasdft12345t";
	int result = 0;
	
	//memset(&data, 0, sizeof(STParamList));
	int data;

	//提取所有的参数
	result = regularInfer(str, "t(.)(.)(.)(.)(.)t", &data, deal);
	if (result)
	{
		printf("regularInfer() error .\n");
	}
}

int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
