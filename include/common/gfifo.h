
#ifndef __GTC_FIFO_H_
#define __GTC_FIFO_H_

/* ���� */

#ifdef __cplusplus
extern "C"
{
#endif

	//����ص�����
	typedef void(*gtc_fifo_del_fn) (void *data);

	typedef struct {
		void **array;
		unsigned int size;      //����(��λ��Ԫ��) ���������С
		unsigned int capacity;      //����(��λ��Ԫ��) ���������С
		unsigned int in;    /* data is added at offset (in % size) */
		unsigned int out;    /* data is extracted from off. (out % size) */
		gtc_fifo_del_fn del;
	} gtc_fifo_t;

	//��ʼ��
	gtc_fifo_t * gtc_fifo_new(unsigned int size, gtc_fifo_del_fn del);

	//�ͷ�
	void gtc_fifo_del(gtc_fifo_t * fifo);

	//���
	int gtc_fifo_put(gtc_fifo_t *fifo, void * data);

	//����
	int gtc_fifo_pop(gtc_fifo_t *fifo, void ** data);

	//��ͷ
	const void * gtc_fifo_front(gtc_fifo_t *fifo);

#define GTC_FIFO_LENGTH(fifo) (fifo->size)

#ifdef __cplusplus
}
#endif

#endif
