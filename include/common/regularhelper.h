
#ifndef __REGULARHELPER_H_
#define __REGULARHELPER_H_

#define PCRE_STATIC  //��̬�����ѡ�� 

//�û�������
typedef void(*FuncHandle)(char *pcData, int *regVector, size_t size, void *userArg);

#ifdef __cplusplus
extern "C"
{
#endif
	//������ʽʶ��
	int regularInfer(char *pcSrc, const char *pattern, void *userData, FuncHandle callback);

#ifdef __cplusplus
}
#endif

#endif







