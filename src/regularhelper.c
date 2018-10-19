
#include <stdlib.h>
#include <string.h>

#include "common/regularhelper.h"
#include "pcre/pcre.h"
#include "common/stringhelper.h"

int regularLoop(pcre *re, char *pcSrc, size_t ovecCount, void *userData, FuncHandle callback);

/********************************************************
   Func Name: regularInfer
Date Created: 2018-9-29
 Description: pcre识别
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
	子串的个数就是正则表达式中()的个数
	*/
	//计算子串的个数
	ovecCount = matchOperator(pattern, '(', ')') + 1;
	
	//编译正则表达式的pcre内部表示结构
	re = pcre_compile(pattern, 0, &pcError, &erroffset, NULL);
	if (NULL == re)
	{
		return -1;
	}

	//匹配字符串
	result = regularLoop(re, pcSrc, ovecCount, userData, callback);

	//清理资源
	pcre_free(re);

	return result;
}

/********************************************************
   Func Name: regularLoop
Date Created: 2018-9-29
 Description: 循环识别字符串
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
	
	//分配内存空间
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
		//匹配正则表达式
		////offset为偏移量，为了循环匹配
		rc = pcre_exec(re, NULL, pcSrc, strlen(pcSrc), offset, 0, offsetVector, ovecSize);
		if (rc <= 0)
		{
			break;
		}
		//用户自处理
		callback(pcSrc, offsetVector, rc, userData);
		//偏移量赋值
		offset = offsetVector[1];
		match++;
	}

	return match > 0 ? 0 : -1;

}








