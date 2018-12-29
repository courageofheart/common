
#ifndef __HEAP_H_
#define __HEAP_H_

#include <stdint.h>

/* �ѽṹ */

/*
int (*CompareFunc)(void *left, void *right);���ص�����˵��

���left>right������true����ʾ����
���left<right������true����ʾ��С��
���ѻ�����С�ѿ��Կ�heap_shift_down������ʵ��

*/

#ifdef __cplusplus
extern "C"
{

#endif

	typedef struct tagSTHeapHead
	{
		void **pstRoot;                 //����������
		int size;                       //�ṹ��С
		int used;                       //�Ѿ�ʹ�õĿռ䣨����������һ���ڵ���±꣬������stl�е�end()������
	}STHeapHead;

	typedef int(*CompareFunc)(void *, void *);

	typedef void(*ReleaseFunc)(void *);

	//��ʼ��
	STHeapHead * heap_init(uint32_t iSize);

	//���·����ڴ�ռ�
	int heap_reserve(STHeapHead *pstHead, uint32_t iSize);

	//��ѹ��һ��Ԫ��
	int heap_push(STHeapHead *pstHead, void *pvData, CompareFunc func);

	//�ж϶��Ƿ�Ϊ��
	int heap_empty(STHeapHead *pstHead);

	//�ѵ��ͷ�
	int heap_release(STHeapHead *pstHead, ReleaseFunc func);

	//��ȡ�Ѷ�Ԫ��
	void *heap_top(STHeapHead *pstHead);

	//�����Ѷ�Ԫ��
	void *heap_pop(STHeapHead *pstHead, CompareFunc func);

	//�ѵ����
	int heap_clear(STHeapHead *pstHead);

	typedef void(*doFunc)(void *);
	//�ѵı���
	void heap_foreach(STHeapHead *pstHead, doFunc cb);


#ifdef __cplusplus
}
#endif 

#endif



