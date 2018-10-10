
#ifdef TEST

#include "gtl.h"
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_aes.h"
#include "crypto_sha.h"
#include "regularhelper.h"
#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test()
{
	int ret = 0;
	char *pcOut = NULL;
	const char *p = "ajJzajM1enhtdHYzNmltNQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	char *p2 = NULL;
	int len = 0;
	char md[] = {85,234,47,90,74,110,14,155,160,222,55,184,24,155,40,248,158,142,62,108};
	ret = base64_encode(md, 20, &pcOut, &len);
	printf("%d\n", ret);
	ret = sha_encode_hex(SHA_512, (unsigned char *)p, strlen(p), (unsigned char **)&p2);
	printf("%d\n", ret);
	printf("p2 = %s\n", p2);
}

int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
