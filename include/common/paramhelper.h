
#ifndef __PARAMHELPER_H_
#define __PARAMHELPER_H_

/* �����в������� */

#include "common/gtl_list.h"

typedef struct tagSTOptionNode
{
	STGTListNode stNode;
	char gcOption[64];
	char gcValue[256];
}STOptionNode;

typedef struct option stLongOption;


#ifdef __cplusplus
extern "C"
{
#endif

	//�����в�������
	STGTListHead *parseParam(int argc, char ** argv, const char *pcOptString, const stLongOption *pstLongOption);

	//�ͷ���Դ
	void releaseSource(STGTListHead *pstHead);

#ifdef __cplusplus
}
#endif

#endif

