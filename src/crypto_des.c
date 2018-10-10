
#include "crypto_des.h"
#include "openssl/des.h"

#include <stdlib.h>
#include <string.h>

#define DES_LEN 8

/********************************************************
   Func Name: desCodeCalculate
Date Created: 2018-8-22
 Description: des编码计算
	   Input:        pcDesKey：密钥字符串（必须是8个字节）  
	              pcPlaintext：明文（必须等于8个字节）
	  Output:    pcCiphertext：密文
      Return: 0表示加密成功，-1表示加密失败
     Caution: pcCiphertext内存需要由调用函数释放,对称加密，长度不变
*********************************************************/
int desCodeCalculate(const unsigned char *pcDesKey, unsigned char *pcPlaintext, unsigned char **pcCiphertext)
{
	DES_key_schedule schedule;
	unsigned char *pcOut = NULL;

	if (NULL == pcDesKey || NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(DES_LEN);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, DES_LEN);

	DES_set_key_unchecked((const_DES_cblock *)pcDesKey, &schedule);

	DES_ecb_encrypt((const_DES_cblock *)pcPlaintext, (DES_cblock *)pcOut, &schedule, DES_ENCRYPT);
	*pcCiphertext = pcOut;

	return 0;
}

/********************************************************
   Func Name: desDecodeCalculate
Date Created: 2018-8-22
 Description: des解码计算
	   Input:     pcDesKey：密钥字符串（必须是8个字节）
	             pcCiphertext：密文（一定是8个字节）
	  Output:      pcPlaintext：明文
      Return: 0表示解密成功，-1表示加密失败
     Caution: pcPlaintext内存需要由调用函数释放
*********************************************************/
int desDecodeCalculate(const unsigned char *pcDesKey, unsigned char *pcCiphertext, unsigned char **pcPlaintext)
{
	DES_key_schedule schedule;
	unsigned char *pcOut = NULL;

	if (NULL == pcDesKey || NULL == pcCiphertext || NULL == pcPlaintext)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(DES_LEN);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, DES_LEN);

	DES_set_key_unchecked((const_DES_cblock *)pcDesKey, &schedule);
	DES_ecb_encrypt((DES_cblock *)pcCiphertext, (DES_cblock *)pcOut, &schedule, DES_DECRYPT);

	*pcPlaintext = pcOut;

	return 0;
}

/********************************************************
   Func Name: des_encode
Date Created: 2018-8-22
 Description: des编码
	   Input:        pcDesKey：密钥字符串（必须是8个字节）
	              pcPlaintext：明文
			    iPlaintextLen：明文长度
	  Output:    pcCiphertext：密文
			  piCiphertextLen：密文长度
      Return: 0表示加密成功，-1表示加密失败
     Caution: pcCiphertext内存需要由调用函数释放
*********************************************************/
int des_encode(const unsigned char *pcDesKey, char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen)
{
	int iPadNum = 0;
	int iOutlen = 0;
	char *pcFormat = NULL;
	int i = 0;
	unsigned char gcBuf[8] = { 0 };
	unsigned char *pcTemp = NULL;
	char *pcIndex = NULL;
	char *pcOut = NULL;

	if (NULL == pcDesKey || NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	//判断字符串是否是8的整倍数
	iPadNum = 8 - iPlaintextLen % 8;
	if (0 == iPadNum)
	{
		iOutlen = DES_LEN + iPlaintextLen;
	}
	else
	{
		iOutlen = iPadNum + iPlaintextLen;
	}
	
	pcFormat = (char *)malloc(iOutlen);
	if (NULL == pcFormat)
	{
		return -1;
	}
	memset(pcFormat, iPadNum, iOutlen);
	//缺几补几 缺0补8个0
	memcpy(pcFormat, pcPlaintext, iPlaintextLen);

	pcOut = (char *)malloc(iOutlen);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, iOutlen);
	pcIndex = pcOut;

	for (i = 0; i < iOutlen; i += DES_LEN)
	{
		memset(gcBuf, 0, DES_LEN);
		memcpy(gcBuf, pcFormat + i, DES_LEN);
		desCodeCalculate(pcDesKey, gcBuf, &pcTemp);
		memcpy(pcIndex, pcTemp, DES_LEN);
		if (pcTemp)
		{
			free(pcTemp);
			pcTemp = NULL;
		}
		pcIndex += DES_LEN;
	}

	*pcCiphertext = pcOut;
	*piCiphertextLen = iOutlen;

	return 0;
}

/********************************************************
   Func Name: des_decode
Date Created: 2018-8-22
 Description: des解码
	   Input:     pcDesKey：密钥字符串（必须是8个字节）
	              pcCiphertext：密文
			    iCiphertextLen：密文长度
	  Output:      pcPlaintext：明文
			    piPlaintextLen：明文长度
      Return: 0表示解密成功，-1表示加密失败
     Caution: pcPlaintext内存需要由调用函数释放
*********************************************************/
int des_decode(const unsigned char *pcDesKey, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen)
{
	char *pcOut = NULL;
	int iOutlen = 0;
	int i = 0;
	unsigned char gcBuf[8] = { 0 };
	char *pcIndex = NULL;
	unsigned char *pcTemp = NULL;
	int iPadNum = 0;

	if (NULL == pcDesKey || NULL == pcCiphertext || NULL == pcPlaintext || NULL == piPlaintextLen)
	{
		return -1;
	}
	if (0 != iCiphertextLen%DES_LEN)
	{
		return -1;
	}

	//密文长度大于等于明文
	pcOut = (char *)malloc(iCiphertextLen);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, iCiphertextLen);
	pcIndex = pcOut;

	for (i = 0; i < iCiphertextLen; i += DES_LEN)
	{
		memset(gcBuf, 0, DES_LEN);
		memcpy(gcBuf, pcCiphertext + i, DES_LEN);
		desDecodeCalculate(pcDesKey, gcBuf, &pcTemp);
		memcpy(pcIndex, pcTemp, DES_LEN);
		if (pcTemp)
		{
			free(pcTemp);
			pcTemp = NULL;
		}
		pcIndex += DES_LEN;
	}

	iPadNum = pcOut[iCiphertextLen - 1];
	if (0 != iPadNum)
	{
		for (i = 0; i < iPadNum; i++)
		{
			pcOut[iCiphertextLen - 1 - i] = 0;
		}
	}
	else
	{
		iPadNum = DES_LEN;
	}

	iOutlen = iCiphertextLen - iPadNum;
	*pcPlaintext = pcOut;
	*piPlaintextLen = iOutlen;

	return 0;
}




