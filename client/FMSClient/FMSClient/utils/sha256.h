#ifndef _SHA256_H_ 
#define _SHA256_H_ 

/*
�����ַ���SHA-256
����˵����
str         �ַ���ָ��
length      �ַ�������
sha256      ���ڱ���SHA-256���ַ���ָ�룬����Ӧ>=64
����ֵΪ����sha256
*/
char* SHA256(const char* str, long length, char* sha256);

#endif