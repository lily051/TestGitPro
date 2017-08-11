/**************************************************************************
*   Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
*   FileName:       Helpers.cpp    
*   Description:    implementation of the CHelpers class
*   Author:         liyaya (liyaya@hikvision.com)
*   Date:           2009-11-10
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2009-11-10       liyaya         created
**************************************************************************/

#include "stdafx.h"
#include "Helpers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CHelpers

CHelpers::CHelpers()
{
}

CHelpers::~CHelpers()
{
}

// CrackStrVariant() taken from the CCrack::strVariant method found in the
// DAOVIEW sample that ships with VC 5.0/6.0
CString CHelpers::CrackStrVariant(const COleVariant& var, BOOL bDateLen)
{
    CString strRet;
    strRet = _T("<Unknown>");
    switch(var.vt)
	{
		case VT_EMPTY:
			strRet = _T("");
			break;
		case VT_NULL:
			strRet = _T("");
			break;
		case VT_I2:
			strRet.Format(_T("%hd"), V_I2(&var));
			break;
		case VT_I4:
			strRet.Format(_T("%d"),V_I4(&var));
			break;
		case VT_R4:
			strRet.Format(_T("%e"), (double)V_R4(&var));
			break;
		case VT_R8:
			strRet.Format(_T("%.2f"), V_R8(&var));
			break;
		case VT_CY:
			strRet = COleCurrency(var).Format();
			break;
		case VT_DATE: {
			COleDateTime	oldDate(var);
			strRet	=	oldDate.Format(VAR_DATEVALUEONLY);
			CString	strTime = oldDate.Format(VAR_TIMEVALUEONLY);
			if(bDateLen && !strTime.IsEmpty())	strRet += _T(" ") + strTime;
			}break;
		case VT_BSTR:
			strRet = V_BSTR(&var);
			break;
		case VT_DECIMAL:
			strRet.Format(_T("%d"), var.lVal);
			break;
		case VT_DISPATCH:
			strRet = _T("VT_DISPATCH");
			break;
		case VT_ERROR:
			strRet = _T("VT_ERROR");
			break;
		case VT_BOOL:
			return ( V_BOOL(&var) ? _T("TRUE") : _T("FALSE"));
		case VT_VARIANT:
			strRet = _T("VT_VARIANT");
			break;
		case VT_UNKNOWN:
			strRet = _T("VT_UNKNOWN");
			break;
		case VT_I1:
			strRet = _T("VT_I1");
			break;
		case VT_UI1:
			strRet.Format(_T("0x%02hX"), (unsigned short)V_UI1(&var));
			break;
		case VT_UI2:
			strRet = _T("VT_UI2");
			break;
		case VT_UI4:
			strRet = _T("VT_UI4");
			break;
		case VT_I8:
			strRet = _T("VT_I8");
			break;
		case VT_UI8:
			strRet = _T("VT_UI8");
			break;
		case VT_INT:
			strRet = _T("VT_INT");
			break;
		case VT_UINT:
			strRet = _T("VT_UINT");
			break;
		case VT_VOID:
			strRet = _T("VT_VOID");
			break;
		case VT_HRESULT:
			strRet = _T("VT_HRESULT");
			break;
		case VT_PTR:
			strRet = _T("VT_PTR");
			break;
		case VT_SAFEARRAY:
			strRet = _T("VT_SAFEARRAY");
			break;
		case VT_CARRAY:
			strRet = _T("VT_CARRAY");
			break;
		case VT_USERDEFINED:
			strRet = _T("VT_USERDEFINED");
			break;
		case VT_LPSTR:
			strRet = _T("VT_LPSTR");
			break;
		case VT_LPWSTR:
			strRet = _T("VT_LPWSTR");
			break;
		case VT_FILETIME:
			strRet = _T("VT_FILETIME");
			break;
		case VT_BLOB:
			strRet = _T("VT_BLOB");
			break;
		case VT_STREAM:
			strRet = _T("VT_STREAM");
			break;
		case VT_STORAGE:
			strRet = _T("VT_STORAGE");
			break;
		case VT_STREAMED_OBJECT:
			strRet = _T("VT_STREAMED_OBJECT");
			break;
		case VT_STORED_OBJECT:
			strRet = _T("VT_STORED_OBJECT");
			break;
		case VT_BLOB_OBJECT:
			strRet = _T("VT_BLOB_OBJECT");
			break;
		case VT_CF:
			strRet = _T("VT_CF");
			break;
		case VT_CLSID:
			strRet = _T("VT_CLSID");
			break;
	}
    
	WORD vt = var.vt;
    if(vt & VT_ARRAY)
	{
        vt = vt & ~VT_ARRAY;
        strRet = _T("Array of ");
    }
    
	if(vt & VT_BYREF)
	{
        vt = vt & ~VT_BYREF;
        strRet += _T("Pointer to ");
    }
    
	if(vt != var.vt)
	{
        switch(vt)
		{
			case VT_EMPTY:
				strRet += _T("VT_EMPTY");
				break;
			case VT_NULL:
				strRet += _T("VT_NULL");
				break;
			case VT_I2:
				strRet += _T("VT_I2");
				break;
			case VT_I4:
				strRet += _T("VT_I4");
				break;
			case VT_R4:
				strRet += _T("VT_R4");
				break;
			case VT_R8:
				strRet += _T("VT_R8");
				break;
			case VT_CY:
				strRet += _T("VT_CY");
				break;
			case VT_DATE:
				strRet += _T("VT_DATE");
				break;
			case VT_BSTR:
				strRet += _T("VT_BSTR");
				break;
			case VT_DISPATCH:
				strRet += _T("VT_DISPATCH");
				break;
			case VT_ERROR:
				strRet += _T("VT_ERROR");
				break;
			case VT_BOOL:
				strRet += _T("VT_BOOL");
				break;
			case VT_VARIANT:
				strRet += _T("VT_VARIANT");
				break;
			case VT_UNKNOWN:
				strRet += _T("VT_UNKNOWN");
				break;
			case VT_I1:
				strRet += _T("VT_I1");
				break;
			case VT_UI1:
				strRet += _T("VT_UI1");
				break;
			case VT_UI2:
				strRet += _T("VT_UI2");
				break;
			case VT_UI4:
				strRet += _T("VT_UI4");
				break;
			case VT_I8:
				strRet += _T("VT_I8");
				break;
			case VT_UI8:
				strRet += _T("VT_UI8");
				break;
			case VT_INT:
				strRet += _T("VT_INT");
				break;
			case VT_UINT:
				strRet += _T("VT_UINT");
				break;
			case VT_VOID:
				strRet += _T("VT_VOID");
				break;
			case VT_HRESULT:
				strRet += _T("VT_HRESULT");
				break;
			case VT_PTR:
				strRet += _T("VT_PTR");
				break;
			case VT_SAFEARRAY:
				strRet += _T("VT_SAFEARRAY");
				break;
			case VT_CARRAY:
				strRet += _T("VT_CARRAY");
				break;
			case VT_USERDEFINED:
				strRet += _T("VT_USERDEFINED");
				break;
			case VT_LPSTR:
				strRet += _T("VT_LPSTR");
				break;
			case VT_LPWSTR:
				strRet += _T("VT_LPWSTR");
				break;
			case VT_FILETIME:
				strRet += _T("VT_FILETIME");
				break;
			case VT_BLOB:
				strRet += _T("VT_BLOB");
				break;
			case VT_STREAM:
				strRet += _T("VT_STREAM");
				break;
			case VT_STORAGE:
				strRet += _T("VT_STORAGE");
				break;
			case VT_STREAMED_OBJECT:
				strRet += _T("VT_STREAMED_OBJECT");
				break;
			case VT_STORED_OBJECT:
				strRet += _T("VT_STORED_OBJECT");
				break;
			case VT_BLOB_OBJECT:
				strRet += _T("VT_BLOB_OBJECT");
				break;
			case VT_CF:
				strRet += _T("VT_CF");
				break;
			case VT_CLSID:
				strRet += _T("VT_CLSID");
				break;
        }
    }
    
	return strRet;
}
