
#ifndef __STRINGHELPER_H_
#define __STRINGHELPER_H_

#include <stdlib.h>

/* �ַ������� */

//ע�⣺���к���ֻ�����ַ����������ַ���ʹ�ý��ᱨ��
#ifdef __cplusplus
extern "C"
{
#endif
	//ɾ��һ����ָ�����ַ�
	int removeLineCharacter(char * pcBuf, char cAppoint);

	//ȥ����β�ո�
	int trim(char * pcSrc);

	//ȥ����߿ո�
	int ltrim(char * pcSrc);

	//ȥ���ұ߿ո�
	int rtrim(char * pcSrc);

	//ȥ��ָ���ַ�
	int removeCharacter(char * pcSrc, char cAppoint);

	//�ͷ��ַ���ָ������
	void releaseGridMemory(char ***pcGrid);

	//�滻ָ���ַ�
	void replaceCharacter(char *pcSrc, char cFrom, char cTo);

	//�滻ָ���ַ���
	void replaceString(char *pcSrc, const char *pcFrom, const char *pcTo);

	//�����ƥ���Լ��
	size_t matchOperator(const char *pcSrc, char lch, char rch);

	//�ַ�תʮ����������
	char char_to_hex(char ch);


#ifdef __cplusplus
}
#endif

#endif


