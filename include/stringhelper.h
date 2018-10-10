
#ifndef __STRINGHELPER_H_
#define __STRINGHELPER_H_

#include <stdlib.h>

/* 字符串处理 */

//注意：所有函数只用于字符串处理，非字符串使用将会报错
#ifdef __cplusplus
extern "C"
{
#endif
	//删除一行中指定的字符
	int removeLineCharacter(char * pcBuf, char cAppoint);

	//去除首尾空格
	int trim(char * pcSrc);

	//去除左边空格
	int ltrim(char * pcSrc);

	//去除右边空格
	int rtrim(char * pcSrc);

	//去除指定字符
	int removeCharacter(char * pcSrc, char cAppoint);

	//释放字符串指针数组
	void releaseGridMemory(char ***pcGrid);

	//替换指定字符
	void replaceCharacter(char *pcSrc, char cFrom, char cTo);

	//替换指定字符串
	void replaceString(char *pcSrc, const char *pcFrom, const char *pcTo);

	//运算符匹配性检测
	size_t matchOperator(const char *pcSrc, char lch, char rch);

	//字符转十六进制数字
	char char_to_hex(char ch);


#ifdef __cplusplus
}
#endif

#endif


