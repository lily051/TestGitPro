#pragma once

// �ļ���: ZipFunction.h
#pragma once
#include "zip.h"

namespace ZipUtils
{
	// ------------------------------------------------------------------------------------------------------------------------
	// Summary:
	//   ѹ��ָ��·���µ��ļ���������ѹ������ָ��·����
	// Parameters:
	//   lpszSrcPath        - ��ѹ���ļ����ڵ�·��; ��"D://00"��
	//   lpszDestPath       - ѹ����ɺ󣬴��ѹ������·����
	//                        �˲���ʡ��ʱ��Ĭ�ϴ��·��Ϊexe���������ļ���·����
	//   lpszZipName        - ѹ����ɺ�ѹ�������ƣ��硰MySkin.zip����
	// Returns:
	//   ѹ���ɹ�����ZR_OK��ѹ��ʧ�ܷ��ش����롣
	// ------------------------------------------------------------------------------------------------------------------------
	ZRESULT CompressDirToZip(LPCTSTR lpszSrcPath, LPCTSTR lpszZipName, LPCTSTR lpszDestPath = NULL);
}