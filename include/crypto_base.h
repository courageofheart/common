

#ifndef __CRYPTO_BASE_H_
#define __CRYPTO_BASE_H_

/*
由于密文中可能存在/0 /r /n 这些特殊字符，不利于我们在网络中进行传输
所以将密文中的每一个字符转换成两个十六进制数字。
十六进制数字的范围是[0-9A-Z]，没有0这个数字，只有0这个字符
*/

#ifdef __cplusplus
extern "C"
{
#endif
	/********************************************************
	   Func Name: string_to_hex
	Date Created: 2018-10-10
	 Description: 字符串转十六进制字符
		   Input: 
		  Output:
		  Return:
		 Caution: 返回的pcDst是字符串
	*********************************************************/
	int string_to_hex(unsigned char *pcSrc, int inLen, char **pcDst);

	/********************************************************
	   Func Name: hex_to_string
	Date Created: 2018-10-10
	 Description: 十六进制字符串转字符串
		   Input: 
		  Output:
		  Return:
		 Caution: 返回的pcDst是字符数组，不是字符串
	*********************************************************/
	int hex_to_string(unsigned char *pcSrc, unsigned char **pcDst, int *outLen);

	/********************************************************
	   Func Name: char_to_hex
	Date Created: 2018-10-10
	 Description: 字符转十六进制
		   Input: 
		  Output:
		  Return: 十六进制数字
		 Caution: 例如(a =10,b=11)
	*********************************************************/
	char char_to_hex(char ch);

#ifdef __cplusplus
}
#endif

#endif







