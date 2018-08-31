
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

unsigned char g_SecretKey[8] = {'f','g','1','2','e','t','3','4'};

void test_param(int argc,char * argv[])
{
	StListHead *pstHead = NULL;
	STOptionNode *pstOptionNode = NULL;
	const char *pcOptString = "v:h";
	stLongOption long_options[] = {
		{ "prefix", required_argument, NULL, 0 },
		{ "help", no_argument, NULL, 0 },
		{ NULL, 0, NULL, 0 }
	};

	pstHead = parseParam(argc, argv, pcOptString, long_options);
	if (NULL == pstHead)
	{
		printf("parseParam() failed .\n");
		return;
	}
	FOREACH(pstHead, pstOptionNode, stNode)
	{
		printf("option=%s and value is %s \n", pstOptionNode->gcOption, pstOptionNode->gcValue);
	}
	releaseSource(pstHead);
}

void test_string()
{
	char gcBuf[] = "                 sdfsdf   sdfsd          ";
	//trim((char *)gcBuf);
	//printf("trim = %s\n", gcBuf);
	//ltrim((char *)gcBuf);
	//printf("ltrim = %s\n", gcBuf);
	rtrim((char *)gcBuf);
	printf("rtrim = %s\n", gcBuf);
}

void test_des()
{
	int iRet = 0;
	char *pSrc = "asdfads124123431";
	char *pcOut = NULL;
	int iOutLen = 0;
	char *pcRes = NULL;
	int iResLen = 0;

	iRet = des_encode(g_SecretKey,pSrc,strlen(pSrc),&pcOut,&iOutLen);
	if (0 != iRet)
	{
		printf("des_encode() failed .\n");
		return;
	}
	iRet = des_decode(g_SecretKey, pcOut, iOutLen, &pcRes, &iResLen);
	if (0 != iRet)
	{
		printf("des_decode() failed .\n");
		return;
	}

	printf("des_decode = %s \n",pcRes);
}

int main(int argc,char * argv[])
{
	//test_param(argc,argv);
	//test_string();
	test_des();
	return 0;
}

