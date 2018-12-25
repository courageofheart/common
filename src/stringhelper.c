
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
 Description: 去除一行中指定字符
       Input: pcBuf: 源字符数组
              cAppoint:需要删除的字符
      Output: 无
      Return: 成功返回0，失败返回负值
     Caution: 会修改源字符数组的内容,要求必须是字符串
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
 Description: 去除首尾空格
       Input: pcBuf: 源字符数组
      Output: 无
      Return: 成功返回0，失败返回负值
     Caution: 会修改源字符数组的内容,要求必须是字符串
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
 Description: 去除左边空格
       Input: pcBuf: 源字符数组
      Output: 无
      Return: 成功返回0，失败返回负值
     Caution: 会修改源字符数组的内容,要求必须是字符串
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

	//32 是空格的 ascii
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
 Description: 去除右边空格
       Input: pcBuf: 源字符数组
      Output: 无
      Return: 成功返回0，失败返回负值
     Caution: 会修改源字符数组的内容,要求必须是字符串
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

	//32 是空格的 ascii
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
 Description: 去除指定字符
       Input: pcBuf: 源字符数组
              cAppoint:需要删除的字符
              uiLen: 字符数组的长度
      Output: 无
      Return: 成功返回0，失败返回负值
     Caution: 会修改源字符数组的内容
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
 Description: 释放字符串指针数组
       Input: pcGrid：字符串数组的指针
      Output: 
      Return: 
     Caution: 数组要求最后一个元素为NULL
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
 Description: 替换指定字符
       Input: pcGrid：字符串数组的指针
      Output: 
      Return: 
     Caution: 会修改源数据
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
   Func Name: 替换指定字符串
Date Created: 2018-7-5
 Description: 释放字符串指针数组
       Input: pcGrid：字符串数组的指针
      Output: 
      Return: 
     Caution: 会修改源数据
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
	//保护
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
	//替换完成
	memset(pcSrc, 0, len);
	strcpy(pcSrc, pcOut);
}

/********************************************************
   Func Name: matchOperator
Date Created: 2018-9-29
 Description: 运算符匹配性检测
	   Input: pcSrc：源字符串
	            lch：左字符
	            rch：右字符
	  Output: 匹配左右字符个数--0即没有匹配
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
			//入栈
			GTStack_Push(header, &pstNode->stNode);
		}else if (rch == *pcIndex)
		{
			pstNode = LIST_ENTRY(GTStack_Top(header),STCharacter,stNode);
			if (pstNode)
			{
				//匹配到相应符号
				count++;
				//弹出栈顶元素
				pstNode = LIST_ENTRY(GTStack_Pop(header), STCharacter, stNode);
				if (pstNode)
				{
					free(pstNode);
					pstNode = NULL;
				}
			}
			//无法匹配到对应的符号，舍弃
		}
		pcIndex++;
	}

	//清理内存
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
	Description: 字符转十六进制数字
		Input:
		Output:
		Return: 十六进制数字
		Caution: 例如(a =10,b=11)
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

