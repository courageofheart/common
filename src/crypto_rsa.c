
#include "common/crypto_rsa.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/err.h"

#include <math.h>
#include <string.h>

#define KEY_LEN 1024

/********************************************************
   Func Name: rsaCodeCalculate
Date Created: 2018-8-22
 Description: rsa�������
	   Input:    pcPrikeyPath��˽Կ�ļ�
	              pcPlaintext�����ģ�Ҫ�����Ĵ�С����С��128-11���ֽڣ�
			    iPlaintextLen�����ĳ���
	  Output:    pcCiphertext������
			  piCiphertextLen�����ĳ���
      Return: 0��ʾ���ܳɹ���-1��ʾ����ʧ��
     Caution: pcCiphertext�ڴ���Ҫ�ɵ��ú����ͷ�
*********************************************************/
int rsaCodeCalculate(const char *pcPrikeyPath,char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen)
{
	FILE *pfPrikey = NULL;
	RSA * pRSAPrivateKey = NULL;
	unsigned char *pcSecretkey = NULL;
	int iSecretLen = 0;
	int iOutlen = 0;

	if (NULL == pcPrikeyPath || NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	pfPrikey = fopen(pcPrikeyPath, "r");
	if (NULL == pfPrikey)
	{
		return -1;
	}

	pRSAPrivateKey = RSA_new();
	if (NULL == PEM_read_RSAPrivateKey(pfPrikey, &pRSAPrivateKey, 0, 0))
	{
		fclose(pfPrikey);
		pfPrikey = NULL;
		return -1;
	}
	fclose(pfPrikey);
	pfPrikey = NULL;

	iSecretLen = RSA_size(pRSAPrivateKey);
	pcSecretkey = (unsigned char *)malloc(iSecretLen + 1);
	if (NULL == pcSecretkey)
	{
		return -1;
	}

	iOutlen = RSA_private_encrypt(iPlaintextLen, (unsigned char *)pcPlaintext, pcSecretkey, pRSAPrivateKey, RSA_PKCS1_PADDING);
	if (iOutlen <= 0)
	{
		RSA_free(pRSAPrivateKey);
		pRSAPrivateKey = NULL;
		return -1;
	}
	RSA_free(pRSAPrivateKey);
	pRSAPrivateKey = NULL;

	*pcCiphertext = (char *)pcSecretkey;
	*piCiphertextLen = iOutlen;

	CRYPTO_cleanup_all_ex_data();

	return 0;
}

/********************************************************
   Func Name: rsaDecodeCalculate
Date Created: 2018-8-22
 Description: rsa�������
	   Input:     pcPrikeyPath��˽Կ
	              pcCiphertext�����ģ����ĳ��ȱ�����128���ֽڣ�
			    iCiphertextLen�����ĳ���
	  Output:      pcPlaintext������
			    piPlaintextLen�����ĳ���
      Return: 0��ʾ���ܳɹ���-1��ʾ����ʧ��
     Caution: pcPlaintext�ڴ���Ҫ�ɵ��ú����ͷ�
*********************************************************/
int rsaDecodeCalculate(const char *pcPubkeyPath, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen)
{
	FILE *pfPubkey = NULL;
	RSA * pRSAPublicKey = NULL;
	unsigned char *pcSecretkey = NULL;
	int iSecretLen = 0;
	int iOutlen = 0;

	if (NULL == pcPubkeyPath || NULL == pcCiphertext || NULL == pcPlaintext || NULL == piPlaintextLen)
	{
		return -1;
	}

	pfPubkey = fopen(pcPubkeyPath, "r");
	if (NULL == pfPubkey)
	{
		return -1;
	}

	pRSAPublicKey = RSA_new();
	if (NULL == PEM_read_RSA_PUBKEY(pfPubkey, &pRSAPublicKey, 0, 0))
	{
		fclose(pfPubkey);
		pfPubkey = NULL;
		return -1;
	}
	fclose(pfPubkey);
	pfPubkey = NULL;

	iSecretLen = RSA_size(pRSAPublicKey);
	pcSecretkey = (unsigned char *)malloc(iSecretLen + 1);
	if (NULL == pcSecretkey)
	{
		return -1;
	}

	iOutlen = RSA_public_decrypt(iCiphertextLen, (unsigned char *)pcCiphertext, pcSecretkey, pRSAPublicKey, RSA_PKCS1_PADDING);
	if (iOutlen <= 0)
	{
		RSA_free(pRSAPublicKey);
		pRSAPublicKey = NULL;
		return -1;
	}
	RSA_free(pRSAPublicKey);
	pRSAPublicKey = NULL;

	*pcPlaintext = (char *)pcSecretkey;
	*piPlaintextLen = iOutlen;

	CRYPTO_cleanup_all_ex_data();

	return 0;
}

/********************************************************
   Func Name: rsa_encode
Date Created: 2018-8-22
 Description: rsa����
	   Input:    pcPrikeyPath��˽Կ�ļ�
	              pcPlaintext������
			    iPlaintextLen�����ĳ���
	  Output:    pcCiphertext������
			  piCiphertextLen�����ĳ���
      Return: 0��ʾ���ܳɹ���-1��ʾ����ʧ��
     Caution: pcCiphertext�ڴ���Ҫ�ɵ��ú����ͷ�
*********************************************************/
int rsa_encode(const char *pcPrikeyPath,char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen)
{
	int i = 0;
	int space = 0;
	int iOutlen = 0;
	char *pcOutBuf = NULL;
	char *pcSpaceBuf = NULL;
	char *pcTempBuf = NULL;
	int iTempLen = 0;
	int iRet = 0;
	char *pcIndex = NULL;

	if (NULL == pcPrikeyPath || NULL == pcPlaintext || NULL == pcCiphertext || NULL == piCiphertextLen)
	{
		return -1;
	}

	//��������
	space = KEY_LEN / 8 - 11;
	//�������ĵĴ�С  ����ȡ��
	iOutlen = (int)ceil((double)iPlaintextLen / space) * (KEY_LEN / 8);
	pcOutBuf = (char *)malloc(iOutlen);
	if (NULL == pcOutBuf)
	{
		return -1;
	}
	memset(pcOutBuf, 0, iOutlen);
	pcIndex = pcOutBuf;

	pcSpaceBuf = (char *)malloc(space);
	if (NULL == pcSpaceBuf)
	{
		return -1;
	}

	for (i = 0; i < iPlaintextLen; i += space)
	{
		memset(pcSpaceBuf, 0, space);
		if ((i + space) >= iPlaintextLen)
		{
			memcpy(pcSpaceBuf, pcPlaintext+i, iPlaintextLen-i);
			iRet = rsaCodeCalculate(pcPrikeyPath, pcSpaceBuf, iPlaintextLen - i, &pcTempBuf, &iTempLen);
			if (0 != iRet)
			{
				break;
			}
			//ƴ�Ӽ�������
			memcpy(pcIndex, pcTempBuf, iTempLen);
			//�ͷŵ��ú����ڴ�
			if (pcTempBuf)
			{
				free(pcTempBuf);
				pcTempBuf = NULL;
			}
			break;
		}
		memcpy(pcSpaceBuf, pcPlaintext+i, space);
		iRet = rsaCodeCalculate(pcPrikeyPath, pcSpaceBuf, space, &pcTempBuf, &iTempLen);
		if (0 != iRet)
		{
			break;
		}
		//ƴ�Ӽ�������
		memcpy(pcIndex, pcTempBuf, iTempLen);
		if (pcTempBuf)
		{
			free(pcTempBuf);
			pcTempBuf = NULL;
		}
		pcIndex += iTempLen;
	}

	if (pcSpaceBuf)
	{
		free(pcSpaceBuf);
		pcSpaceBuf = NULL;
	}

	*pcCiphertext = pcOutBuf;
	*piCiphertextLen = iOutlen;

	return iRet;
}

/********************************************************
   Func Name: rsa_decode
Date Created: 2018-8-22
 Description: rsa����
	   Input:     pcPrikeyPath��˽Կ
	              pcCiphertext������
			    iCiphertextLen�����ĳ��ȣ�������key len����������
	  Output:      pcPlaintext������
			    piPlaintextLen�����ĳ���
      Return: 0��ʾ���ܳɹ���-1��ʾ����ʧ��
     Caution: pcPlaintext�ڴ���Ҫ�ɵ��ú����ͷ�
*********************************************************/
int rsa_decode(const char *pcPubkeyPath, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen)
{
	int i = 0;
	int space = 0;
	int iOutlen = 0;
	char *pcOutBuf = NULL;
	char *pcSpaceBuf = NULL;
	char *pcTempBuf = NULL;
	int iTempLen = 0;
	int iRet = 0;
	char *pcIndex = NULL;

	if (NULL == pcPubkeyPath || NULL == pcCiphertext || NULL == pcPlaintext || NULL == piPlaintextLen)
	{
		return -1;
	}

	space = KEY_LEN / 8;
	if (0 != (iCiphertextLen % space))
	{
		return -1;
	}

	//���ĳ��Ⱦ��Դ������ĳ���
	pcOutBuf = (char *)malloc(iCiphertextLen);
	if (NULL == pcOutBuf)
	{
		return -1;
	}
	memset(pcOutBuf, 0, iCiphertextLen);
	pcIndex = pcOutBuf;

	pcSpaceBuf = (char *)malloc(space);
	if (NULL == pcSpaceBuf)
	{
		return -1;
	}

	for (i = 0; i < iCiphertextLen; i += space)
	{
		memset(pcSpaceBuf, 0, space);
		memcpy(pcSpaceBuf, pcCiphertext + i, space);
		iRet = rsaDecodeCalculate(pcPubkeyPath, pcSpaceBuf, space, &pcTempBuf, &iTempLen);
		if (0 != iRet)
		{
			break;
		}
		memcpy(pcIndex, pcTempBuf, iTempLen);
		pcIndex += iTempLen;
		iOutlen += iTempLen;
	}

	if (pcSpaceBuf)
	{
		free(pcSpaceBuf);
		pcSpaceBuf = NULL;
	}

	*pcPlaintext = pcOutBuf;
	*piPlaintextLen = iOutlen;

	return iRet;

}

