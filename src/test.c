
#ifdef TEST

#include "gtl.h"
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_sha.h"
#include "regularhelper.h"
#include "crypto_aes.h"
#include "crypto_rsa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void test()
{
	int result = 0;
	//测试des

	//加密字符串
	char *p = "我爱祖国 .";

	//定义8个字节密钥
	unsigned char key[16] = { '1','2','3','4','a','b','c','d' };
	unsigned char *key1 = "0123456789abcdef";
	unsigned char *key2 = "0123456789abcdef";
	char *pcOut1 = NULL;
	int outLen1 = 0;
	char *pcOut2 = NULL;
	int outLen2 = 0;

	//加密
	result = rsa_encode("prikey.pem", p, strlen(p), &pcOut1, &outLen1);
	if (result)
	{
		printf("des_encode() error .\n");
		return;
	}

	printf("Ciphertext[%s] and size[%d] .\n", pcOut1, outLen1);

	//解密
	result = rsa_decode("yntcom.de", pcOut1, outLen1, &pcOut2, &outLen2);
	if (result)
	{
		printf("des_decode() error .\n");
		return;
	}

	printf("text[%s] and size[%d] .\n", pcOut2, outLen2);

}

int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
