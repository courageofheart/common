
#ifndef __CRYPTO_MD5_H_
#define __CRYPTO_MD5_H_

 #define MD5_LENTH 16

#ifdef __cplusplus
extern "C"
{
#endif

	//md5º∆À„
	int md5_calculate(char *plaintext, int plaintext_size, char **md5_str);

#ifdef __cplusplus
}
#endif


#endif



