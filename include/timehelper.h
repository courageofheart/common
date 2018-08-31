
#ifndef __TIMEHELPER_H_
#define __TIMEHELPER_H_

#include <time.h>

/* 时间相关函数 */

#ifdef __cplusplus
extern "C"
{
#endif

	//时间字符串转时间戳
	time_t timestrToNumber(const char *pcTimestr);

	//获取当前时间戳
	time_t getCurrentTime();

	//获取随机数
	int getRandomNumber(int iRange);

	//获取UUID
	char * getUUID();

#ifdef __cplusplus
}
#endif

#endif

