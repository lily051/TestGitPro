/** @file hak.h
 *  @note Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief ����/����
 *
 *  @author Hu Tonghai
 *  @date 2015/03/01
 *
 *  @note ��ʷ��¼��
 *  @note V1.0.0.0 ����
 */
#ifndef _HAKCPP_H__
#define _HAKCPP_H__

#if (defined(_WIN32) || defined(_WIN64))
#  ifdef LIBHAK_EXPORTS
#    define HAK_EXTERN __declspec(dllexport)
#  else
#    define HAK_EXTERN __declspec(dllimport)
#  endif
#  define HAK_API __stdcall
#else
#  define HAK_EXTERN 
#  define HAK_API 
#endif


#ifdef __cplusplus
extern "C"
{
#endif

    /** @fn int HAK_API Hak1(char* p1, char* p2, char *p3)
     *  @brief ��ʼ���⡣
     *  @param p1 [in] �̶�������ֵ 103 ��
     *  @param p2 [in] �̶������ַ��� "DATA" ��
     *  @param p3 [in] �̶����� NULL ��
     *  @return �ɹ�����ֵΪ0��
     *  @note HAK_EXTERN int HAK_API Hak_Init(103, "DATA", NULL);
     */
    HAK_EXTERN int HAK_API Hak1(char* p1, char* p2, char *p3);
    #define Hak_Init Hak1

    /** @fn int HAK_API Hak2(char* p1, char* p2, char *p3)
     *  @brief ����ʼ���⡣
     *  @param p1 [in] �̶����� NULL ��
     *  @param p2 [in] �̶����� NULL ��
     *  @param p3 [in] �̶����� NULL ��
     *  @return �ɹ�����ֵΪ0��
     *  @note HAK_EXTERN int HAK_API Hak_Uninit(NULL, NULL, NULL);
     */
    HAK_EXTERN int HAK_API Hak2(char* p1, char* p2, char *p3);
    #define Hak_Uninit Hak2

    /** @fn int HAK_API Hak5(char* p1, char* p2, char *p3)
     *  @brief ���ܡ�
     *  @param p1 [out] ���ɼ��ܺ���ַ����Ļ��������仺�����Ĵ�СӦ�����ٴ��ڵ���(p2�����ַ����ĳ��� / 16 + 1) * 32 + 1��
     *  @param p2 [in] �����ܵ������ַ�������Ҫ��'\0'��Ϊ�ַ�����������
     *  @param p3 [in, out] ����p1�������ĳ��ȣ�����p1�м��ܺ����Ч�ַ������ȣ�����������'\0'��������
     *  @return �ɹ�����ֵΪ0��
     *  @note HAK_EXTERN int HAK_API Hak_Encrypt(char* out_aes_hex_txt, char* in_txt, int* outl_aes_hex_txt);
     */
    HAK_EXTERN int HAK_API Hak5(char* p1, char* p2, char *p3);
    #define Hak_Encrypt Hak5

    /** @fn int HAK_API Hak7(char* p1, char* p2, char *p3)
     *  @brief ���ܡ�
     *  @param p1 [out] ���ɽ��ܺ���ַ����Ļ��������仺�����Ĵ�СӦ�����ٴ��ڵ��� p2�����ַ����ĳ��� / 32 * 16��
     *  @param p2 [in] �����ܵ������ַ�������Ҫ��'\0'��Ϊ�ַ�����������
     *  @param p3 [in, out] ����p1�������ĳ��ȣ�����p1�н��ܺ����Ч�ַ������ȣ�����������'\0'��������
     *  @return �ɹ�����ֵΪ0��
     *  @note HAK_EXTERN int HAK_API Hak_Decrypt(char* out_txt, char* in_aes_hex_txt, int* outl_txt);
     */
    HAK_EXTERN int HAK_API Hak7(char* p1, char* p2, char *p3);
    #define Hak_Decrypt Hak7

    /** @fn int HAK_API Hak9(char* p1, char* p2, char *p3)
     *  @brief �ж��ַ����Ƿ��Ѽ��ܡ�
     *  @param p1 [in] ������жϵ��ַ�������Ҫ��'\0'��Ϊ�ַ�����������
     *  @param p2 [in] �̶����� NULL ��
     *  @param p3 [in] �̶����� NULL ��
     *  @return ���Ѽ����򷵻�ֵΪ1�����򷵻�ֵΪ0��
     *  @note HAK_EXTERN int HAK_API Hak_IsEncrypted(char* in_txt, NULL, NULL);
     */
    HAK_EXTERN int HAK_API Hak9(char* p1, char* p2, char *p3);
    #define Hak_IsEncrypted Hak9

#ifdef __cplusplus
}
#endif

#endif
