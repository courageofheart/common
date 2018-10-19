
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/base64.h"

#define SRC_CHAR_SIZE 3                //Դ��3���ֽ�
#define BASE_CHAR_SIZE 4               //�����4���ֽ�
#define CHAR_SIZE 8                    //һ���ֽ���8bits
#define BASE_DATA_SIZE 6               //base������6��bits��ʵ������

/********************************************************
   Func Name: base64_encode_value
Date Created: 2018-8-2
 Description: ��ȡ��Ӧ�����ֵ
       Input: value_in����Ҫ������ַ�
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
 Description: ��ȡ��Ӧ�����ֵ
       Input: value_in����Ҫ������ַ�
      Output: 
      Return: 
     Caution: 
*********************************************************/
int base64_decode_value(char value_in)
{
	static const char decoding[] = {62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};

	static const char decoding_size = sizeof(decoding);
	//+ ��ascllֵ��43
	value_in -= 43;
	if (value_in < 0 || value_in >= decoding_size) return -1;
	return decoding[(int)value_in];
}

/********************************************************
   Func Name: base64_encode
Date Created: 2018-8-2
 Description: base64����
       Input: plaintext_in��Դ�ļ�
	             length_in��Դ�ļ�����
	  Output:     code_out�����ɱ����ļ�
				length_out�����ɱ����ļ��ĳ���
      Return: 
     Caution: code_out�ڴ��ɵ��ú����ͷţ�code_out���ַ���
*********************************************************/
int base64_encode(const char *plaintext_in, int length_in, char **code_out, int *length_out)
{
	int iPadLen = 0;                 //��Ҫ������ֽ���
	int iBaseLen = 0;                //base64�������ֽ���
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
		//3 - length_in%3 Դ����Ҫ��ӵ��ֽ���
		iPadLen = SRC_CHAR_SIZE - length_in % SRC_CHAR_SIZE;
	}
	//����base�����ʵ�ʳ��� +1 ���һ���ַ���'/0'
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
				//ĩβʵ�����������ֽڵ����ݣ����������ֽڵ����ݽ��б���

				//��һ���ֽڴ���
				gPadChar[0] = base64_encode_value(*(plaintext_in+i) >> 2 & 0x3f);

				//�ڶ����ֽڴ���
				gPadChar[1] = base64_encode_value((*(plaintext_in+i) << 6 >> 2 & 0x30) + (*(plaintext_in+i+1) >> 4 & 0xf));

				//�ڶ����ֽ����4bits����,����Ĳ�0
				gPadChar[2] = base64_encode_value((*(plaintext_in+i+1) << 4 >> 2 & 0x3c));

				//û���ֽڵ���'='���� ��Ϊbase����ֻ��6bit��ʵ���ݣ����Բ����Դ�����е�"="�ظ�
				gPadChar[3] = '=';

			}else if (2 == iPadLen)
			{
				//ĩβʵ������һ���ֽڵ����ݣ�����һ���ֽڵ����ݽ��б���
				//��һ���ֽڴ���
				gPadChar[0] = base64_encode_value(*(plaintext_in+i) >> 2 & 0x3f);

				//��һ���ֽ����2bits����
				gPadChar[1] = base64_encode_value((*(plaintext_in+i) << 6 >> 2 & 0x30));

				gPadChar[2] = '=';
				gPadChar[3] = '=';
			}
			
		}else
		{
			//��һ���ֽڴ���  0x3fǰ��λ����
			//ȡ��һ���ֽڵ�ǰ6bits
			gPadChar[0] =base64_encode_value(*(plaintext_in+i) >> 2 & 0x3f);

			//�ڶ����ֽڴ���
			//*(pcIndex+i) << 6 >> 2 & 0x30 ȡ��һ���ֽڵĺ�2bits ��Ч��������
			//*(pcIndex+i+1) >> 4 & 0xf ȡ�ڶ����ֽڵ�ǰ4bits ��Ч��������
			gPadChar[1] = base64_encode_value((*(plaintext_in+i) << 6 >> 2 & 0x30) + (*(plaintext_in+i+1) >> 4 & 0xf));

			//�ڶ����ֽں͵������ֽڴ���
			//*(pcIndex+i+1) << 4 >> 2 & 0x3c ȡ�ڶ����ֽڵĺ�4bits���� ��Ч��������
			//(*(pcIndex+i+2) >> 6 & 0x3 ȡ�������ֽڵ�ǰ2bits���� ��Ч��������
			gPadChar[2] = base64_encode_value((*(plaintext_in+i+1) << 4 >> 2 & 0x3c) + (*(plaintext_in+i+2) >> 6 & 0x3));

			//�������ֽڴ���
			//*(pcIndex+i+2) & 0x3f  ����������ֽڵ�ǰ2bits����
			gPadChar[3] = base64_encode_value(*(plaintext_in+i+2) & 0x3f);
		}
		

		//�����ַ�������������ʹ��strcat
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
 Description: base64����
       Input:       code_in;�������ļ�
	              length_in���������ļ�����
	  Output: plaintext_out��Դ�ļ�
		             outlen��Դ�ļ�����
      Return: 
     Caution: plaintext_out�ڴ��ɵ��ú����ͷţ�plaintext_out���ַ���
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
	//����Դ�ļ����ַ�����
	iSrcLen = length_in/4*3 - iPadNum;

	//ĩβ����\0
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
				//ʵ�����������ַ� ֻ����3��base�ַ���ʾ

				//�ַ�1
				pcOut[j] = (base64_decode_value(code_in[i]) << 2) + (base64_decode_value(code_in[i+1]) << 2 >> 6 & 0x3);
				//�ַ�2
				pcOut[j+1] = (base64_decode_value(code_in[i+1]) << 4) + (base64_decode_value(code_in[i+2]) << 2 >> 4 & 0xf);
				j += 2;
			}else if (2 == iPadNum)
			{
				//ʵ������1���ַ����� ֻ����2��base�ַ���ʾ
				pcOut[j] = (base64_decode_value(code_in[i])<<2) + (base64_decode_value(code_in[i+1]) << 2 >> 6 &0x3);
				j ++;
			}
		}else
		{
			//�ַ�1
			pcOut[j] = (base64_decode_value(code_in[i])<<2) + (base64_decode_value(code_in[i+1]) << 2 >> 6 &0x3);
			//�ַ�2
			pcOut[j+1] = (base64_decode_value(code_in[i+1]) << 4) + (base64_decode_value(code_in[i+2]) << 2 >> 4 & 0xf);
			//�ַ�3
			pcOut[j+2] = (base64_decode_value(code_in[i+2]) << 6) + (base64_decode_value(code_in[i+3]) & 0x3f);
			j += 3;
		}
	}

	pcOut[iSrcLen] = '\0';
	*plaintext_out = pcOut;
	*outlen = iSrcLen;

	return 0;
}

