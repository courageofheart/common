
#include "stringhelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
   Func Name: releaseGridMemory
Date Created: 2018-7-5
 Description: �ͷ��ַ���ָ������
       Input: pcGrid���ַ��������ָ��
      Output: 
      Return: 
     Caution: ���޸�Դ����
*********************************************************/
void replaceCharacter(const char *pcSrc, char cFrom, char cTo)
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


