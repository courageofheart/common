

#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#include "ucharcode.h"
#include "uchardet.h"


/********************************************************
   Func Name: transcodeToUTF8
Date Created: 2018-8-3
 Description: 转码UTF8
       Input: pcSrc：源编码数据
	          lenIn：源编码长度
			 pcDest：结果存放内存地址（INOUT）
			 lenOut：剩余内存单位个数（INOUT）
	     pcCodeType：源编码类型
      Output:         
      Return: error code
     Caution: pcDest内存需要由调用函数分配
*********************************************************/
int fromcodeToCode(char *pcSrc, int lenIn, char *pcDest, int *lenOut, const char *pcFromCode, const char *pcToCode)
{
	int iRet = 0;

	//由于iconv()函数会修改指针，所以要保存源指针
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
	   *@param cd iconv_open()产生的句柄
	   *@param pcStart 需要转换的字符串
	   *@param inbytesleft 存放还有多少字符没有转换
	   *@param pcOut 存放转换后的字符串
	   *@param outlen 存放转换后,pcOut剩余的空间
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
 Description: 转码UTF8
       Input: pcSrc：源编码数据
	          lenIn：源编码长度
			 pcDest：结果存放内存地址（INOUT）
			 lenOut：剩余内存单位个数（INOUT）
	     pcCodeType：源编码类型
      Output:         
      Return: error code
     Caution: pcDest内存需要由调用函数分配
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
 Description: 转码GBK
       Input: pcSrc：源编码数据
	          lenIn：源编码长度
			 pcDest：结果存放内存地址（INOUT）
			 lenOut：剩余内存单位个数（INOUT）
	     pcCodeType：源编码类型
      Output:         
      Return: error code
     Caution: pcDest内存需要由调用函数分配
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
 Description: 获取字符串编码
       Input: pcSrc：源编码数据
	         iLenIn：源编码长度
			 pcCode：结果存放内存地址
      Output:         
      Return: error code
     Caution: pcDest内存需要由调用函数释放
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

	//如果样本字符不够，那么有可能导致分析失败
	iErrorCode = uchardet_handle_data(ud, pcSrc, iLenIn);
	if (0 != iErrorCode)
	{
		return -2;
	}

	uchardet_data_end(ud);

	pcDest = uchardet_get_charset(ud);

	//+1 多留一个字符'\0'
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
 Description: 根据首字节,获取utf8字符所占字节数
       Input: ch：utf8首字节
      Output:         
      Return: utf8字符所占字节数
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



