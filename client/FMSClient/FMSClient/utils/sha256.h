#ifndef _SHA256_H_ 
#define _SHA256_H_ 

/*
计算字符串SHA-256
参数说明：
str         字符串指针
length      字符串长度
sha256      用于保存SHA-256的字符串指针，长度应>=64
返回值为参数sha256
*/
char* SHA256(const char* str, long length, char* sha256);

#endif