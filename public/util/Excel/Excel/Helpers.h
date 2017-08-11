/**************************************************************************
*   Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
*   FileName:       
*   Description:    
*   Author:         liyaya (liyaya@hikvision.com)
*   Date:           2009-11-10
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2009-11-10       liyaya         created
**************************************************************************/
// Helpers.h: interface for the CHelpers class.
//

#if !defined(AFX_HELPERS_H__DF0ED2F1_BED3_11D2_BDAA_204C4F4F5020__INCLUDED_)
#define AFX_HELPERS_H__DF0ED2F1_BED3_11D2_BDAA_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// CHelpers

class CHelpers
{
public:
	static CString CrackStrVariant(const COleVariant& var, BOOL bDateLen = FALSE);

	~CHelpers();
private:
	CHelpers();
};

#endif // !defined(AFX_HELPERS_H__DF0ED2F1_BED3_11D2_BDAA_204C4F4F5020__INCLUDED_)