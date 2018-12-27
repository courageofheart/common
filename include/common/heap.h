
#ifndef __HEAP_H_
#define __HEAP_H_

/* �ѽṹ */

#ifdef __cplusplus
extern "C"
{

#endif

	typedef struct tagSTHeapHead
	{
		void **pstRoot;                 //����������
		int size;                       //�ṹ��С
		int used;                       //�Ѿ�ʹ�õĿռ�
	}head_t;

	typedef int(*CompareFunc)(void *, void *);

	typedef void(*ReleaseFunc)(void *);

	//���·����ڴ�ռ�
	int heap_reserve(head_t *pstHead, int iSize);

	//��ѹ��һ��Ԫ��
	int heap_push(head_t *pstHead, void *pvData, CompareFunc func);

	//�ж϶��Ƿ�Ϊ��
	int head_empty(head_t *pstHead);

	//�ѵ��ͷ�
	int head_release(head_t *pstHead, ReleaseFunc func);

	//��ȡ�Ѷ�Ԫ��
	void *heap_top(head_t *pstHead);

	//�����Ѷ�Ԫ��
	void *heap_pop(head_t *pstHead, CompareFunc func);

	//�ѵ����
	int head_clear(head_t *pstHead);

	//typedef void(*ShowFunc)(void *);

	//void test_show(head_t *pstHead, ShowFunc func);


#ifdef __cplusplus
}
#endif 

#endif



