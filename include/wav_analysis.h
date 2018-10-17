
#ifndef __WAV_ANALYSIS_H_
#define __WAV_ANALYSIS_H_

#include <stdint.h>

/* wav文件头解析 */

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct tagSTWavReader
	{
		uint8_t * data;
		uint32_t channel;
		uint32_t sampleRate;
		uint32_t sampleBits;
		uint32_t pcmSize;
	}wav_reader_t;

	//初始化
	wav_reader_t * wav_reader_init(uint8_t *audio, uint32_t audio_size);

	//获取声道
	uint32_t get_channel(wav_reader_t *reader);

	//获取采样精度
	uint32_t get_samplebits(wav_reader_t *reader);

	//获取采样率
	uint32_t get_samplerate(wav_reader_t *reader);

	//获取pcm数据长度
	uint32_t get_pcm_size(wav_reader_t *reader);

	//获取pcm数据
	uint8_t * get_pcm_data(wav_reader_t * reader);

	//释放reader结构
	void wav_reader_close(wav_reader_t **reader);

#ifdef __cplusplus
}
#endif




#endif


