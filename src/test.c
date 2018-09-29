
#ifdef TEST

#include "gtl.h"
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_aes.h"
#include "doublelink.h"
#include "gtlqueue.h"
#include "crypto_sha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test()
{
	char *p = NULL;
	char str[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	sha512_encode((unsigned char *)str,&p);
	printf("data is %s\n", p);
}

int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
