
#include "crypto_sha.h"
#include "openssl/sha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************
   Func Name: printHash
Date Created: 2018-9-29
 Description: 打印hash
	   Input: 
	  Output:
      Return: 
     Caution: pcOut需要由调用函数分配
*********************************************************/
static void printHash(unsigned char *md, int len,char *pcOut)
{
	int i = 0;
	char temp[3] = { 0 };
	for (i = 0; i < len; i++)
	{
		memset(temp, 0, 3);
		sprintf(temp, "%02x", md[i]);
		strcat(pcOut, temp);
	}
}

/********************************************************
   Func Name: sha_encode
Date Created: 2018-9-29
 Description: sha加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放
*********************************************************/
int sha_encode(SHA_TYPE type
	, unsigned char *pcPlaintext
	, int iPlaintextLen
	, unsigned char **pcCiphertext
	, int *piCiphertextLen)
{
	int result = 0;

	if (NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	switch (type)
	{
	case SHA_1:
		result = sha1_encode(pcPlaintext, iPlaintextLen, pcCiphertext, piCiphertextLen);
		break;
	case SHA_224:
		result = sha224_encode(pcPlaintext, iPlaintextLen, pcCiphertext, piCiphertextLen);
		break;
	case SHA_256:
		result = sha256_encode(pcPlaintext, iPlaintextLen, pcCiphertext, piCiphertextLen);
		break;
	case SHA_384:
		result = sha384_encode(pcPlaintext, iPlaintextLen, pcCiphertext, piCiphertextLen);
		break;
	case SHA_512:
		result = sha512_encode(pcPlaintext, iPlaintextLen, pcCiphertext, piCiphertextLen);
		break;
	default:
		result = -1;
		break;
	}

	return result;
}

/********************************************************
   Func Name: sha_encode_hex
Date Created: 2018-9-29
 Description: sha加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放，且pcCiphertext是字符串
*********************************************************/
int sha_encode_hex(SHA_TYPE type
	, unsigned char *pcPlaintext
	, int iPlaintextLen
	, unsigned char **pcCiphertext)
{
	int result = 0;
	int outLen = 0;
	unsigned char *pcTmp = NULL;
	unsigned char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	switch (type)
	{
	case SHA_1:
		result = sha1_encode(pcPlaintext, iPlaintextLen, &pcTmp, &outLen);
		break;
	case SHA_224:
		result = sha224_encode(pcPlaintext, iPlaintextLen, &pcTmp, &outLen);
		break;
	case SHA_256:
		result = sha256_encode(pcPlaintext, iPlaintextLen, &pcTmp, &outLen);
		break;
	case SHA_384:
		result = sha384_encode(pcPlaintext, iPlaintextLen, &pcTmp, &outLen);
		break;
	case SHA_512:
		result = sha512_encode(pcPlaintext, iPlaintextLen, &pcTmp, &outLen);
		break;
	default:
		result = -1;
		break;
	}

	if (result)
	{
		return -1;
	}

	//+1 多加一个\0
	pcOut = (unsigned char *)malloc(outLen * 2 + 1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, outLen * 2 + 1);
	printHash(pcTmp, outLen, (char *)pcOut);

	*pcCiphertext = pcOut;

	return result;
}

/********************************************************
   Func Name: sha1_encode
Date Created: 2018-9-29
 Description: sha1加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放
*********************************************************/
int sha1_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen)
{
	unsigned char md[SHA_DIGEST_LENGTH];
	unsigned char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(SHA_DIGEST_LENGTH);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA_DIGEST_LENGTH);
	SHA1(pcPlaintext, iPlaintextLen, md);
	memcpy(pcOut, md, SHA_DIGEST_LENGTH);

	*pcCiphertext = pcOut;
	*piCiphertextLen = SHA_DIGEST_LENGTH;

	return 0;
}

/********************************************************
   Func Name: sha224_encode
Date Created: 2018-9-29
 Description: sha224加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放
*********************************************************/
int sha224_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen)
{
	unsigned char md[SHA224_DIGEST_LENGTH];
	unsigned char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(SHA224_DIGEST_LENGTH);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA224_DIGEST_LENGTH);
	SHA224(pcPlaintext, iPlaintextLen, md);
	memcpy(pcOut, md, SHA224_DIGEST_LENGTH);

	*pcCiphertext = pcOut;
	*piCiphertextLen = SHA224_DIGEST_LENGTH;

	return 0;
}

/********************************************************
   Func Name: sha256_encode
Date Created: 2018-9-29
 Description: sha256加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放
*********************************************************/
int sha256_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen)
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	unsigned char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(SHA256_DIGEST_LENGTH);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA256_DIGEST_LENGTH);
	SHA256(pcPlaintext, iPlaintextLen, md);
	memcpy(pcOut, md, SHA256_DIGEST_LENGTH);

	*pcCiphertext = pcOut;
	*piCiphertextLen = SHA256_DIGEST_LENGTH;

	return 0;
}

/********************************************************
   Func Name: sha384_encode
Date Created: 2018-9-29
 Description: sha384加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放
*********************************************************/
int sha384_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen)
{
	unsigned char md[SHA384_DIGEST_LENGTH];
	unsigned char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(SHA384_DIGEST_LENGTH);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA384_DIGEST_LENGTH);
	SHA384(pcPlaintext, iPlaintextLen, md);
	memcpy(pcOut, md, SHA384_DIGEST_LENGTH);

	*pcCiphertext = pcOut;
	*piCiphertextLen = SHA384_DIGEST_LENGTH;

	return 0;
}

/********************************************************
   Func Name: sha512_encode
Date Created: 2018-9-29
 Description: sha512加密
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext需要由调用函数释放
*********************************************************/
int sha512_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen)
{
	unsigned char md[SHA512_DIGEST_LENGTH];
	unsigned char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	pcOut = (unsigned char *)malloc(SHA512_DIGEST_LENGTH);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA512_DIGEST_LENGTH);
	SHA512(pcPlaintext, iPlaintextLen, md);
	memcpy(pcOut, md, SHA512_DIGEST_LENGTH);

	*pcCiphertext = pcOut;
	*piCiphertextLen = SHA512_DIGEST_LENGTH;

	return 0;
}


