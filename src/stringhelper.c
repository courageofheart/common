
#include "common/stringhelper.h"
#include "common/gtl_stack.h"

#include <stdio.h>
#include <string.h>

typedef struct tagSTCharacter
{
	char ch;
	STGTStackNode stNode;
}STCharacter;

/********************************************************
   Func Name: removeLineCharacter
Date Created: 2017-12-31
 Description: ȥ��һ����ָ���ַ�
       Input: pcBuf: Դ�ַ�����
              cAppoint:��Ҫɾ�����ַ�
      Output: ��
      Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
     Caution: ���޸�Դ�ַ����������,Ҫ��������ַ���
*********************************************************/
int removeLineCharacter(char * pcBuf, char cAppoint)
{
    char *pcDesc = NULL;
    char *pcs = NULL;
    char *pcd = NULL;
	int iGridSize = 0;

    if (NULL == pcBuf)
    {
        return -1;
    }

	iGridSize = strlen(pcBuf);
    pcDesc = (char *)malloc(iGridSize+1);
    if (NULL == pcDesc)
    {
        return -1;
    }
    memset(pcDesc, 0, iGridSize+1);
    pcs = pcBuf;
    pcd = pcDesc;

    while (*pcs != '\n' && *pcs != '\0')
    {
        if (*pcs != cAppoint)
        {
            *pcd = *pcs;
            pcd++;
        }
        pcs++;
    }
	memset(pcBuf, 0, iGridSize);
	strncpy(pcBuf, pcDesc, strlen(pcDesc));

	if (pcDesc)
	{
		free(pcDesc);
		pcDesc = NULL;
	}

    return 0;
}

/********************************************************
   Func Name: trim
Date Created: 2018-8-24
 Description: ȥ����β�ո�
       Input: pcBuf: Դ�ַ�����
      Output: ��
      Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
     Caution: ���޸�Դ�ַ����������,Ҫ��������ַ���
*********************************************************/
int trim(char * pcSrc)
{
	if (ltrim(pcSrc))
	{
		return -1;
	}
	if (rtrim(pcSrc))
	{
		return -1;
	}
	return 0;
}

/********************************************************
   Func Name: ltrim
Date Created: 2018-8-24
 Description: ȥ����߿ո�
       Input: pcBuf: Դ�ַ�����
      Output: ��
      Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
     Caution: ���޸�Դ�ַ����������,Ҫ��������ַ���
*********************************************************/
int ltrim(char * pcSrc)
{
	char *pcIndex = NULL;
	char *pcDest = NULL;
	int iSize = 0;

	if (NULL == pcSrc)
	{
		return -1;
	}

	iSize = strlen(pcSrc);
	pcDest = (char *)malloc(iSize + 1);
	if (NULL == pcDest)
	{
		return -1;
	}
	memset(pcDest, 0, iSize + 1);
	pcIndex = pcSrc;

	//32 �ǿո�� ascii
	while (32 == *pcIndex)
	{
		pcIndex++;
	}
	strcpy(pcDest, pcIndex);
	memset(pcSrc, 0, iSize);
	strcpy(pcSrc, pcDest);

	return 0;
}

/********************************************************
   Func Name: rtrim
Date Created: 2018-8-24
 Description: ȥ���ұ߿ո�
       Input: pcBuf: Դ�ַ�����
      Output: ��
      Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
     Caution: ���޸�Դ�ַ����������,Ҫ��������ַ���
*********************************************************/
int rtrim(char * pcSrc)
{
	char *pcIndex = NULL;
	char *pcDest = NULL;
	int iSize = 0;

	if (NULL == pcSrc)
	{
		return -1;
	}

	iSize = strlen(pcSrc);
	pcDest = (char *)malloc(iSize + 1);
	if (NULL == pcDest)
	{
		return -1;
	}
	memset(pcDest, 0, iSize + 1);
	pcIndex = pcSrc + iSize - 1;

	//32 �ǿո�� ascii
	while (32 == *pcIndex)
	{
		pcIndex--;
	}
	strncpy(pcDest, pcSrc, pcIndex - pcSrc + 1);
	memset(pcSrc, 0, iSize);
	strcpy(pcSrc, pcDest);

	return 0;
}

/********************************************************
   Func Name: removeCharacter
Date Created: 2017-12-31
 Description: ȥ��ָ���ַ�
       Input: pcBuf: Դ�ַ�����
              cAppoint:��Ҫɾ�����ַ�
              uiLen: �ַ�����ĳ���
      Output: ��
      Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
     Caution: ���޸�Դ�ַ����������
*********************************************************/
int removeCharacter(char * pcSrc, char cAppoint)
{
    char *pcDest = NULL;
    char *pcs = NULL;
    char *pcd = NULL;
	int iSize = 0;

    if (NULL == pcSrc)
    {
        return -1;
    }

	iSize = strlen(pcSrc);
    pcDest = (char *)malloc(iSize+1);
    if (NULL == pcDest)
    {
        return -1;
    }
    memset(pcDest, 0, iSize+1);
    pcs = pcSrc;
    pcd = pcDest;

    while ( *pcs != '\0')
    {
        if (*pcs != cAppoint)
        {
            *pcd = *pcs;
            pcd++;
        }
        pcs++;
    }
	memset(pcSrc, 0, iSize);
	strcpy(pcSrc, pcDest);

	if (pcDest)
	{
		free(pcDest);
		pcDest = NULL;
	}
	
    return 0;
}

/********************************************************
   Func Name: releaseGridMemory
Date Created: 2018-7-5
 Description: �ͷ��ַ���ָ������
       Input: pcGrid���ַ��������ָ��
      Output: 
      Return: 
     Caution: ����Ҫ�����һ��Ԫ��ΪNULL
*********************************************************/
void releaseGridMemory(char ***pcGrid)
{
	char ** pcArr = NULL;
	int i = 0;
	if (NULL == pcGrid)
	{
		return;
	}

	pcArr = *pcGrid;
	while(pcArr[i])
	{
		if (pcArr[i])
		{
			free(pcArr[i]);
			pcArr[i] = NULL;
		}
		pcArr[i] = NULL;
		i++;
	}
	if (pcArr)
	{
		free(pcArr);
		pcArr = NULL;
	}
	pcArr = NULL;
	*pcGrid = NULL;
}

/********************************************************
   Func Name: replaceCharacter
Date Created: 2018-7-5
 Description: �滻ָ���ַ�
       Input: pcGrid���ַ��������ָ��
      Output: 
      Return: 
     Caution: ���޸�Դ����
*********************************************************/
void replaceCharacter(char *pcSrc, char cFrom, char cTo)
{
	char *pcIndex = NULL;

	if (NULL == pcSrc)
	{
		return;
	}
	pcIndex = (char *)pcSrc;
	while (*pcIndex)
	{
		if (cFrom == *pcIndex)
		{
			*pcIndex = cTo;
		}
		pcIndex++;
	}
}

/********************************************************
   Func Name: �滻ָ���ַ���
Date Created: 2018-7-5
 Description: �ͷ��ַ���ָ������
       Input: pcGrid���ַ��������ָ��
      Output: 
      Return: 
     Caution: ���޸�Դ����
*********************************************************/
void replaceString(char *pcSrc, const char *pcFrom, const char *pcTo)
{
	char *pcPrev = NULL;
	char *pcNext = NULL;
	char *pcOut = NULL;
	char *pcOutIndex = NULL;
	int len = 0;

	if (NULL == pcSrc || NULL == pcFrom || NULL == pcTo)
	{
		return;
	}
	//����
	if (strlen(pcTo) > strlen(pcFrom))
	{
		return;
	}
	pcOut = (char *)malloc(strlen(pcSrc) + 1);
	if (NULL == pcOut)
	{
		return;
	}
	memset(pcOut, 0, strlen(pcSrc) + 1);
	pcOutIndex = pcOut;
	len = strlen(pcSrc);
	pcPrev = (char *)pcSrc;
	pcNext = strstr(pcPrev, pcFrom);
	while (pcNext)
	{
		memcpy(pcOutIndex, pcPrev, pcNext - pcPrev);
		pcOutIndex += (pcNext - pcPrev);
		pcPrev = pcNext + strlen(pcFrom);
		strcpy(pcOutIndex, pcTo);
		pcOutIndex += strlen(pcTo);
		pcNext = strstr(pcPrev, pcFrom);
	}
	strcpy(pcOutIndex, pcPrev);
	//�滻���
	memset(pcSrc, 0, len);
	strcpy(pcSrc, pcOut);
}

/********************************************************
   Func Name: matchOperator
Date Created: 2018-9-29
 Description: �����ƥ���Լ��
	   Input: pcSrc��Դ�ַ���
	            lch�����ַ�
	            rch�����ַ�
	  Output: ƥ�������ַ�����--0��û��ƥ��
     Caution: 
*********************************************************/
size_t matchOperator(const char *pcSrc, char lch, char rch)
{
	STGTStackHead * header = NULL;
	const char *pcIndex = NULL;
	STCharacter *pstNode = NULL;
	size_t count = 0;

	if (NULL == pcSrc)
	{
		return 0;
	}

	header = GTStack_Init();
	if (NULL == header)
	{
		return 0;
	}
	pcIndex = pcSrc;
	while (*pcIndex)
	{
		if (lch == *pcIndex)
		{
			pstNode = (STCharacter *)malloc(sizeof(STCharacter));
			if (NULL == pstNode)
			{
				return 0;
			}
			memset(pstNode, 0, sizeof(STCharacter));
			pstNode->ch = *pcIndex;
			//��ջ
			GTStack_Push(header, &pstNode->stNode);
		}else if (rch == *pcIndex)
		{
			pstNode = LIST_ENTRY(GTStack_Top(header),STCharacter,stNode);
			if (pstNode)
			{
				//ƥ�䵽��Ӧ����
				count++;
				//����ջ��Ԫ��
				pstNode = LIST_ENTRY(GTStack_Pop(header), STCharacter, stNode);
				if (pstNode)
				{
					free(pstNode);
					pstNode = NULL;
				}
			}
			//�޷�ƥ�䵽��Ӧ�ķ��ţ�����
		}
		pcIndex++;
	}

	//�����ڴ�
	if (!GTStack_Empty(header))
	{
		while (pstNode = LIST_ENTRY(GTStack_Pop(header),STCharacter,stNode),pstNode)
		{
			if (pstNode)
			{
				free(pstNode);
				pstNode = NULL;
			}
		}
	}

	return count;
}

/********************************************************
	Func Name: char_to_hex
Date Created: 2018-10-10
	Description: �ַ�תʮ����������
		Input:
		Output:
		Return: ʮ����������
		Caution: ����(a =10,b=11)
*********************************************************/
char char_to_hex(char ch)
{
	char data = 0;

	switch (ch)
	{
	case '0':
		data = 0;
		break;
	case '1':
		data = 1;
		break;
	case '2':
		data = 2;
		break;
	case '3':
		data = 3;
		break;
	case '4':
		data = 4;
		break;
	case '5':
		data = 5;
		break;
	case '6':
		data = 6;
		break;
	case '7':
		data = 7;
		break;
	case '8':
		data = 8;
		break;
	case '9':
		data = 9;
		break;
	case 'a':
	case 'A':
		data = 10;
		break;
	case 'b':
	case 'B':
		data = 11;
		break;
	case 'c':
	case 'C':
		data = 12;
		break;
	case 'd':
	case 'D':
		data = 13;
		break;
	case 'e':
	case 'E':
		data = 14;
		break;
	case 'f':
	case 'F':
		data = 15;
		break;
	default:
		break;
	}

	return data;
}

