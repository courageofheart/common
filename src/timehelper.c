
#include "timehelper.h"
#include "uuid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 1024
#define UUID_LEN 37

/********************************************************
   Func Name: timestrToNumber
Date Created: 2018-8-7
 Description: 时间字符串转时间戳
       Input: pcTimestr：时间字符串[2018-08-15 17:07:08]
      Output: 
      Return: 时间戳
     Caution: 
*********************************************************/
time_t timestrToNumber(const char *pcTimestr)
{
	time_t t = 0;
	FILE *pfs = NULL;
	char gcCmd[DEFAULT_SIZE] = {0};
	char gcBuf[DEFAULT_SIZE] = {0};

	if (NULL == pcTimestr)
	{
		return t;
	}

	sprintf(gcCmd,"date -d \"%s\" +%%s",pcTimestr);

	pfs = popen(gcCmd,"r");
	if (NULL == pfs)
	{
		return t;
	}
	if (fgets(gcBuf, 1024, pfs))
	{
		t = atol(gcBuf);
	}

	if (pfs)
	{
		pclose(pfs);
		pfs = NULL;
	}
	
	return t;
}

/********************************************************
   Func Name: getCurrentTime
Date Created: 2018-8-16
 Description: 获取当前时间
       Input: 
      Output: 
      Return: 时间戳
     Caution: 
*********************************************************/
time_t getCurrentTime()
{
	time_t ts;

	time(&ts);

	return ts;
}

/********************************************************
   Func Name: getRandomNumber
Date Created: 2018-8-24
 Description: 获取随机数
       Input: iRange：随机数范围
      Output: 
      Return: 随机数
     Caution: 
*********************************************************/
int getRandomNumber(int iRange)
{
	time_t ts = 0;
	int iRes = 0;

	if (0 >= iRange)
	{
		return 0;
	}

	 //生成随机数种子
	srand((unsigned int)(time(&ts)));
	iRes = (int)(rand() % iRange);

	return iRes;
}

/********************************************************
   Func Name: getUUID
Date Created: 2018-7-21
 Description: 获取一个uuid
       Input: 
      Output: 
      Return: 返回一个uuid字符串
     Caution: 内存需要由外面释放（uuid有36个字节）
*********************************************************/
char * getUUID()
{
	uuid_t uuid;
	char *pcRes = NULL;

	pcRes = (char *)malloc(UUID_LEN);
	if (NULL == pcRes)
	{
		return NULL;
	}
	memset(pcRes, 0, UUID_LEN);

	//创建uuid
	uuid_generate(uuid);
	//转化成字符串
	uuid_unparse(uuid, pcRes);

	return pcRes;
}

