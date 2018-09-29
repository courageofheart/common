
#include "crypto_sha.h"
#include "openssl/sha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************
   Func Name: printHash
Date Created: 2018-9-29
 Description: ��ӡhash
	   Input: 
	  Output:
      Return: 
     Caution: pcOut��Ҫ�ɵ��ú�������
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
   Func Name: sha1_encode
Date Created: 2018-9-29
 Description: sha1����
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext��Ҫ�ɵ��ú����ͷ�
*********************************************************/
int sha1_encode(unsigned char *pcPlaintext, char **pcCiphertext)
{
	unsigned char md[SHA_DIGEST_LENGTH];
	char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	//+1 Ԥ���ַ���������
	pcOut = (char *)malloc(SHA_DIGEST_LENGTH * 2+1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA_DIGEST_LENGTH * 2+1);
	SHA1(pcPlaintext, strlen((char *)pcPlaintext), md);
	printHash(md, SHA_DIGEST_LENGTH, pcOut);

	*pcCiphertext = pcOut;

	return 0;
}

/********************************************************
   Func Name: sha224_encode
Date Created: 2018-9-29
 Description: sha224����
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext��Ҫ�ɵ��ú����ͷ�
*********************************************************/
int sha224_encode(unsigned char *pcPlaintext, char **pcCiphertext)
{
	unsigned char md[SHA224_DIGEST_LENGTH];
	char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	//+1 Ԥ���ַ���������
	pcOut = (char *)malloc(SHA224_DIGEST_LENGTH * 2+1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA224_DIGEST_LENGTH * 2+1);
	SHA224(pcPlaintext, strlen((char *)pcPlaintext), md);
	printHash(md, SHA224_DIGEST_LENGTH, pcOut);

	*pcCiphertext = pcOut;

	return 0;
}

/********************************************************
   Func Name: sha256_encode
Date Created: 2018-9-29
 Description: sha256����
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext��Ҫ�ɵ��ú����ͷ�
*********************************************************/
int sha256_encode(unsigned char *pcPlaintext, char **pcCiphertext)
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	//+1 Ԥ���ַ���������
	pcOut = (char *)malloc(SHA256_DIGEST_LENGTH * 2+1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA256_DIGEST_LENGTH * 2+1);
	SHA256(pcPlaintext, strlen((char *)pcPlaintext), md);
	printHash(md, SHA256_DIGEST_LENGTH, pcOut);

	*pcCiphertext = pcOut;

	return 0;
}

/********************************************************
   Func Name: sha384_encode
Date Created: 2018-9-29
 Description: sha384����
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext��Ҫ�ɵ��ú����ͷ�
*********************************************************/
int sha384_encode(unsigned char *pcPlaintext, char **pcCiphertext)
{
	unsigned char md[SHA384_DIGEST_LENGTH];
	char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	//+1 Ԥ���ַ���������
	pcOut = (char *)malloc(SHA384_DIGEST_LENGTH * 2+1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA384_DIGEST_LENGTH * 2+1);
	SHA384(pcPlaintext, strlen((char *)pcPlaintext), md);
	printHash(md, SHA384_DIGEST_LENGTH, pcOut);

	*pcCiphertext = pcOut;

	return 0;
}

/********************************************************
   Func Name: sha512_encode
Date Created: 2018-9-29
 Description: sha512����
	   Input: 
	  Output:
      Return: error code
     Caution: pcCiphertext��Ҫ�ɵ��ú����ͷ�
*********************************************************/
int sha512_encode(unsigned char *pcPlaintext, char **pcCiphertext)
{
	unsigned char md[SHA512_DIGEST_LENGTH];
	char *pcOut = NULL;

	if (NULL == pcPlaintext || NULL == pcCiphertext)
	{
		return -1;
	}

	//+1 Ԥ���ַ���������
	pcOut = (char *)malloc(SHA512_DIGEST_LENGTH * 2+1);
	if (NULL == pcOut)
	{
		return -1;
	}
	memset(pcOut, 0, SHA512_DIGEST_LENGTH * 2+1);
	SHA512(pcPlaintext, strlen((char *)pcPlaintext), md);
	printHash(md, SHA512_DIGEST_LENGTH, pcOut);

	*pcCiphertext = pcOut;

	return 0;
}


