
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
 Description: ʱ���ַ���תʱ���
       Input: pcTimestr��ʱ���ַ���[2018-08-15 17:07:08]
      Output: 
      Return: ʱ���
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
 Description: ��ȡ��ǰʱ��
       Input: 
      Output: 
      Return: ʱ���
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
 Description: ��ȡ�����
       Input: iRange���������Χ
      Output: 
      Return: �����
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

	 //�������������
	srand((unsigned int)(time(&ts)));
	iRes = (int)(rand() % iRange);

	return iRes;
}

/********************************************************
   Func Name: getUUID
Date Created: 2018-7-21
 Description: ��ȡһ��uuid
       Input: 
      Output: 
      Return: ����һ��uuid�ַ���
     Caution: �ڴ���Ҫ�������ͷţ�uuid��36���ֽڣ�
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

	//����uuid
	uuid_generate(uuid);
	//ת�����ַ���
	uuid_unparse(uuid, pcRes);

	return pcRes;
}

/********************************************************
   Func Name: getMonotonicTime
Date Created: 2018-9-3
 Description: ��ȡ����ʱ��
       Input: 
      Output: 
      Return: ʱ���
     Caution: 
*********************************************************/
time_t getMonotonicTime()
{
	struct timespec ts;

	if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts))
	{
		return 0;
	}

	return ts.tv_sec;
}


