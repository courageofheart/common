
#ifndef __CRYPTO_SHA1_H_
#define __CRYPTO_SHA1_H_

#ifdef __cplusplus
extern "C"
{
#endif
	//sha1加密
	int sha1_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha224加密
	int sha224_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha256加密
	int sha256_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha384加密
	int sha384_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha512加密
	int sha512_encode(unsigned char *pcPlaintext, char **pcCiphertext);

#ifdef __cplusplus
}
#endif

#endif

