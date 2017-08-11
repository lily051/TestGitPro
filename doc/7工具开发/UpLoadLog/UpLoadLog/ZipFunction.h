#pragma once

// 文件名: ZipFunction.h
#pragma once
#include "zip.h"

namespace ZipUtils
{
	// ------------------------------------------------------------------------------------------------------------------------
	// Summary:
	//   压缩指定路径下的文件，并保存压缩包到指定路径。
	// Parameters:
	//   lpszSrcPath        - 待压缩文件所在的路径; 如"D://00"。
	//   lpszDestPath       - 压缩完成后，存放压缩包的路径。
	//                        此参数省略时，默认存放路径为exe程序所在文件的路径。
	//   lpszZipName        - 压缩完成后，压缩的名称；如“MySkin.zip”。
	// Returns:
	//   压缩成功返回ZR_OK，压缩失败返回错误码。
	// ------------------------------------------------------------------------------------------------------------------------
	ZRESULT CompressDirToZip(LPCTSTR lpszSrcPath, LPCTSTR lpszZipName, LPCTSTR lpszDestPath = NULL);
}