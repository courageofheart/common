
#ifndef __REGULARHELPER_H_
#define __REGULARHELPER_H_

#define PCRE_STATIC  //静态库编译选项 

//用户处理函数
typedef void(*FuncHandle)(char *pcData, int *regVector, size_t size, void *userArg);

#ifdef __cplusplus
extern "C"
{
#endif
	//正则表达式识别
	int regularInfer(char *pcSrc, const char *pattern, void *userData, FuncHandle callback);

#ifdef __cplusplus
}
#endif

#endif







