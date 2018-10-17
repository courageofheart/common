
#include "wav_analysis.h"

#include <stdlib.h>
#include <string.h>

//初始化
wav_reader_t * wav_reader_init(uint8_t *audio, uint32_t audio_size)
{
	wav_reader_t *reader = NULL;
	uint8_t gcHead[36] = { 0 };
	uint8_t *tmp_data = NULL;
	uint32_t data_size = 0;
	uint32_t index = 0;
	uint8_t flag = 0;

	if (NULL == audio || audio_size < 44)
	{
		return NULL;
	}

	reader = (wav_reader_t *)malloc(sizeof(wav_reader_t));
	if (NULL == reader)
	{
		return NULL;
	}
	memset(reader, 0, sizeof(wav_reader_t));

	//文件头解析
	memcpy(gcHead, audio, 36);
	reader->channel = (gcHead[23] << 8) + gcHead[22];
	reader->sampleRate = (gcHead[25] << 8) + gcHead[24];
	reader->sampleBits = gcHead[34];
	index += 36;

	//RIFF数据块信息读取
	while (index < audio_size)
	{
		//获取ID信息
		memset(gcHead, 0, 36);
		memcpy(gcHead, audio + index, 4);
		//判断是否是data
		if ('d' == gcHead[0] && 'a' == gcHead[1] && 't' == gcHead[2] && 'a' == gcHead[3])
		{
			flag = 1;
		}
		index += 4;

		//获取数据块长度
		memset(gcHead, 0, 36);
		memcpy(gcHead, audio + index, 4);
		data_size = gcHead[0] + (gcHead[1] << 8) + (gcHead[2] << 16) + (gcHead[3] << 24);
		index += 4;

		//获取数据块
		tmp_data = (uint8_t *)malloc(data_size);
		if (NULL == tmp_data)
		{
			return NULL;
		}
		memset(tmp_data, 0, data_size);
		memcpy(tmp_data, audio + index, data_size);
		//fread(pcData, data_size, 1, pfr);
		if (flag)
		{
			reader->pcmSize = data_size;
			//不释放内存信息，保留pcm数据
			break;
		}
		//释放内存信息
		free(tmp_data);
		tmp_data = NULL;
		index += data_size;
	}
	reader->data = tmp_data;

	return reader;

}

//获取声道
uint32_t get_channel(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->channel;

}

//获取采样精度
uint32_t get_samplebits(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->sampleBits;

}

//获取采样率
uint32_t get_samplerate(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->sampleRate;

}

//获取pcm数据长度
uint32_t get_pcm_size(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->pcmSize;

}

//获取pcm数据
uint8_t * get_pcm_data(wav_reader_t * reader)
{
	if (NULL == reader)
	{
		return NULL;
	}

	return reader->data;

}

//释放reader结构
void wav_reader_close(wav_reader_t **reader)
{
	wav_reader_t *obj = NULL;
	if (NULL == reader)
	{
		return;
	}

	obj = *reader;

	if (obj->data)
	{
		free(obj->data);
		obj->data = NULL;
	}
	if (obj)
	{
		free(obj);
		obj = NULL;
	}
	*reader = NULL;
}



