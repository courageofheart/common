
#ifndef __CRYPTO_SHA1_H_
#define __CRYPTO_SHA1_H_

#ifdef __cplusplus
extern "C"
{
#endif
	//sha1����
	int sha1_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha224����
	int sha224_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha256����
	int sha256_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha384����
	int sha384_encode(unsigned char *pcPlaintext, char **pcCiphertext);

	//sha512����
	int sha512_encode(unsigned char *pcPlaintext, char **pcCiphertext);

#ifdef __cplusplus
}
#endif

#endif

