/*
*	SDK: 2015年6月25日
*
*	实现功能：请参照函数说明
*
*/


#pragma once

#ifdef OSDRTCDLL_EXPORTS
#define OSDRTCDLL extern "C" __declspec(dllexport)
#else 
#define OSDRTCDLL extern "C" __declspec(dllimport)
#endif

/*
*	同步OSD时间
*
*	[in]param pBaseFilter: 指向USB摄像机的IBaseFilter指针
*
*/
OSDRTCDLL bool H264_XU_SetOSDRTC(IBaseFilter *pBaseFilter);      // 同步OSD时间


/*
*	实现OSD第二行字符串(最大支持长度19(最好在传参数的时候进行长度判断)，超过无效),当前只支持字母和数字。
*		
*	[in]param pBaseFilter: 指向USB摄像机的IBaseFilter指针
*   [in]param Group: 写OSD第二行标志（一般为0x00,0x01,0x02）
*	[in]param OSDString[]:待添加的字符
*
*	return: false 失败
*	 		true 成功
*
*
*	调用方法： if (!H264_XU_SetOSD2ndString(pBaseFilter, 0x00, OSDString) ||
				!H264_XU_SetOSD2ndString(pBaseFilter, 0x01, OSDString+8) ||
				!H264_XU_SetOSD2ndString(pBaseFilter, 0x02, OSDString+16))
					{
					失败
					}
					else 
					{
					成功
					}
*
*/
OSDRTCDLL bool H264_XU_SetOSD2ndString(IBaseFilter* pBaseFilter, int Group, TCHAR OSDString[]);