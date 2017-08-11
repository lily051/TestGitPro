/** @file hak.h
 *  @note Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief 加密/解密
 *
 *  @author Hu Tonghai
 *  @date 2015/03/01
 *
 *  @note 历史记录：
 *  @note V1.0.0.0 创建
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
     *  @brief 初始化库。
     *  @param p1 [in] 固定传入数值 103 。
     *  @param p2 [in] 固定传入字符串 "DATA" 。
     *  @param p3 [in] 固定传入 NULL 。
     *  @return 成功返回值为0。
     *  @note HAK_EXTERN int HAK_API Hak_Init(103, "DATA", NULL);
     */
    HAK_EXTERN int HAK_API Hak1(char* p1, char* p2, char *p3);
    #define Hak_Init Hak1

    /** @fn int HAK_API Hak2(char* p1, char* p2, char *p3)
     *  @brief 反初始化库。
     *  @param p1 [in] 固定传入 NULL 。
     *  @param p2 [in] 固定传入 NULL 。
     *  @param p3 [in] 固定传入 NULL 。
     *  @return 成功返回值为0。
     *  @note HAK_EXTERN int HAK_API Hak_Uninit(NULL, NULL, NULL);
     */
    HAK_EXTERN int HAK_API Hak2(char* p1, char* p2, char *p3);
    #define Hak_Uninit Hak2

    /** @fn int HAK_API Hak5(char* p1, char* p2, char *p3)
     *  @brief 加密。
     *  @param p1 [out] 容纳加密后的字符串的缓冲区，其缓冲区的大小应该至少大于等于(p2明文字符串的长度 / 16 + 1) * 32 + 1。
     *  @param p2 [in] 待加密的明文字符串，需要以'\0'作为字符串结束符。
     *  @param p3 [in, out] 传入p1缓冲区的长度，传出p1中加密后的有效字符串长度，不包括最后的'\0'结束符。
     *  @return 成功返回值为0。
     *  @note HAK_EXTERN int HAK_API Hak_Encrypt(char* out_aes_hex_txt, char* in_txt, int* outl_aes_hex_txt);
     */
    HAK_EXTERN int HAK_API Hak5(char* p1, char* p2, char *p3);
    #define Hak_Encrypt Hak5

    /** @fn int HAK_API Hak7(char* p1, char* p2, char *p3)
     *  @brief 解密。
     *  @param p1 [out] 容纳解密后的字符串的缓冲区，其缓冲区的大小应该至少大于等于 p2密文字符串的长度 / 32 * 16。
     *  @param p2 [in] 待解密的明文字符串，需要以'\0'作为字符串结束符。
     *  @param p3 [in, out] 传入p1缓冲区的长度，传出p1中解密后的有效字符串长度，不包括最后的'\0'结束符。
     *  @return 成功返回值为0。
     *  @note HAK_EXTERN int HAK_API Hak_Decrypt(char* out_txt, char* in_aes_hex_txt, int* outl_txt);
     */
    HAK_EXTERN int HAK_API Hak7(char* p1, char* p2, char *p3);
    #define Hak_Decrypt Hak7

    /** @fn int HAK_API Hak9(char* p1, char* p2, char *p3)
     *  @brief 判断字符串是否已加密。
     *  @param p1 [in] 传入待判断的字符串，需要以'\0'作为字符串结束符。
     *  @param p2 [in] 固定传入 NULL 。
     *  @param p3 [in] 固定传入 NULL 。
     *  @return 若已加密则返回值为1，否则返回值为0。
     *  @note HAK_EXTERN int HAK_API Hak_IsEncrypted(char* in_txt, NULL, NULL);
     */
    HAK_EXTERN int HAK_API Hak9(char* p1, char* p2, char *p3);
    #define Hak_IsEncrypted Hak9

#ifdef __cplusplus
}
#endif

#endif
