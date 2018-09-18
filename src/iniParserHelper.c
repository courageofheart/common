#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iniParserHelper.h"

//double类型允许的精度误差
#define ACCURACY 0.00001

/********************************************************
   Func Name: inipGetString
Date Created: 2018-7-3
 Description: 获取ini文件中字符串的值
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:返回结构体
      Return: 无
     Caution: 
*********************************************************/
static void inipGetString(dictionary *dicIni, const char *pcKey, STIniParserResult *pstResult)
{
	if (NULL == dicIni || pcKey == NULL || NULL == pstResult)
	{
		return;
	}
	pstResult->pcStrResult = (char *)iniparser_getstring(dicIni, pcKey, NULL);
	if (NULL == pstResult->pcStrResult)
	{
		pstResult->iErrorCode = -1;
	}else
	{
		pstResult->iErrorCode = 0;
	}
}

/********************************************************
   Func Name: inipGetInt
Date Created: 2018-7-3
 Description: 获取ini文件中int类型的值
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:返回结构体
      Return: 无
     Caution: 
*********************************************************/
static void inipGetInt(dictionary *dicIni, const char *pcKey, STIniParserResult *pstResult)
{
	int iResCode1 = 0, iResCode2 = 1, iResCode = 0;
	int iFlag = 0;
	iResCode = iniparser_getint(dicIni, pcKey, iResCode1);
	if (iResCode == iResCode1)
	{
		iFlag += 1;
	}
	iResCode = iniparser_getint(dicIni, pcKey, iResCode2);
	if (iResCode == iResCode1)
	{
		iFlag += 1;
	}
	if (iFlag > 1)
	{
		pstResult->iErrorCode = -1;
	}else
	{
		pstResult->iIntResult = iResCode;
		pstResult->iErrorCode = 0;
	}
}

/********************************************************
   Func Name: inipGetDouble
Date Created: 2018-7-3
 Description: 获取ini文件中double类型的值
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:返回结构体
      Return: 无
     Caution: 
*********************************************************/
static void inipGetDouble(dictionary *dicIni, const char *pcKey, STIniParserResult *pstResult)
{
	double iResCode1 = 0.0001, iResCode2 = 0.0002, iResCode = 0.0;
	int iFlag = 0;
	iResCode = iniparser_getdouble(dicIni, pcKey, iResCode1);
	if (iResCode >= iResCode1-ACCURACY && iResCode <= iResCode1+ACCURACY)
	{
		iFlag += 1;
	}
	iResCode = iniparser_getdouble(dicIni, pcKey, iResCode2);
	if (iResCode >= iResCode2-ACCURACY && iResCode <= iResCode2+ACCURACY)
	{
		iFlag += 1;
	}
	if (iFlag > 1)
	{
		pstResult->iErrorCode = -1;
	}else
	{
		pstResult->dDoubleResult = iResCode;
		pstResult->iErrorCode = 0;
	}
}

/********************************************************
   Func Name: inipGetBool
Date Created: 2018-7-3
 Description: 获取ini文件中bool类型的值
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:返回结构体
      Return: 无
     Caution: 
*********************************************************/
static void inipGetBool(dictionary *dicIni, const char *pcKey, STIniParserResult *pstResult)
{
	int iResCode = -1, iTestNum = -2;
	iResCode = iniparser_getboolean(dicIni, pcKey, iTestNum);
	if (iResCode == iTestNum)
	{
		pstResult->iErrorCode = -1;
	}else
	{
		pstResult->iBoolResult = iResCode;
		pstResult->iErrorCode = 0;
	}
}

/********************************************************
   Func Name: getIniValue
Date Created: 2018-7-3
 Description: 获取ini文件的值
       Input: dicIni        : dictionary struct
	          pcSectionName : section name
              pcKey         :key
              enValueType   : value type
      Output: 
      Return: result struct
     Caution: need free return value
*********************************************************/
STIniParserResult * getIniValue(dictionary * dicIni, const char *pcSectionName, const char *pcKey, EMIniPraseType enValueType)
{
	char *pcSectionKey = NULL;
	size_t uiLen = 0;
	STIniParserResult * pstResult = NULL;

	pstResult = malloc(sizeof(STIniParserResult));
	if(NULL == pstResult)
	{
		return NULL;
	}
	memset(pstResult, 0, sizeof(STIniParserResult));

	if (NULL == dicIni || NULL == pcSectionName || NULL == pcKey)
	{
		pstResult->iErrorCode = -1;
		return pstResult;
	}
	//多一个":"和"/0"
	uiLen = strlen(pcSectionName) + strlen(pcKey) + 1 + 1;
	pcSectionKey = (char *)malloc(uiLen);
	if(NULL == pcSectionKey)
	{
		pstResult->iErrorCode = -1;
		return pstResult;
	}
	memset(pcSectionKey, 0, uiLen);
	sprintf(pcSectionKey,"%s:%s",pcSectionName,pcKey);
	switch (enValueType)
	{
	case INIP_STR:
		inipGetString(dicIni, pcSectionKey, pstResult);
		break;
	case INIP_INT:
		inipGetInt(dicIni, pcSectionKey, pstResult);
		break;
	case INIP_DOUBLE:
		inipGetDouble(dicIni, pcSectionKey, pstResult);
		break;
	case INIP_BOOL:
		inipGetBool(dicIni, pcSectionKey, pstResult);
		break;
	default:
		pstResult->iErrorCode = -1;
		break;
	}
	if(pcSectionKey)
	{
		free(pcSectionKey);
		pcSectionKey = NULL;
	}
	return pstResult;
}

/********************************************************
   Func Name: initIniParser
Date Created: 2018-7-3
 Description: 初始化ini解析器
       Input: pcFileName : Name of the ini file to read
              dicIni     : dictionary object
      Output: 
      Return: errcode
     Caution: need call releaseIniParser()
*********************************************************/
int initIniParser(const char *pcFileName , dictionary **dicIni)
{
	int iResultCode = -1;

	if (NULL == pcFileName || NULL == dicIni)
	{
		iResultCode = -1;
		return iResultCode;
	}
	
	if (access(pcFileName, F_OK))
	{
		iResultCode = -1;
		return iResultCode;
	}
	*dicIni = iniparser_load(pcFileName);
	if(NULL == *dicIni)
	{
		iResultCode = -1;
		return iResultCode;
	}
	return 0;
}

/********************************************************
   Func Name: releaseIniParser
Date Created: 2018-7-3
 Description: 释放ini解析器
       Input: dicIni     : dictionary object
      Output: 
      Return: 
     Caution: 
*********************************************************/
void releaseIniParser( dictionary *dicIni)
{
	if (dicIni)
	{
		iniparser_freedict(dicIni);
	}
}

/********************************************************
   Func Name: getSectionCount
Date Created: 2018-7-4
 Description: 获取section的个数
       Input: dicIni           : dictionary object
	          iSenctionCount   :section numbers
      Output: 
      Return: error code
     Caution: 
*********************************************************/
int getSectionCount(dictionary *dicIni, int *iSenctionCount)
{
	int iResultCode = -1;
	if (NULL == dicIni || NULL == iSenctionCount)
	{
		iResultCode = -1;
		return iResultCode;
	}
	iResultCode = iniparser_getnsec(dicIni);
	if (-1 == iResultCode)
	{
		iResultCode = -1;
		return iResultCode;
	}
	*iSenctionCount = iResultCode;
	return 0;
}

/********************************************************
   Func Name: getSectionByIndex
Date Created: 2018-7-4
 Description: 获取指定位置的section
       Input: dicIni           : dictionary object
	          n                : section numbers
      Output: pcResult         : section name
      Return: error code
     Caution: Do not free or modify the returned pcResult
*********************************************************/
int getSectionByIndex( dictionary *dicIni, unsigned int n, char **pcResult)
{
	int iResultCode = -1;
	char *pcTmp = NULL;
	if (NULL == dicIni || NULL == pcResult)
	{
		iResultCode = -1;
		return iResultCode;
	}
	pcTmp = (char *)iniparser_getsecname(dicIni, (int)n);
	if (NULL == pcTmp)
	{
		iResultCode = -1;
		return iResultCode;
	}
	*pcResult = pcTmp;
	return 0;
}

/********************************************************
   Func Name: getSectionKeyCount
Date Created: 2018-7-4
 Description: 获取section下key的个数
       Input: dicIni           : dictionary object
	          pcSectionName    : section name
      Output: n                : key numbers
      Return: error code
     Caution: 
*********************************************************/
int getSectionKeyCount( dictionary *dicIni, const char *pcSectionName, unsigned int *n)
{
	int iResultCode = -1;
	unsigned int num = 0;
	if (NULL == dicIni || NULL == pcSectionName || NULL == n)
	{
		iResultCode = -1;
		return iResultCode;
	}
	num = (unsigned int)iniparser_getsecnkeys(dicIni,pcSectionName);
	if (0 == num)
	{
		iResultCode = -1;
		return iResultCode;
	}
	*n = num;
	return 0;
}

/********************************************************
   Func Name: getSectionKeys
Date Created: 2018-7-4
 Description: 获取section的键的集合
       Input: dicIni           : dictionary object
	          pcSectionName    : section name
    OutInput: pcKeys           : keys in a section of a dictionary
      Return: error code
     Caution: pcKeys必须有函数调用者进行分配，但是pcKeys数组中的元素的内存不需要由调用者释放
*********************************************************/
int getSectionKeys(dictionary *dicIni, const char *pcSectionName, char **pcKeys)
{
	int iResultCode = -1;
	if (NULL == dicIni || NULL == pcSectionName || NULL == pcKeys)
	{
		iResultCode = -1;
		return iResultCode;
	}
	iniparser_getseckeys(dicIni, pcSectionName, (const char **)pcKeys);
	return 0;
}

/********************************************************
   Func Name: getIniValueExtend
Date Created: 2018-7-4
 Description: 获取指定section:key的字符串值
       Input: dicIni           : dictionary object
	          pcSectionName    : section name
              pcKey            : key name
    OutInput: 
      Return: error code
     Caution: 
*********************************************************/
char * getIniValueExtend(dictionary *dicIni, const char *pcSectionName, const char *pcKey)
{
	STIniParserResult *pstData = NULL;
	char * pcResult = NULL;

	pstData = getIniValue(dicIni, pcSectionName, pcKey, INIP_STR);
	if (pstData)
	{
		pcResult = pstData->pcStrResult;
		free(pstData);
		pstData = NULL;
	}
	return pcResult;
}

/********************************************************
   Func Name: inipGetStringExtend
Date Created: 2018-7-3
 Description: 获取ini文件中字符串的值（section已经拼接完成）
       Input: dicIni: dictionary struct
              pcKey:key
      Output: 
      Return: key的值
     Caution: 
*********************************************************/
char * inipGetStringExtend(dictionary *dicIni, const char * pcKey)
{
	char *pcResult = NULL;
	if (NULL == dicIni || pcKey == NULL)
	{
		return NULL;
	}
	pcResult= (char *)iniparser_getstring(dicIni, pcKey, NULL);
	return pcResult;
}

