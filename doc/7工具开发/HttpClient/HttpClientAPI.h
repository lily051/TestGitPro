#ifndef HTTP_CLIENT_API_H
#define HTTP_CLIENT_API_H
#include <string>
#ifdef HTTPCLIENT_DLL_EXPORTS
#define HTTP_API __declspec(dllexport)
#else
#define HTTP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif	
	/**
	* @brief 初始化
	* @param 无
	* @return 返回是否初始化成功
	*/
	HTTP_API int Http_Client_Init();
	/**
	* @brief 反初始化
	* @param 无
	* @return 返回是否反初始化成功
	*/
	HTTP_API int Http_Client_Fini();
	/**
	* @brief HTTP POST请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/
	HTTP_API int Http_Client_Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);

	/**
	* @brief HTTP GET请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/
	HTTP_API int Http_Client_Get(const std::string & strUrl, std::string & strResponse);

	/**
	* @brief HTTP 下载文件
	* @param strUrl 输入参数,请求的文件Url地址,如:http://10.10.48.101/randomCode.jpg
	* @param strFilePath 输出参数,返回的内容
	* @return 返回是否下载成功
	*/
	HTTP_API int Http_Client_Download(const std::string & strUrl, const std::string & strFilePath);

	/**
	* @brief HTTPS POST请求,无证书版本
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
	* @param strResponse 输出参数,返回的内容
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @return 返回是否Post成功
	*/
	HTTP_API int Http_Client_Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);

	/**
	* @brief HTTPS GET请求,无证书版本
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strResponse 输出参数,返回的内容
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @return 返回是否Post成功
	*/
	HTTP_API int Http_Client_Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);
#ifdef __cplusplus
}
#endif

#endif