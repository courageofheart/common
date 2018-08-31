

#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#include "ucharcode.h"
#include "uchardet.h"


/********************************************************
   Func Name: transcodeToUTF8
Date Created: 2018-8-3
 Description: ת��UTF8
       Input: pcSrc��Դ��������
	          lenIn��Դ���볤��
			 pcDest���������ڴ��ַ��INOUT��
			 lenOut��ʣ���ڴ浥λ������INOUT��
	     pcCodeType��Դ��������
      Output:         
      Return: error code
     Caution: pcDest�ڴ���Ҫ�ɵ��ú�������
*********************************************************/
int fromcodeToCode(char *pcSrc, int lenIn, char *pcDest, int *lenOut, const char *pcFromCode, const char *pcToCode)
{
	int iRet = 0;

	//����iconv()�������޸�ָ�룬����Ҫ����Դָ��
	char *pcStart = pcSrc;
	char *pcOut = pcDest;

	iconv_t cd = (iconv_t)-1;
	size_t inbytesleft = (size_t)lenIn;

	if (NULL == pcSrc || 0 >= lenIn || NULL == pcDest || NULL == lenOut || NULL == pcFromCode || NULL == pcToCode)
	{
		return -1;
	}

	cd = iconv_open(pcToCode, pcFromCode);
	if ((iconv_t)-1 == cd)
	{
		return -2;
	}

	/* 
	   *@param cd iconv_open()�����ľ��
	   *@param pcStart ��Ҫת�����ַ���
	   *@param inbytesleft ��Ż��ж����ַ�û��ת��
	   *@param pcOut ���ת������ַ���
	   *@param outlen ���ת����,pcOutʣ��Ŀռ�
    */
	iRet = iconv(cd, &pcStart, &inbytesleft, &pcOut, (size_t *)lenOut);
	if (iRet < 0)
	{
		return -3;
	}

	iconv_close(cd);

	return iRet;
}

/********************************************************
   Func Name: transcodeToUTF8
Date Created: 2018-8-3
 Description: ת��UTF8
       Input: pcSrc��Դ��������
	          lenIn��Դ���볤��
			 pcDest���������ڴ��ַ��INOUT��
			 lenOut��ʣ���ڴ浥λ������INOUT��
	     pcCodeType��Դ��������
      Output:         
      Return: error code
     Caution: pcDest�ڴ���Ҫ�ɵ��ú�������
*********************************************************/
int transcodeToUTF8(char *pcSrc, int lenIn, char *pcDest, int *lenOut,const char *pcCodeType)
{
	int iRet = 0;

	const char * targetCode = "UTF8";
	if (NULL == pcSrc || 0 >= lenIn || NULL == pcDest || NULL == lenOut || NULL == pcCodeType)
	{
		return -1;
	}

	iRet = fromcodeToCode(pcSrc, lenIn, pcDest, lenOut, pcCodeType, targetCode);

	return iRet;
}

/********************************************************
   Func Name: transcodeToGBK
Date Created: 2018-8-3
 Description: ת��GBK
       Input: pcSrc��Դ��������
	          lenIn��Դ���볤��
			 pcDest���������ڴ��ַ��INOUT��
			 lenOut��ʣ���ڴ浥λ������INOUT��
	     pcCodeType��Դ��������
      Output:         
      Return: error code
     Caution: pcDest�ڴ���Ҫ�ɵ��ú�������
*********************************************************/
int transcodeToGBK(char *pcSrc, int lenIn, char *pcDest, int *lenOut, const char *pcCodeType)
{
	int iRet = 0;

	const char * targetCode = "GBK";
	if (NULL == pcSrc || 0 >= lenIn || NULL == pcDest || NULL == lenOut || NULL == pcCodeType)
	{
		return -1;
	}

	iRet = fromcodeToCode(pcSrc, lenIn, pcDest, lenOut, pcCodeType, targetCode);

	return iRet;
}

/********************************************************
   Func Name: getStringCode
Date Created: 2018-8-3
 Description: ��ȡ�ַ�������
       Input: pcSrc��Դ��������
	         iLenIn��Դ���볤��
			 pcCode���������ڴ��ַ
      Output:         
      Return: error code
     Caution: pcDest�ڴ���Ҫ�ɵ��ú����ͷ�
*********************************************************/
int getStringCode(const char *pcSrc, int iLenIn, char **pcCode)
{
	uchardet_t ud;
	int iErrorCode = 0;
	char *pcBuf = NULL;
	const char *pcDest = NULL;

	if (NULL == pcSrc || 0 == iLenIn || NULL == pcCode)
	{
		return -1;
	}

	ud = uchardet_new();

	//��������ַ���������ô�п��ܵ��·���ʧ��
	iErrorCode = uchardet_handle_data(ud, pcSrc, iLenIn);
	if (0 != iErrorCode)
	{
		return -2;
	}

	uchardet_data_end(ud);

	pcDest = uchardet_get_charset(ud);

	//+1 ����һ���ַ�'\0'
	pcBuf = (char *)malloc(strlen(pcDest)+1);
	if (NULL == pcBuf)
	{
		return -3;
	}
	memset(pcBuf, 0, strlen(pcDest)+1);

	strcpy(pcBuf,pcDest);

	*pcCode = pcBuf;

	uchardet_delete(ud);

	return 0;
}

/********************************************************
   Func Name: getUTF8charSize
Date Created: 2018-8-29
 Description: �������ֽ�,��ȡutf8�ַ���ռ�ֽ���
       Input: ch��utf8���ֽ�
      Output:         
      Return: utf8�ַ���ռ�ֽ���
     Caution: 
*********************************************************/
int getUTF8charSize(unsigned char ch)
{
	int iSize = 0;

	if (ch >= 0xFC && ch < 0xFE)
	{
		iSize = 6;
	}
	else if (ch >= 0xF8)
	{
		iSize = 5;
	}
	else if (ch >= 0xF0)
	{
		iSize = 4;
	}
	else if (ch >= 0xE0)
	{
		iSize = 3;
	}
	else if (ch >= 0xC0)
	{
		iSize = 2;
	}
	else if (0 == (ch & 0x80))
	{
		iSize = 1;
	}
	else
	{
		iSize = 0;
	}

	return iSize;
}



