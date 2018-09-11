
#ifdef TEST

#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"
#include "crypto_aes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void test()
{
	char p[] = "aaas#0ssddf#0dgaa#0afgd";
	replaceString((char *)p, "#0", "@1");
	printf("p = %s\n", p);

}

int main(int argc,char * argv[])
{
	//test_param(argc,argv);
	//test_string();
	test();
	return 0;
}

#endif
