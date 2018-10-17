
#include "wav_analysis.h"

#include <stdlib.h>
#include <string.h>

//��ʼ��
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

	//�ļ�ͷ����
	memcpy(gcHead, audio, 36);
	reader->channel = (gcHead[23] << 8) + gcHead[22];
	reader->sampleRate = (gcHead[25] << 8) + gcHead[24];
	reader->sampleBits = gcHead[34];
	index += 36;

	//RIFF���ݿ���Ϣ��ȡ
	while (index < audio_size)
	{
		//��ȡID��Ϣ
		memset(gcHead, 0, 36);
		memcpy(gcHead, audio + index, 4);
		//�ж��Ƿ���data
		if ('d' == gcHead[0] && 'a' == gcHead[1] && 't' == gcHead[2] && 'a' == gcHead[3])
		{
			flag = 1;
		}
		index += 4;

		//��ȡ���ݿ鳤��
		memset(gcHead, 0, 36);
		memcpy(gcHead, audio + index, 4);
		data_size = gcHead[0] + (gcHead[1] << 8) + (gcHead[2] << 16) + (gcHead[3] << 24);
		index += 4;

		//��ȡ���ݿ�
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
			//���ͷ��ڴ���Ϣ������pcm����
			break;
		}
		//�ͷ��ڴ���Ϣ
		free(tmp_data);
		tmp_data = NULL;
		index += data_size;
	}
	reader->data = tmp_data;

	return reader;

}

//��ȡ����
uint32_t get_channel(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->channel;

}

//��ȡ��������
uint32_t get_samplebits(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->sampleBits;

}

//��ȡ������
uint32_t get_samplerate(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->sampleRate;

}

//��ȡpcm���ݳ���
uint32_t get_pcm_size(wav_reader_t *reader)
{
	if (NULL == reader)
	{
		return 0;
	}

	return reader->pcmSize;

}

//��ȡpcm����
uint8_t * get_pcm_data(wav_reader_t * reader)
{
	if (NULL == reader)
	{
		return NULL;
	}

	return reader->data;

}

//�ͷ�reader�ṹ
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



