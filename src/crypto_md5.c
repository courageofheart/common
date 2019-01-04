
#include "common/crypto_md5.h"
#include "common/openssl/md5.h"
#include "common/hex_code.h"

#include <stdlib.h>
#include <string.h>

/********************************************************
   Func Name: getEncryptKey
Date Created: 2018-9-5
 Description: 获取aes加密密钥
	   Input: pcUserKey：用户密码 
	             pstKey：AES密钥
	  Output:
      Return: error code
     Caution: pstKey需要由调用函数分配
*********************************************************/
int md5_calculate(char *plaintext, int plaintext_size, char **md5_str)
{
	int result = 0;
	MD5_CTX md5_ctx;
	unsigned char md5_buffer[MD5_LENTH] = { 0 };

	if (NULL == plaintext || 0 == plaintext_size || NULL == md5_str)
	{
		return -1;
	}

	//初始化MD5上下文结构
	MD5_Init(&md5_ctx);

	//刷新MD5，将文件连续数据分片放入进行MD5刷新。
	MD5_Update(&md5_ctx, plaintext, plaintext_size);

	//生成MD5数据
	MD5_Final(md5_buffer, &md5_ctx);

	//转成十六进制字符串
	result = hex_encode(md5_buffer, MD5_LENTH, md5_str);
	if (result)
	{
		return -1;
	}

	return 0;
}


