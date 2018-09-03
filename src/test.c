
#include "paramhelper.h"
#include "stringhelper.h"
#include "crypto_des.h"
#include "timehelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>


void test()
{
	printf("time is %ld \n", getMonotonicTime());
}

int main(int argc,char * argv[])
{
	//test_param(argc,argv);
	//test_string();
	test();
	return 0;
}

