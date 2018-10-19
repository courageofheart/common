
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/base64.h"

#define SRC_CHAR_SIZE 3                //源码3个字节
#define BASE_CHAR_SIZE 4               //编码后4个字节
#define CHAR_SIZE 8                    //一个字节有8bits
#define BASE_DATA_SIZE 6               //base编码中6个bits是实际数据

/********************************************************
   Func Name: base64_encode_value
Date Created: 2018-8-2
 Description: 获取对应编码的值
       Input: value_in：需要编码的字符
      Output: 
      Return: 
     Caution: 
*********************************************************/
char base64_encode_value(char value_in)
{
	static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (value_in > 63) return '=';
	return encoding[(int)value_in];
}

/********************************************************
   Func Name: base64_decode_value
Date Created: 2018-8-2
 Description: 获取对应解码的值
       Input: value_in：需要解码的字符
      Output: 
      Return: 
     Caution: 
*********************************************************/
int base64_decode_value(char value_in)
{
	static const char decoding[] = {62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};

	static const char decoding_size = sizeof(decoding);
	//+ 的ascll值是43
	value_in -= 43;
	if (value_in < 0 || value_in >= decoding_size) return -1;
	return decoding[(int)value_in];
}

/********************************************************
   Func Name: base64_encode
Date Created: 2018-8-2
 Description: base64编码
       Input: plaintext_in：源文件
	             length_in：源文件长度
	  Output:     code_out：生成编码文件
				length_out：生成编码文件的长度
      Return: 
     Caution: code_out内存由调用函数释放，code_out是字符串
*********************************************************/
int base64_encode(const char *plaintext_in, int length_in, char **code_out, int *length_out)
{
	int iPadLen = 0;                 //需要补齐的字节数
	int iBaseLen = 0;                //base64编码后的字节数
	int i = 0;
	char *pcOut = NULL;
	char gPadChar[BASE_CHAR_SIZE] = {0};
	char * pcOutIndex = NULL;

	if (NULL == plaintext_in || 0 == length_in || NULL == code_out || NULL == length_out)
	{
		return -1;
	}

	if (0 != length_in % SRC_CHAR_SIZE)
	{
		//3 - length_in%3 源码需要添加的字节数
		iPadLen = SRC_CHAR_SIZE - length_in % SRC_CHAR_SIZE;
	}
	//计算base编码后实际长度 +1 最后一个字符是'/0'
	iBaseLen = (length_in + iPadLen)* CHAR_SIZE / BASE_DATA_SIZE + 1;

	pcOut = (char *)malloc(sizeof(char) * iBaseLen);
	if (NULL == pcOut)
	{
		return -2;
	}
	memset(pcOut, 0, sizeof(char) * iBaseLen);
	pcOutIndex = pcOut;

	for (i = 0; i < length_in; i += SRC_CHAR_SIZE)
	{
		if (i == (length_in + iPadLen -3) && 0 != iPadLen)
		{
			if (1 == iPadLen)
			{
				//末尾实际上有两个字节的数据，将这两个字节的数据进行编码

				//第一个字节处理
				gPadChar[0] = base64_encode_value(*(plaintext_in+i) >> 2 & 0x3f);

				//第二个字节处理
				gPadChar[1] = base64_encode_value((*(plaintext_in+i) << 6 >> 2 & 0x30) + (*(plaintext_in+i+1) >> 4 & 0xf));

				//第二个字节最后4bits处理,不足的补0
				gPadChar[2] = base64_encode_value((*(plaintext_in+i+1) << 4 >> 2 & 0x3c));

				//没有字节的以'='代替 因为base编码只有6bit真实数据，所以不会和源数据中的"="重复
				gPadChar[3] = '=';

			}else if (2 == iPadLen)
			{
				//末尾实际上有一个字节的数据，将这一个字节的数据进行编码
				//第一个字节处理
				gPadChar[0] = base64_encode_value(*(plaintext_in+i) >> 2 & 0x3f);

				//第一个字节最后2bits处理
				gPadChar[1] = base64_encode_value((*(plaintext_in+i) << 6 >> 2 & 0x30));

				gPadChar[2] = '=';
				gPadChar[3] = '=';
			}
			
		}else
		{
			//第一个字节处理  0x3f前两位清零
			//取第一个字节的前6bits
			gPadChar[0] =base64_encode_value(*(plaintext_in+i) >> 2 & 0x3f);

			//第二个字节处理
			//*(pcIndex+i) << 6 >> 2 & 0x30 取第一个字节的后2bits 无效数据清零
			//*(pcIndex+i+1) >> 4 & 0xf 取第二个字节的前4bits 无效数据清零
			gPadChar[1] = base64_encode_value((*(plaintext_in+i) << 6 >> 2 & 0x30) + (*(plaintext_in+i+1) >> 4 & 0xf));

			//第二个字节和第三个字节处理
			//*(pcIndex+i+1) << 4 >> 2 & 0x3c 取第二个字节的后4bits数据 无效数据清零
			//(*(pcIndex+i+2) >> 6 & 0x3 取第三个字节的前2bits数据 无效数据清零
			gPadChar[2] = base64_encode_value((*(plaintext_in+i+1) << 4 >> 2 & 0x3c) + (*(plaintext_in+i+2) >> 6 & 0x3));

			//第三个字节处理
			//*(pcIndex+i+2) & 0x3f  清除第三个字节的前2bits数据
			gPadChar[3] = base64_encode_value(*(plaintext_in+i+2) & 0x3f);
		}
		

		//并非字符串操作，不能使用strcat
		memcpy(pcOutIndex, gPadChar, BASE_CHAR_SIZE);
		pcOutIndex += BASE_CHAR_SIZE;

		memset(gPadChar, 0, BASE_CHAR_SIZE);
	}

	pcOut[iBaseLen-1] = 0;
	*length_out = iBaseLen;
	*code_out = pcOut;

	return 0;
}

/********************************************************
   Func Name: base64_decode
Date Created: 2018-8-3
 Description: base64解码
       Input:       code_in;编码后的文件
	              length_in：编码后的文件长度
	  Output: plaintext_out：源文件
		             outlen：源文件长度
      Return: 
     Caution: plaintext_out内存由调用函数释放，plaintext_out是字符串
*********************************************************/
int base64_decode(char *code_in, int length_in, char **plaintext_out, int *outlen)
{
	int i = 0, j = 0;
	int iPadNum = 0;
	char *pcSrc = code_in;
	char * pcIndex = NULL;
	int iSrcLen = 0;
	char *pcOut = NULL;

	if (NULL == code_in || NULL == plaintext_out || NULL == outlen)
	{
		return -1;
	}

	while(1)
	{
		pcIndex = strchr(pcSrc, '=');
		if (NULL == pcIndex)
		{
			break;
		}
		iPadNum++;
		pcIndex += 1;
		pcSrc = pcIndex;

	}
	//计算源文件的字符个数
	iSrcLen = length_in/4*3 - iPadNum;

	//末尾增加\0
	pcOut = (char *)malloc(sizeof(char)*iSrcLen + 1);
	if (NULL == pcOut)
	{
		return -2;
	}
	memset(pcOut, 0, sizeof(char)*iSrcLen + 1);

	for (i = 0, j = 0; i < length_in; i += 4)
	{
		if ((i == length_in-4) && iPadNum > 0)
		{
			if (1 == iPadNum)
			{
				//实际上有两个字符 只能用3个base字符表示

				//字符1
				pcOut[j] = (base64_decode_value(code_in[i]) << 2) + (base64_decode_value(code_in[i+1]) << 2 >> 6 & 0x3);
				//字符2
				pcOut[j+1] = (base64_decode_value(code_in[i+1]) << 4) + (base64_decode_value(code_in[i+2]) << 2 >> 4 & 0xf);
				j += 2;
			}else if (2 == iPadNum)
			{
				//实际上有1个字符数据 只能用2个base字符表示
				pcOut[j] = (base64_decode_value(code_in[i])<<2) + (base64_decode_value(code_in[i+1]) << 2 >> 6 &0x3);
				j ++;
			}
		}else
		{
			//字符1
			pcOut[j] = (base64_decode_value(code_in[i])<<2) + (base64_decode_value(code_in[i+1]) << 2 >> 6 &0x3);
			//字符2
			pcOut[j+1] = (base64_decode_value(code_in[i+1]) << 4) + (base64_decode_value(code_in[i+2]) << 2 >> 4 & 0xf);
			//字符3
			pcOut[j+2] = (base64_decode_value(code_in[i+2]) << 6) + (base64_decode_value(code_in[i+3]) & 0x3f);
			j += 3;
		}
	}

	pcOut[iSrcLen] = '\0';
	*plaintext_out = pcOut;
	*outlen = iSrcLen;

	return 0;
}

