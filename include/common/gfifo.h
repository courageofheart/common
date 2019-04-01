
#ifndef __GTC_FIFO_H_
#define __GTC_FIFO_H_

/* 队列 */

#ifdef __cplusplus
extern "C"
{
#endif

	//清理回调函数
	typedef void(*gtc_fifo_del_fn) (void *data);

	typedef struct {
		void **array;
		unsigned int size;      //容量(单位：元素) 并非数组大小
		unsigned int capacity;      //容量(单位：元素) 并非数组大小
		unsigned int in;    /* data is added at offset (in % size) */
		unsigned int out;    /* data is extracted from off. (out % size) */
		gtc_fifo_del_fn del;
	} gtc_fifo_t;

	//初始化
	gtc_fifo_t * gtc_fifo_new(unsigned int size, gtc_fifo_del_fn del);

	//释放
	void gtc_fifo_del(gtc_fifo_t * fifo);

	//入队
	int gtc_fifo_put(gtc_fifo_t *fifo, void * data);

	//出队
	int gtc_fifo_pop(gtc_fifo_t *fifo, void ** data);

	//队头
	const void * gtc_fifo_front(gtc_fifo_t *fifo);

#define GTC_FIFO_LENGTH(fifo) (fifo->size)

#ifdef __cplusplus
}
#endif

#endif
