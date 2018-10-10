
#ifndef __CRYPTO_SHA1_H_
#define __CRYPTO_SHA1_H_

typedef enum enSHA_TYPE
{
	SHA_1,
	SHA_224,
	SHA_256,
	SHA_384,
	SHA_512,
}SHA_TYPE;

#ifdef __cplusplus
extern "C"
{
#endif

	//sha�����ַ����汾
	int sha_encode_hex(SHA_TYPE type
		, unsigned char *pcPlaintext
		, int iPlaintextLen
		, unsigned char **pcCiphertext);

	//sha����
	int sha_encode(SHA_TYPE type
		, unsigned char *pcPlaintext
		, int iPlaintextLen
		, unsigned char **pcCiphertext
		, int *piCiphertextLen);

	//sha1����
	int sha1_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen);

	//sha224����
	int sha224_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen);

	//sha256����
	int sha256_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen);

	//sha384����
	int sha384_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen);

	//sha512����
	int sha512_encode(unsigned char *pcPlaintext, int iPlaintextLen, unsigned char **pcCiphertext, int *piCiphertextLen);

#ifdef __cplusplus
}
#endif

#endif

