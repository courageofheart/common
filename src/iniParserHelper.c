#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iniParserHelper.h"

//double��������ľ������
#define ACCURACY 0.00001

/********************************************************
   Func Name: inipGetString
Date Created: 2018-7-3
 Description: ��ȡini�ļ����ַ�����ֵ
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:���ؽṹ��
      Return: ��
     Caution: 
*********************************************************/
static void inipGetString(IN dictionary * dicIni, IN const char * pcKey, OUT STIniParserResult *pstResult)
{
	if (NULL == dicIni || pcKey == NULL || NULL == pstResult)
	{
		return;
	}
	pstResult->pcStrResult = (char *)iniparser_getstring(dicIni, pcKey, NULL);
	if (NULL == pstResult->pcStrResult)
	{
		pstResult->iErrorCode = DATA_ERROR;
	}else
	{
		pstResult->iErrorCode = RESULT_OK;
	}
}

/********************************************************
   Func Name: inipGetInt
Date Created: 2018-7-3
 Description: ��ȡini�ļ���int���͵�ֵ
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:���ؽṹ��
      Return: ��
     Caution: 
*********************************************************/
static void inipGetInt(IN dictionary * dicIni, IN const char * pcKey, OUT STIniParserResult *pstResult)
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
		pstResult->iErrorCode = DATA_ERROR;
	}else
	{
		pstResult->iIntResult = iResCode;
		pstResult->iErrorCode = RESULT_OK;
	}
}

/********************************************************
   Func Name: inipGetDouble
Date Created: 2018-7-3
 Description: ��ȡini�ļ���double���͵�ֵ
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:���ؽṹ��
      Return: ��
     Caution: 
*********************************************************/
static void inipGetDouble(IN dictionary * dicIni, IN const char * pcKey, OUT STIniParserResult *pstResult)
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
		pstResult->iErrorCode = DATA_ERROR;
	}else
	{
		pstResult->dDoubleResult = iResCode;
		pstResult->iErrorCode = RESULT_OK;
	}
}

/********************************************************
   Func Name: inipGetBool
Date Created: 2018-7-3
 Description: ��ȡini�ļ���bool���͵�ֵ
       Input: dicIni: dictionary struct
              pcKey:key
      Output: pstResult:���ؽṹ��
      Return: ��
     Caution: 
*********************************************************/
static void inipGetBool(IN dictionary * dicIni, IN const char * pcKey, OUT STIniParserResult *pstResult)
{
	int iResCode = -1, iTestNum = -2;
	iResCode = iniparser_getboolean(dicIni, pcKey, iTestNum);
	if (iResCode == iTestNum)
	{
		pstResult->iErrorCode = DATA_ERROR;
	}else
	{
		pstResult->iBoolResult = iResCode;
		pstResult->iErrorCode = RESULT_OK;
	}
}

/********************************************************
   Func Name: getIniValue
Date Created: 2018-7-3
 Description: ��ȡini�ļ���ֵ
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
		pstResult->iErrorCode = PARAM_ERROR;
		return pstResult;
	}
	//��һ��":"��"/0"
	uiLen = strlen(pcSectionName) + strlen(pcKey) + 1 + 1;
	pcSectionKey = (char *)malloc(uiLen);
	if(NULL == pcSectionKey)
	{
		pstResult->iErrorCode = MALLOC_ERROR;
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
		pstResult->iErrorCode = DATA_ERROR;
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
 Description: ��ʼ��ini������
       Input: pcFileName : Name of the ini file to read
              dicIni     : dictionary object
      Output: 
      Return: errcode
     Caution: need call releaseIniParser()
*********************************************************/
int initIniParser(IN const char *pcFileName ,IN dictionary **dicIni)
{
	int iResultCode = DEFAULT_ERROR;

	if (NULL == pcFileName || NULL == dicIni)
	{
		iResultCode = PARAM_ERROR;
		return iResultCode;
	}
	
	if (access(pcFileName, F_OK))
	{
		iResultCode = FILE_NOTEXIST_ERROR;
		return iResultCode;
	}
	*dicIni = iniparser_load(pcFileName);
	if(NULL == *dicIni)
	{
		iResultCode = DEFAULT_ERROR;
		return iResultCode;
	}
	return RESULT_OK;
}

/********************************************************
   Func Name: releaseIniParser
Date Created: 2018-7-3
 Description: �ͷ�ini������
       Input: dicIni     : dictionary object
      Output: 
      Return: 
     Caution: 
*********************************************************/
void releaseIniParser(IN dictionary * dicIni)
{
	if (dicIni)
	{
		iniparser_freedict(dicIni);
	}
}

/********************************************************
   Func Name: getSectionCount
Date Created: 2018-7-4
 Description: ��ȡsection�ĸ���
       Input: dicIni           : dictionary object
	          iSenctionCount   :section numbers
      Output: 
      Return: error code
     Caution: 
*********************************************************/
int getSectionCount(IN dictionary *dicIni, IN int *iSenctionCount)
{
	int iResultCode = DEFAULT_ERROR;
	if (NULL == dicIni || NULL == iSenctionCount)
	{
		iResultCode = PARAM_ERROR;
		return iResultCode;
	}
	iResultCode = iniparser_getnsec(dicIni);
	if (-1 == iResultCode)
	{
		iResultCode = DATA_ERROR;
		return iResultCode;
	}
	*iSenctionCount = iResultCode;
	return RESULT_OK;
}

/********************************************************
   Func Name: getSectionByIndex
Date Created: 2018-7-4
 Description: ��ȡָ��λ�õ�section
       Input: dicIni           : dictionary object
	          n                : section numbers
      Output: pcResult         : section name
      Return: error code
     Caution: Do not free or modify the returned pcResult
*********************************************************/
int getSectionByIndex( IN dictionary * dicIni, IN unsigned int n, OUT char **pcResult)
{
	int iResultCode = DEFAULT_ERROR;
	char *pcTmp = NULL;
	if (NULL == dicIni || NULL == pcResult)
	{
		iResultCode = PARAM_ERROR;
		return iResultCode;
	}
	pcTmp = (char *)iniparser_getsecname(dicIni, (int)n);
	if (NULL == pcTmp)
	{
		iResultCode = DATA_ERROR;
		return iResultCode;
	}
	*pcResult = pcTmp;
	return RESULT_OK;
}

/********************************************************
   Func Name: getSectionKeyCount
Date Created: 2018-7-4
 Description: ��ȡsection��key�ĸ���
       Input: dicIni           : dictionary object
	          pcSectionName    : section name
      Output: n                : key numbers
      Return: error code
     Caution: 
*********************************************************/
int getSectionKeyCount( IN dictionary * dicIni, IN const char *pcSectionName, OUT unsigned int *n)
{
	int iResultCode = DEFAULT_ERROR;
	unsigned int num = 0;
	if (NULL == dicIni || NULL == pcSectionName || NULL == n)
	{
		iResultCode = PARAM_ERROR;
		return iResultCode;
	}
	num = (unsigned int)iniparser_getsecnkeys(dicIni,pcSectionName);
	if (0 == num)
	{
		iResultCode = DATA_ERROR;
		return iResultCode;
	}
	*n = num;
	return RESULT_OK;
}

/********************************************************
   Func Name: getSectionKeys
Date Created: 2018-7-4
 Description: ��ȡsection�ļ��ļ���
       Input: dicIni           : dictionary object
	          pcSectionName    : section name
    OutInput: pcKeys           : keys in a section of a dictionary
      Return: error code
     Caution: pcKeys�����к��������߽��з��䣬����pcKeys�����е�Ԫ�ص��ڴ治��Ҫ�ɵ������ͷ�
*********************************************************/
int getSectionKeys(IN dictionary * dicIni, IN const char *pcSectionName,INOUT char **pcKeys)
{
	int iResultCode = DEFAULT_ERROR;
	if (NULL == dicIni || NULL == pcSectionName || NULL == pcKeys)
	{
		iResultCode = PARAM_ERROR;
		return iResultCode;
	}
	iniparser_getseckeys(dicIni, pcSectionName, (const char **)pcKeys);
	return RESULT_OK;
}

/********************************************************
   Func Name: getIniValueExtend
Date Created: 2018-7-4
 Description: ��ȡָ��section:key���ַ���ֵ
       Input: dicIni           : dictionary object
	          pcSectionName    : section name
              pcKey            : key name
    OutInput: 
      Return: error code
     Caution: 
*********************************************************/
char * getIniValueExtend(IN dictionary * dicIni, IN const char *pcSectionName, IN const char *pcKey)
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
 Description: ��ȡini�ļ����ַ�����ֵ��section�Ѿ�ƴ����ɣ�
       Input: dicIni: dictionary struct
              pcKey:key
      Output: 
      Return: key��ֵ
     Caution: 
*********************************************************/
char * inipGetStringExtend(IN dictionary * dicIni, IN const char * pcKey)
{
	char *pcResult = NULL;
	if (NULL == dicIni || pcKey == NULL)
	{
		return NULL;
	}
	pcResult= (char *)iniparser_getstring(dicIni, pcKey, NULL);
	return pcResult;
}

