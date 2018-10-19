
#include "common/crypto_aes.h"
#include "common/openssl/aes.h"

#include <stdlib.h>
#include <string.h>

#define SLICE_SIZE 16

/********************************************************
   Func Name: getEncryptKey
Date Created: 2018-9-5
 Description: 获取aes加密密钥
	   Input: pcUserKey：用户密码 
	             pstKey：AES密钥
	  Output:
      Return: error code
     Caution: pstKey需要由调用函数分配
*********************************************************/
static int getEncryptKey(const unsigned char *pcUserKey, AES_KEY *pstKey)
{
	int iSize = 0;
	if (NULL == pcUserKey || NULL == pstKey)
	{
		return -1;
	}

	iSize = strlen((char *)pcUserKey);
	if (!(16 == iSize || 24 == iSize || 32 == iSize))
	{
		return -1;
	}

	if (AES_set_encrypt_key(pcUserKey, iSize*8, pstKey))
	{
		return -1;
	}

	return 0;
}

/********************************************************
   Func Name: getDecryptKey
Date Created: 2018-9-5
 Description: 获取aes解密密钥
	   Input: pcUserKey：用户密码 
	             pstKey：AES密钥
	  Output:
      Return: error code
     Caution: pstKey需要由调用函数分配
*********************************************************/
static int getDecryptKey(const unsigned char *pcUserKey, AES_KEY *pstKey)
{
	int iSize = 0;
	if (NULL == pcUserKey || NULL == pstKey)
	{
		return -1;
	}

	iSize = strlen((char *)pcUserKey);
	if (!(16 == iSize || 24 == iSize || 32 == iSize))
	{
		return -1;
	}

	if (AES_set_decrypt_key(pcUserKey, iSize * 8, pstKey))
	{
		return -1;
	}

	return 0;
}

/********************************************************
   Func Name: aesCodeCalculate
Date Created: 2018-8-22
 Description: aes编码计算
	   Input:      pcFirstKey：密钥字符串（必须是16字节或者24字节或者32字节）
	              pcSecondKey：密钥字符串（必须等于16个字节）
	              pcPlaintext：明文（必须等于16个字节）
	  Output:    pcCiphertext：密文
      Return: 0表示加密成功，-1表示加密失败
     Caution: pcCiphertext内存需要由调用函数释放,对称加密，长度不变
*********************************************************/
int aesCodeCalculate(const unsigned char *pcFirstKey, const unsigned char *pcSecondKey
	, unsigned char *pcPlaintext, unsigned char **pcCiphertext)
{
	AES_KEY secretKey;
	unsigned char *pcOutBuf = NULL;

	if (NULL == pcFirstKey || NULL == pcSecondKey || NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	memset(&secretKey, 0, sizeof(AES_KEY));
	//获取密钥
	if (getEncryptKey(pcFirstKey, &secretKey))
	{
		return -1;
	}

	pcOutBuf = (unsigned char *)malloc(SLICE_SIZE);
	if (NULL == pcOutBuf)
	{
		return -1;
	}
	memset(pcOutBuf, 0, SLICE_SIZE);

	//因为AES_cbc_encrypt()函数会修改pcSecondKey，所以需要拷贝一份
	{
		unsigned char *pcCopy = NULL;
		pcCopy = (unsigned char *)malloc(SLICE_SIZE);
		if (NULL == pcCopy)
		{
			return -1;
		}
		memset(pcCopy, 0, SLICE_SIZE);
		memcpy(pcCopy, pcSecondKey, SLICE_SIZE);
		AES_cbc_encrypt(pcPlaintext, pcOutBuf, SLICE_SIZE, &secretKey, pcCopy, AES_ENCRYPT);
	}

	*pcCiphertext = pcOutBuf;

	return 0;
}

/********************************************************
   Func Name: aesDecodeCalculate
Date Created: 2018-8-22
 Description: aes解码计算
	   Input:      pcFirstKey：密钥字符串（必须是16字节或者24字节或者32字节）
	              pcSecondKey：密钥字符串（必须等于16个字节）
	              pcPlaintext：明文（必须等于16个字节）
	  Output:    pcCiphertext：密文
      Return: 0表示加密成功，-1表示加密失败
     Caution: pcCiphertext内存需要由调用函数释放,对称加密，长度不变
*********************************************************/
int aesDecodeCalculate(const unsigned char *pcFirstKey, const unsigned char *pcSecondKey
	, unsigned char *pcPlaintext, unsigned char **pcCiphertext)
{
	AES_KEY secretKey;
	unsigned char *pcOutBuf = NULL;

	if (NULL == pcFirstKey || NULL == pcSecondKey || NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	memset(&secretKey, 0, sizeof(AES_KEY));
	//获取密钥
	if (getDecryptKey(pcFirstKey, &secretKey))
	{
		return -1;
	}

	pcOutBuf = (unsigned char *)malloc(SLICE_SIZE);
	if (NULL == pcOutBuf)
	{
		return -1;
	}
	memset(pcOutBuf, 0, SLICE_SIZE);

	//因为AES_cbc_encrypt()函数会修改pcSecondKey，所以需要拷贝一份
	{
		unsigned char *pcCopy = NULL;
		pcCopy = (unsigned char *)malloc(SLICE_SIZE);
		if (NULL == pcCopy)
		{
			return -1;
		}
		memset(pcCopy, 0, SLICE_SIZE);
		memcpy(pcCopy, pcSecondKey, SLICE_SIZE);
		AES_cbc_encrypt(pcPlaintext, pcOutBuf, SLICE_SIZE, &secretKey, pcCopy, AES_DECRYPT);
	}

	*pcCiphertext = pcOutBuf;

	return 0;
}

/********************************************************
   Func Name: aes_encode
Date Created: 2018-9-5
 Description: aes编码
	   Input:      pcFirstKey：密钥字符串（必须是16字节或者24字节或者32字节）
				  pcSecondKey：密钥字符串（必须等于16个字节）
	              pcPlaintext：明文
			    iPlaintextLen：明文长度
	  Output:    pcCiphertext：密文
			  piCiphertextLen：密文长度
      Return: 0表示加密成功，-1表示加密失败
     Caution: pcCiphertext内存需要由调用函数释放
*********************************************************/
int aes_encode(const unsigned char *pcFirstKey, const unsigned char *pcSecondKey, char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen)
{
	int iPadNum = 0;
	int iOutlen = 0;
	char *pcFormat = NULL;
	int i = 0;
	unsigned char gcBuf[SLICE_SIZE] = { 0 };
	unsigned char *pcTemp = NULL;
	char *pcIndex = NULL;
	char *pcOut = NULL;

	if (NULL == pcFirstKey || NULL == pcSecondKey || NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	//判断字符串是否是16的整倍数
	iPadNum = SLICE_SIZE - iPlaintextLen % SLICE_SIZE;
	if (0 == iPadNum)
	{
		iOutlen = SLICE_SIZE + iPlaintextLen;
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
	//缺几补几 缺0补16个0
	memcpy(pcFormat, pcPlaintext, iPlaintextLen);

	pcOut = (char *)malloc(iOutlen);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, iOutlen);
	pcIndex = pcOut;

	for (i = 0; i < iOutlen; i += SLICE_SIZE)
	{
		memset(gcBuf, 0, SLICE_SIZE);
		memcpy(gcBuf, pcFormat + i, SLICE_SIZE);
		if (aesCodeCalculate(pcFirstKey, pcSecondKey, gcBuf, &pcTemp))
		{
			return -1;
		}
		memcpy(pcIndex, pcTemp, SLICE_SIZE);
		if (pcTemp)
		{
			free(pcTemp);
			pcTemp = NULL;
		}
		pcIndex += SLICE_SIZE;
	}

	*pcCiphertext = pcOut;
	*piCiphertextLen = iOutlen;

	return 0;
}


/********************************************************
   Func Name: des_decode
Date Created: 2018-9-5
 Description: aes解码
	   Input:       pcFirstKey：密钥字符串（必须是16字节或者24字节或者32字节）
				   pcSecondKey：密钥字符串（必须等于16个字节）
	              pcCiphertext：密文
			    iCiphertextLen：密文长度
	  Output:      pcPlaintext：明文
			    piPlaintextLen：明文长度
      Return: 0表示解密成功，-1表示加密失败
     Caution: pcPlaintext内存需要由调用函数释放
*********************************************************/
int aes_decode(const unsigned char *pcFirstKey, const unsigned char *pcSecondKey
	, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen)
{
	char *pcOut = NULL;
	int iOutlen = 0;
	int i = 0;
	unsigned char gcBuf[SLICE_SIZE] = { 0 };
	char *pcIndex = NULL;
	unsigned char *pcTemp = NULL;
	int iPadNum = 0;

	if (NULL == pcFirstKey || NULL == pcSecondKey || NULL == pcCiphertext || NULL == pcPlaintext || NULL == piPlaintextLen)
	{
		return -1;
	}
	if (0 != iCiphertextLen % SLICE_SIZE)
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

	for (i = 0; i < iCiphertextLen; i += SLICE_SIZE)
	{
		memset(gcBuf, 0, SLICE_SIZE);
		memcpy(gcBuf, pcCiphertext + i, SLICE_SIZE);
		if (aesDecodeCalculate(pcFirstKey,pcSecondKey, gcBuf, &pcTemp))
		{
			return -1;
		}
		memcpy(pcIndex, pcTemp, SLICE_SIZE);
		if (pcTemp)
		{
			free(pcTemp);
			pcTemp = NULL;
		}
		pcIndex += SLICE_SIZE;
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
		iPadNum = SLICE_SIZE;
	}

	iOutlen = iCiphertextLen - iPadNum;
	*pcPlaintext = pcOut;
	*piPlaintextLen = iOutlen;

	return 0;
}





