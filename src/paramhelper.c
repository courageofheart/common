
#include "common/paramhelper.h"
#include "common/stringhelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>


/********************************************************
   Func Name: parseLongOption
Date Created: 2018-8-24
 Description: ��ѡ���������
	   Input:    pcSrc��Դ����
	          pcOption��ѡ��
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
	//ȥ��--
	removeCharacter(pcOption, '-');

	return 0;
}

/********************************************************
   Func Name: cmdParamParse
Date Created: 2018-8-24
 Description: �����в�������
	   Input:    argc��main()�в���1
	             argv��main()�в���2
	      pcOptString����ѡ���ַ���
        pstLongOption����ѡ���ַ���
      Output: 
      Return: error code
     Caution: not thread safe
*********************************************************/
STGTListHead *parseParam(int argc, char ** argv, const char *pcOptString, const stLongOption *pstLongOption)
{
	STOptionNode *pstNode = NULL;
	STGTListHead *pstHead = NULL;
	int iOption = 0;
	int optionIndex = 0;

	if (0 == argc || NULL == argv || NULL == pcOptString || NULL == pstLongOption)
	{
		return NULL;
	}

	pstHead = (STGTListHead *)malloc(sizeof(STGTListHead));
	if (NULL == pstHead)
	{
		return NULL;
	}
	memset(pstHead, 0, sizeof(STGTListHead));

	while (iOption = getopt_long(argc, argv, pcOptString, pstLongOption, &optionIndex), iOption != -1)
	{
		if ('?' == iOption)
		{
			//����ѡ����洢
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
			//��ѡ��
			parseLongOption(argv[optind - 1], pstNode->gcOption);
		}
		else
		{
			//��ѡ��
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
		GTList_PushBack(pstHead, &pstNode->stNode);
	}

	return pstHead;
}

/********************************************************
   Func Name: releaseSource
Date Created: 2018-8-24
 Description: �ͷ���Դ
	   Input: pstHead��ͷ���
      Output: 
      Return: error code
     Caution: not thread safe
*********************************************************/
void releaseSource(STGTListHead *pstHead)
{
	STOptionNode *pstOptionNode = NULL;

	if (NULL == pstHead)
	{
		return;
	}

	while ((pstOptionNode = LIST_ENTRY(GTList_PopFront(pstHead),STOptionNode,stNode)))
	{
		free(pstHead);
		pstHead = NULL;
	}
}


