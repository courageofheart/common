
#include "paramhelper.h"
#include "stringhelper.h"

#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>


/********************************************************
   Func Name: parseLongOption
Date Created: 2018-8-24
 Description: 长选项参数解析
	   Input:    pcSrc：源数据
	          pcOption：选项
      Output: 
      Return: error code
     Caution: not thread safe
*********************************************************/
static int parseLongOption(char *pcSrc, char *pcOption)
{
	char *pcIndex = NULL;
	char *pcBuf = NULL;

	if (NULL == pcSrc || NULL == pcOption)
	{
		return -1;
	}

	pcIndex = pcSrc;
	pcBuf = strtok(pcIndex, "=");
	strcpy(pcOption, pcBuf);
	//去除--
	removeCharacter(pcOption, '-');

	return 0;
}

/********************************************************
   Func Name: cmdParamParse
Date Created: 2018-8-24
 Description: 命令行参数解析
	   Input:    argc：main()中参数1
	             argv：main()中参数2
	      pcOptString：短选项字符串
        pstLongOption：长选项字符串
      Output: 
      Return: error code
     Caution: not thread safe
*********************************************************/
StListHead *parseParam(int argc, char ** argv, const char *pcOptString, const stLongOption *pstLongOption)
{
	STOptionNode *pstNode = NULL;
	StListHead *pstHead = NULL;
	int iOption = 0;
	int optionIndex = 0;

	if (0 == argc || NULL == argv || NULL == pcOptString || NULL == pstLongOption)
	{
		return NULL;
	}

	pstHead = (StListHead *)malloc(sizeof(StListHead));
	if (NULL == pstHead)
	{
		return NULL;
	}
	memset(pstHead, 0, sizeof(StListHead));

	while (iOption = getopt_long(argc, argv, pcOptString, pstLongOption, &optionIndex), iOption != -1)
	{
		if ('?' == iOption)
		{
			//错误选项不做存储
			continue;
		}

		pstNode = (STOptionNode *)malloc(sizeof(STOptionNode));
		if (NULL == pstNode)
		{
			return pstHead;
		}
		memset(pstNode, 0, sizeof(STOptionNode));

		if (0 == iOption)
		{
			//长选项
			parseLongOption(argv[optind - 1], pstNode->gcOption);
		}
		else
		{
			//短选项
			pstNode->gcOption[0] = iOption;
		}
		if (optarg)
		{
			if (strlen(optarg) >= 256)
			{
				strncpy(pstNode->gcValue, optarg, 256);
			}
			else
			{
				strcpy(pstNode->gcValue, optarg);
			}
		}
		Link_PushBack(pstHead, &pstNode->stNode);
	}

	return pstHead;
}

/********************************************************
   Func Name: releaseSource
Date Created: 2018-8-24
 Description: 释放资源
	   Input: pstHead：头结点
      Output: 
      Return: error code
     Caution: not thread safe
*********************************************************/
void releaseSource(StListHead *pstHead)
{
	STOptionNode *pstOptionNode = NULL;

	if (NULL == pstHead)
	{
		return;
	}

	while ((pstOptionNode = LIST_ENTRY(Link_PopFront(pstHead),STOptionNode,stNode)))
	{
		free(pstHead);
		pstHead = NULL;
	}
}


