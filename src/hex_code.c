
#include "common/hex_code.h"
#include "common/stringhelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/********************************************************
   Func Name: hex_encode
Date Created: 2018-10-10
 Description: 十六进制编码(字符串转十六进制字符)
       Input: value_in：需要编码的字符
      Output: 
      Return: 
     Caution: 返回的pcDst是字符串
*********************************************************/
int hex_encode(unsigned char *pcSrc, int inLen, char **pcDst)
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
	Func Name: hex_decode
Date Created: 2018-10-10
	Description: 十六进制解码(十六进制字符串转字符串)
		Input:
		Output:
		Return:
		Caution: 返回的pcDst是字符数组，不是字符串
*********************************************************/
int hex_decode(unsigned char *pcSrc, unsigned char **pcDst, int *outLen)
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

