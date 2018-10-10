
#include "crypto_base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

/********************************************************
   Func Name: hex_string
Date Created: 2018-10-10
 Description: 字符串转十六进制字符
       Input: value_in：需要编码的字符
      Output: 
      Return: 
     Caution: 返回的pcDst是字符串
*********************************************************/
int string_to_hex(unsigned char *pcSrc, int inLen, char **pcDst)
{
	uint8_t higt = 0;
	uint8_t low = 0;
	int i = 0;
	char *pcOut = NULL;
	char gcBuf[3] = { 0 };

	if (NULL == pcSrc || 0 == inLen || NULL == pcDst)
	{
		return -1;
	}

	//一个字符用两个十六进制字符表示
	pcOut = (char *)malloc(inLen * 2 + 1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, inLen * 2 + 1);

	for (i = 0; i < inLen; i++)
	{
		higt = pcSrc[i] >> 4;
		low = pcSrc[i] & 0x0f;
		sprintf(gcBuf, "%x%x", higt, low);
		strcat(pcOut, gcBuf);
	}

	*pcDst = pcOut;
	
	return 0;
}

/********************************************************
	Func Name: hex_to_string
Date Created: 2018-10-10
	Description: 十六进制字符串转字符串
		Input:
		Output:
		Return:
		Caution: 返回的pcDst是字符数组，不是字符串
*********************************************************/
int hex_to_string(unsigned char *pcSrc, unsigned char **pcDst, int *outLen)
{
	int i = 0;
	unsigned char *pcOut = NULL;
	int len = 0;
	uint8_t higt = 0;
	uint8_t low = 0;

	if (NULL == pcSrc || NULL == pcDst || NULL == outLen)
	{
		return -1;
	}

	len = strlen((char *)pcSrc);
	if ( 0 != len%2)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(len / 2);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, len / 2);

	for (i = 0; i < len / 2; i++)
	{
		higt = (uint8_t)char_to_hex(pcSrc[2 * i]);
		low = (uint8_t)char_to_hex(pcSrc[2 * i + 1]);
		pcOut[i] = (higt << 4) + (low & 0xf);
	}

	*pcDst = pcOut;
	*outLen = len / 2;

	return 0;
}

/********************************************************
	Func Name: char_to_hex
Date Created: 2018-10-10
	Description: 字符转十六进制
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


