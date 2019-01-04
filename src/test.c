
#ifdef TEST

#include "common/paramhelper.h"
#include "common/stringhelper.h"
#include "common/crypto_des.h"
#include "common/timehelper.h"
#include "common/crypto_aes.h"
#include "common/crypto_sha.h"
#include "common/regularhelper.h"
#include "common/base64.h"
#include "common/gtl_list.h"
#include "common/crypto_md5.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void test()
{
	int result = 0;
	const char *p = "hello world";
	char *data = NULL;

	result = md5_calculate((char *)p, strlen(p), &data);

	printf("result = [%d] ,md5 = %s \n",result, data);
}
int main(int argc,char * argv[])
{
	test();
	return 0;
}

#endif
