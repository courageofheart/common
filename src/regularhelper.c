
#include <stdlib.h>
#include <string.h>

#include "common/regularhelper.h"
#include "pcre/pcre.h"
#include "common/stringhelper.h"

int regularLoop(pcre *re, char *pcSrc, size_t ovecCount, void *userData, FuncHandle callback);

/********************************************************
   Func Name: regularInfer
Date Created: 2018-9-29
 Description: pcreʶ��
	   Input: 
	  Output: error code
     Caution: 
*********************************************************/
int regularInfer(char *pcSrc, const char *pattern, void *userData, FuncHandle callback)
{
	int result = 0;
	pcre *re = NULL;
	int erroffset;
	const char *pcError = NULL;
	size_t ovecCount = 0;

	if (NULL == pcSrc || NULL == pattern || NULL == userData ||NULL == callback)
	{
		return -1;
	}

	/*
	�Ӵ��ĸ�������������ʽ��()�ĸ���
	*/
	//�����Ӵ��ĸ���
	ovecCount = matchOperator(pattern, '(', ')') + 1;
	
	//����������ʽ��pcre�ڲ���ʾ�ṹ
	re = pcre_compile(pattern, 0, &pcError, &erroffset, NULL);
	if (NULL == re)
	{
		return -1;
	}

	//ƥ���ַ���
	result = regularLoop(re, pcSrc, ovecCount, userData, callback);

	//������Դ
	pcre_free(re);

	return result;
}

/********************************************************
   Func Name: regularLoop
Date Created: 2018-9-29
 Description: ѭ��ʶ���ַ���
	   Input: 
	  Output: error code
     Caution: 
*********************************************************/
int regularLoop(pcre *re, char *pcSrc, size_t ovecCount, void *userData, FuncHandle callback)
{
	int rc  = 0;
	int *offsetVector = NULL;
	int offset = 0;
	int ovecSize = 0;
	int match = 0;
	
	//�����ڴ�ռ�
	//ovecSize should be a multiple of 3
	ovecSize = ovecCount * 3;
	offsetVector = (int *)malloc(ovecSize * sizeof(int));
	if (NULL == offsetVector)
	{
		return -1;
	}
	memset(offsetVector, 0, ovecSize * sizeof(int));

	while (1)
	{
		//ƥ��������ʽ
		////offsetΪƫ������Ϊ��ѭ��ƥ��
		rc = pcre_exec(re, NULL, pcSrc, strlen(pcSrc), offset, 0, offsetVector, ovecSize);
		if (rc <= 0)
		{
			break;
		}
		//�û��Դ���
		callback(pcSrc, offsetVector, rc, userData);
		//ƫ������ֵ
		offset = offsetVector[1];
		match++;
	}

	return match > 0 ? 0 : -1;

}








