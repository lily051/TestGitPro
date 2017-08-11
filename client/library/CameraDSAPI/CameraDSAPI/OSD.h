/*
*	SDK: 2015��6��25��
*
*	ʵ�ֹ��ܣ�����պ���˵��
*
*/


#pragma once

#ifdef OSDRTCDLL_EXPORTS
#define OSDRTCDLL extern "C" __declspec(dllexport)
#else 
#define OSDRTCDLL extern "C" __declspec(dllimport)
#endif

/*
*	ͬ��OSDʱ��
*
*	[in]param pBaseFilter: ָ��USB�������IBaseFilterָ��
*
*/
OSDRTCDLL bool H264_XU_SetOSDRTC(IBaseFilter *pBaseFilter);      // ͬ��OSDʱ��


/*
*	ʵ��OSD�ڶ����ַ���(���֧�ֳ���19(����ڴ�������ʱ����г����ж�)��������Ч),��ǰֻ֧����ĸ�����֡�
*		
*	[in]param pBaseFilter: ָ��USB�������IBaseFilterָ��
*   [in]param Group: дOSD�ڶ��б�־��һ��Ϊ0x00,0x01,0x02��
*	[in]param OSDString[]:����ӵ��ַ�
*
*	return: false ʧ��
*	 		true �ɹ�
*
*
*	���÷����� if (!H264_XU_SetOSD2ndString(pBaseFilter, 0x00, OSDString) ||
				!H264_XU_SetOSD2ndString(pBaseFilter, 0x01, OSDString+8) ||
				!H264_XU_SetOSD2ndString(pBaseFilter, 0x02, OSDString+16))
					{
					ʧ��
					}
					else 
					{
					�ɹ�
					}
*
*/
OSDRTCDLL bool H264_XU_SetOSD2ndString(IBaseFilter* pBaseFilter, int Group, TCHAR OSDString[]);