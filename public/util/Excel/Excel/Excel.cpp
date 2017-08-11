#include "StdAfx.h"
#include "Excel.h"
#include "Helpers.h"
#include "CHyperlinks.h"

_tstring CExcel::StdtstringFormat(const _TCHAR *fmt,... ) 
{ 
    _tstring strResult;
    if (NULL != fmt)
    {
        va_list marker = NULL;            
        va_start(marker, fmt);                            //初始化变量参数

        size_t nLength = _vsctprintf(fmt, marker) + 1;    //获取格式化字符串长度

        _TCHAR* pBuffer = new(std::nothrow) _TCHAR[nLength];

        _vsntprintf_s(pBuffer, nLength, nLength, fmt, marker); 

        strResult = pBuffer;

        delete [] pBuffer;
        va_end(marker);                                    //重置变量参数
        marker = NULL;
    }

    return strResult; 
}

_tstring CrackStrVariant(const COleVariant& var)
{
    _tstring strRet = _T("Error");
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
        strRet = CExcel::StdtstringFormat(_T("%hd"), V_I2(&var));
        break;
    case VT_I4:
        strRet = CExcel::StdtstringFormat(_T("%d"),V_I4(&var));
        break;
    case VT_R4:
        strRet = CExcel::StdtstringFormat(_T("%e"), (double)V_R4(&var));
        break;
    case VT_R8:
        strRet = CExcel::StdtstringFormat(_T("%.2f"), V_R8(&var));
        break;
    case VT_CY:
        strRet = COleCurrency(var).Format();
        break;
    case VT_DATE: {
        COleDateTime	oldDate(var);
        strRet	=	oldDate.Format(VAR_DATEVALUEONLY);
        _tstring	strTime = oldDate.Format(VAR_TIMEVALUEONLY);
        if(!EXCEL_EMPTY(strTime))	strRet += _T(" ") + strTime;
                  }break;
    case VT_BSTR:
        strRet = EXCEL_BSTRToSTR(var.bstrVal);
        break;
    case VT_DECIMAL:
        strRet = CExcel::StdtstringFormat(_T("%d"), var.lVal);
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
        strRet = CExcel::StdtstringFormat(_T("0x%02hX"), (unsigned short)V_UI1(&var));
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
    default:
        break;
    }

    return strRet;
}
LONG CrackIntVariant(const COleVariant& var)
{
    LONG lValue = 0;
    switch(var.vt)
    {
    case VT_EMPTY:
        lValue = 0;;
        break;
    case VT_NULL:
        lValue = 0;
        break;
    case VT_I2:
        lValue = var.iVal;
        break;
    case VT_UI1:
        lValue = var.bVal;
        break;
    case VT_I4:
        lValue = var.lVal;
        break;
    case VT_R4:
        lValue = var.fltVal;
        break;
    case VT_R8:
        lValue = var.dblVal;
        break;
    case VT_I1:
        lValue = var.cVal;
        break;
    case VT_UI2:
        lValue = var.uiVal;
        break;
    case VT_UI4:
        lValue = var.ulVal;
        break;
    case VT_UI8:
        lValue = var.ullVal;
        break;
    case VT_INT:
        lValue = var.intVal;
        break;
    case VT_UINT:
        lValue = var.uintVal;
        break;
    default:
        lValue = 0;
        break;
    }

    return lValue;
}
LONG CrackDoubleVariant(const COleVariant& var)
{
    double dValue = 0;
    switch(var.vt)
    {
    case VT_EMPTY:
        dValue = 0;;
        break;
    case VT_NULL:
        dValue = 0;
        break;
    case VT_I2:
        dValue = var.iVal;
        break;
    case VT_UI1:
        dValue = var.bVal;
        break;
    case VT_I4:
        dValue = var.lVal;
        break;
    case VT_R4:
        dValue = var.fltVal;
        break;
    case VT_R8:
        dValue = var.dblVal;
        break;
    case VT_I1:
        dValue = var.cVal;
        break;
    case VT_UI2:
        dValue = var.uiVal;
        break;
    case VT_UI4:
        dValue = var.ulVal;
        break;
    case VT_UI8:
        dValue = var.ullVal;
        break;
    case VT_INT:
        dValue = var.intVal;
        break;
    case VT_UINT:
        dValue = var.uintVal;
        break;
    default:
        dValue = 0;
        break;
    }

    return dValue;
}
CExcel::CExcel()
:m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
,m_eSelectVersion(EXCEL_VERSION_2007)
,m_eOperateType(EXCEL_OT_MS_EXCEL)
,m_lSheetNum(0)
,m_lSheetRowCount(0)
,m_lSheetColumnCount(0)
#if defined EXCEL_ADO
,m_ptrConnection(NULL)
,m_pcordsetPtr(NULL)
#endif
{

}
CExcel::CExcel(const _tstring& strFileFullPath)
:m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
,m_eSelectVersion(EXCEL_VERSION_2007)
,m_eOperateType(EXCEL_OT_MS_EXCEL)
,m_strFileFullPath(strFileFullPath)
,m_lSheetRowCount(0)
,m_lSheetNum(0)
,m_lSheetColumnCount(0)
#if defined EXCEL_ADO
,m_ptrConnection(NULL)
,m_pcordsetPtr(NULL)
#endif
{
}
CExcel::CExcel(Enum_OperateType eOperateType,Enum_ExcelVersion eVersion,const _tstring& strFileFullPath)
:m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
,m_eSelectVersion(eVersion)
,m_eOperateType(eOperateType)
,m_strFileFullPath(strFileFullPath)
,m_lSheetRowCount(0)
,m_lSheetNum(0)
,m_lSheetColumnCount(0)
#if defined EXCEL_ADO
,m_ptrConnection(NULL)
,m_pcordsetPtr(NULL)
#endif
{
}
CExcel::~CExcel(void)
{
	try
	{
		
		m_Range.ReleaseDispatch();
		m_Sheet.ReleaseDispatch();
		m_Sheets.ReleaseDispatch();
		m_Book.ReleaseDispatch();
		m_Books.ReleaseDispatch();
		m_Application.ReleaseDispatch();
		m_covOptional.Clear();
	}
	catch(...)
	{	
		OutputDebugString(_T("Excel：析构出错\n"));
	}
}
/**	@fn OutPutError
*	@brief 错误报告输出
*	@param  [INPUT] caller 调用信息
*           [INPUT] e 错误信息
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::OutPutError(const _tstring& caller, _com_error & e)
{

	_tstring wsError;
	_tstring wsTemp;
	wsError = StdtstringFormat(_T(">DB ---- DBHelper caller %s fail, error detail:\n"), EXCEL_2PCSZ(caller));

	wsTemp = StdtstringFormat(_T(">DB ---- \a\t Code = %08x\n"), e.Error());
	wsError += wsTemp;

	wsTemp = StdtstringFormat(_T(">DB ---- \a\t Code meaning = %s\n"), e.ErrorMessage());
	wsError += wsTemp;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	wsTemp = StdtstringFormat(_T(">DB ---- \a\t Source = %s\n"), (LPCTSTR)bstrSource);
	wsError += wsTemp;
	wsTemp =  StdtstringFormat(_T(">DB ---- \a\t Description = %s\n"), (LPCTSTR)bstrDescription);
	wsError += wsTemp;

	OutputDebugString(EXCEL_2PCSZ(wsError));
}


/**	@fn IntToLetter(int n)
*	@brief	索引值转换为Excel列头字母
*	@param  [INPUT] n：索引值
*	@return	无
*/
_tstring CExcel::IntToLetter(int n)
{
	if (n > 256)
	{
		AfxMessageBox(_T("索引超出范围，Excel的列索引不能超过256！"));
		return _T("");
	}

	int i = n / 26;
	int j = n % 26;

	_tstring strChar1 = StdtstringFormat(_T("%c"),i + 64);
	_tstring strChar2 = StdtstringFormat(_T("%c"),j + 64);


	if (n > 26)
	{
		return strChar1 + strChar2;
	}
	else if (n == 26)
	{
		return _T("Z");
	}
	else
	{
		return strChar2;
	}
}

/**	@fn ShowSaveFile
*	@brief	显示保存对话框
*	@param  [INPUT] strFileName 默认文件名称
*           [INPUT] strTitle 对话框名称
*           [INOUT] 
*           [OUTPUT] OutputFileName 文件保存路径
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
BOOL CExcel::ShowSaveFile(const _tstring strFileName,
                         const _tstring strTitle,
                         _tstring& OutputFileName)
{
    _tstring szFilters =   _T("Excel 2007(*.xlsx)|*.xlsx|Excel 2003(*.xls)|*.xls|Excel 97(*.xls)|*.xls||");
    if (ShowFileDlg(_T(""),strFileName,strTitle,szFilters,FALSE,OutputFileName))
    {
        if (EXCEL_NOPS == EXCEL_FIND(OutputFileName,_T(".xls"),2))
        {
            if (EXCEL_VERSION_2007 == m_eSelectVersion)
            {
                OutputFileName += _T(".xlsx");
            }
            else
            {
                OutputFileName += _T(".xls");
            }
			//判断文件是否存在
			if (PathFileExists(OutputFileName))
			{
				CString str;
				str.Format(_T("%s\r\n 文件已存在，是否替换？"), OutputFileName);
				if (IDYES != FMessageBox::Show(str, _T("导出"), MB_YESNO | MB_ICONQUESTION))
				{
					return FALSE;
				}
			}
        }
	
        m_strFileFullPath = OutputFileName;
        return TRUE;
    }

    return FALSE;
}

/**	@fn ShowOpenFile
*	@brief	显示选择对话框
*	@param  [INPUT] strFileName 默认文件名称
*           [INPUT] strTitle 对话框名称
*           [INOUT] 
*           [OUTPUT] 选中文件路径
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	true 成功 false 失败
*/
BOOL CExcel::ShowOpenFile(const _tstring strFileName,
                          const _tstring strTitle,
                          _tstring& OutputFileName)
{
    _tstring szFilters =   _T("Excel 2007(*.xlsx)|*.xlsx|Excel 2003(*.xls)|*.xls|Excel 97(*.xls)|*.xls||");
    if (ShowFileDlg(_T(""),strFileName,strTitle,szFilters,TRUE,OutputFileName))
    {
        m_strFileFullPath = OutputFileName;

        return TRUE;
    }
    else
    {
        return FALSE;
    }    
}

/**	@fn 显示文件操作对话框
*	@brief	
*	@param  [INPUT] strDefaultPath 默认文件路径
*           [INPUT] strFileName 文件名称
*           [INOUT] strTitle 对话框的名称
*           [INOUT] strFilter 文件过滤条件
*           [INOUT] bType 对话框类型
*           [OUTPUT] OutputFileName 文件路径
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
BOOL CExcel::ShowFileDlg(const _tstring& strDefaultPath, const _tstring strFileName,
                         const _tstring strTitle,const _tstring strFilter,BOOL bType,
                         _tstring& OutputFileName)
{
    //CFileDialog dlgFile(FALSE,NULL,sFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,sFilter);
    CFileDialog dlgFile(bType,NULL,EXCEL_2PCSZ(strFileName),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING|OFN_OVERWRITEPROMPT ,EXCEL_2PCSZ(strFilter));

    if (!EXCEL_EMPTY(strDefaultPath))
    {
        dlgFile.GetOFN().lpstrInitialDir = EXCEL_2PCSZ(strDefaultPath);// 默认目录
    }

    dlgFile.m_ofn.lpstrTitle  = EXCEL_2PCSZ(strTitle);
    if (dlgFile.DoModal() == IDOK)
    {
        OutputFileName = dlgFile.GetPathName().GetBuffer();
        m_eSelectVersion = (Enum_ExcelVersion)dlgFile.m_ofn.nFilterIndex;

        return TRUE;
    }
    return FALSE;
}

/**	@fn MakeConnStr
*	@brief	生成数据库连接字符串
*	@param  [INPUT] strFileName 文件路径
*           [INPUT] bHeader 是否包含列头
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	数据库连接字符串
*/
_tstring CExcel::MakeConnStr(const _tstring& strFileName, bool bHeader/* = true*/)
{
    _tstring strHeader = bHeader ? _T("YES") : _T("NO");
    _tstring strConn;
    if (EXCEL_VERSION_2007 == m_eSelectVersion)
    {
        strConn = StdtstringFormat(_T("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s;Extended Properties=\"Excel 12.0 Xml;HDR=%s\""),
            EXCEL_2PCSZ(strFileName),EXCEL_2PCSZ(strHeader));
    }
    else
    {
        strConn = StdtstringFormat(_T("Provider='Microsoft.JET.OLEDB.4.0';Data Source=%s;Extended Properties=\"Excel 8.0;HDR=%s\""),
            EXCEL_2PCSZ(strFileName),EXCEL_2PCSZ(strHeader));
    }
    
    return strConn;
}

/**	@fn CreateExcelApp
*	@brief 创建excel实例
*	@param  [INPUT] bVisible 是否显示excel实例
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	true 成功 false 失败
*/
BOOL CExcel::CreateExcelApp(BOOL bVisible)
{
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            CLSID clsid;
            IUnknown *pUnk;

            //检测excel实例是否已经存在
//             ::CLSIDFromProgID(L"Excel.Application",&clsid);
//             if(::GetActiveObject(clsid, NULL,&pUnk) == S_OK) 
//             {
//                 LPDISPATCH lpDisp;
//                 if (!pUnk->QueryInterface(IID_IDispatch,(void**) &lpDisp) == S_OK)
//                 {
//                     pUnk->Release();
//                     return FALSE;
//                 }
// 
//                 m_Application.AttachDispatch(lpDisp);
//                 pUnk->Release();
//             } 
//             else
            {
                //不存在创建新的excel实例
                if(!m_Application.CreateDispatch(_T("Excel.Application"))) 
                {
					AfxMessageBox(_T("CreateDispatch Failed!"));
                    return FALSE;
                }
            }

            m_Application.put_Visible(bVisible);
        }
        else
        {
#if defined EXCEL_ADO
            TESTHR(m_ptrConnection.CreateInstance(__uuidof(Connection)));
#endif
        }
        
    }
    catch(_com_error &e)
    {
        OutPutError(_T("CreateExcelApp"),e);
    }
    catch (...)
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            m_Application.ReleaseDispatch();
        }
        else
        {
#if defined EXCEL_ADO
            
#endif
        }
        
        return FALSE;
    }

    return TRUE;
}

/**	@fn	CreateExcelApp
*	@brief	创建Excel文件
*	@param  [INPUT] bVisible：Excel文件是否可见
*	@return	TRUE：成功；FALSE：失败
*/
BOOL CExcel::NewExcelApp(BOOL bVisible)
{
    if (!CreateExcelApp(bVisible))
    {
        return FALSE;
    }
	
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            //打开一个WorkBook
            m_Books.AttachDispatch(m_Application.get_Workbooks());
            m_Book.AttachDispatch(m_Books.Add(m_covOptional));

            //设置WorkSheets
            m_Sheets.AttachDispatch(m_Book.get_Sheets());
        }
        else
        {
#if defined EXCEL_ADO
            TESTHR(m_ptrConnection->Open(_bstr_t(EXCEL_2PCSZ(MakeConnStr(m_strFileFullPath))), "", "", NULL));
#endif
        }
        
    }
    catch(_com_error &e)
    {
        OutPutError(_T("NewExcelApp"),e);
    }
    catch (...)
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            m_Sheets.ReleaseDispatch();
            m_Book.ReleaseDispatch();
            m_Books.ReleaseDispatch();
            m_Application.ReleaseDispatch();
            m_covOptional.Clear();
        }
        else
        {
#if defined EXCEL_ADO

#endif
        }
        
    }
	
	return TRUE;
}

/**	@fn OpenExcelApp
*	@brief	打开Excel文件
*	@param  [INPUT] strSheetName 要打开的excel sheet 名称
*           [INPUT] bVisible excel文件是否可见
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
BOOL CExcel::OpenExcelApp(const _tstring& strSheetName,BOOL bVisible)
{
    if (!CreateExcelApp(bVisible))
    {
        return FALSE;
    }
    
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            //打开一个WorkBook
            m_Books.AttachDispatch(m_Application.get_Workbooks());
            LPDISPATCH lpDisp = m_Books.Open(EXCEL_2PCSZ(m_strFileFullPath), vtMissing, vtMissing, vtMissing, 
                vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
                vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);        
            m_Book.AttachDispatch(lpDisp);  

            //设置WorkSheet
            m_Sheets.AttachDispatch(m_Book.get_Sheets());
            //打开指定名称的WorkSheet
            COleVariant vtItem(EXCEL_2PCSZ(strSheetName));
            m_Sheet.AttachDispatch(m_Sheets.get_Item(vtItem));

            CRange usedRange = m_Sheet.get_UsedRange();
            //get used row  
            CRange range;
            range.AttachDispatch(usedRange.get_Rows());  
            m_lSheetRowCount = range.get_Count();  
            range.ReleaseDispatch();

            //get used column   
            range.AttachDispatch(usedRange.get_Columns());  
            m_lSheetColumnCount = range.get_Count();   

            range.ReleaseDispatch();
            usedRange.ReleaseDispatch();
        }
        else
        {
        #if defined EXCEL_ADO
            m_strSheetName = strSheetName;
            TESTHR(m_ptrConnection->Open(_bstr_t(EXCEL_2PCSZ(MakeConnStr(m_strFileFullPath))), "", "", NULL));
        #endif
        }    
    }
    catch(_com_error &e)
    {
        OutPutError(_T("OpenExcelApp"),e);
    }
    catch (...)
    {
        return FALSE;
    }

    return TRUE;
}

/**	@fn AddWorkSheet
*	@brief	添加sheet 
*	@param  [INPUT] strSheetName sheet名称
*           [INPUT] lSheetNum sheet序号
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::AddWorkSheet(const _tstring& strSheetName,LONG lSheetNum)
{

    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        m_Sheet.ReleaseDispatch();
        //如果WorkSheet索引不等于-1，则打开指定索引的WorkSheet
        //否则新增一个WorkSheet
        if (lSheetNum <= 3)
        {
            COleVariant vtItem((short)lSheetNum);
            m_Sheet.AttachDispatch(m_Sheets.get_Item(vtItem));
            m_Sheet.put_Name(EXCEL_2PCSZ(strSheetName));
        }
        else
        {
            //在最前面增加sheet
            m_Sheet.AttachDispatch(m_Sheets.Add(vtMissing,vtMissing,vtMissing,vtMissing));
            //多个sheet名称不能相同
            m_Sheet.put_Name(EXCEL_2PCSZ(strSheetName));
        }
    }
    else
    {
#if defined EXCEL_ADO
        m_strSheetName = strSheetName;
        m_lSheetNum = lSheetNum;
#endif
    }   
}

/**	@fn OpenWorkSheet
*	@brief	打开sheet
*	@param  [INPUT] strSheetName sheet名称
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::OpenWorkSheet(const _tstring& strSheetName)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        m_Sheet.ReleaseDispatch();
        //打开指定名称的WorkSheet
        COleVariant vtItem(EXCEL_2PCSZ(strSheetName));
        m_Sheet.AttachDispatch(m_Sheets.get_Item(vtItem));

        CRange usedRange = m_Sheet.get_UsedRange();
        //get used row  
        CRange range;
        range.AttachDispatch(usedRange.get_Rows());  
        m_lSheetRowCount = range.get_Count();  
        range.ReleaseDispatch();
 
        //get used column   
        range.AttachDispatch(usedRange.get_Columns());  
        m_lSheetColumnCount = range.get_Count();   
        
        range.ReleaseDispatch();
        usedRange.ReleaseDispatch();
    }
    else
    {
    #if defined EXCEL_ADO
        m_strSheetName = strSheetName;
    #endif
    }
    
}

/**	@fn SetTitle
*	@brief	设置标题
*	@param  [INPUT] struTitle：标题信息
*	@return	无
*/
void CExcel::SetTitle(const Struct_Title& struTitle)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell(struTitle.lStartRow,struTitle.lStartColumn,struTitle.strName);
        MergeCell(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);

        //设置粗体
        if (struTitle.bBold)
        {
            SetTextBold(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);
        }

        //设置字体大小
        SetFontSize(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn,struTitle.lFontSize);

        //设置水平对齐方式
        if (struTitle.lHAlign == 0)
        {
            SetHAlignLeft(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);
        }
        else if (struTitle.lHAlign == 1)
        {
            SetHAlignCenter(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);
        }
        else if (struTitle.lHAlign == 2)
        {
            SetHAlignRight(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);
        }
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn SetComment
*	@brief	设置备注信息
*	@param  [INPUT] struComment：备注信息
*	@return	无
*/
void CExcel::SetComment(const Struct_Comment& struComment)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell(struComment.lStartRow,struComment.lStartColumn,struComment.strName);
        MergeCell(struComment.lStartRow,struComment.lStartColumn,struComment.lEndRow,struComment.lEndColumn);

        //设置水平对齐方式
        if (struComment.lHAlign == 0)
        {
            SetHAlignLeft(struComment.lStartRow,struComment.lStartColumn,struComment.lEndRow,struComment.lEndColumn);
        }
        else if (struComment.lHAlign == 1)
        {
            SetHAlignCenter(struComment.lStartRow,struComment.lStartColumn,struComment.lEndRow,struComment.lEndColumn);
        }
        else if (struComment.lHAlign == 2)
        {
            SetHAlignRight(struComment.lStartRow,struComment.lStartColumn,struComment.lEndRow,struComment.lEndColumn);
        }
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn AddHeaders
*	@brief	添加列头
*	@param  [INPUT] vecHeader：列头列表
*	@return	无
*/
void CExcel::AddHeaders(std::vector<Struct_Header>& vecHeader)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        for (std::vector<Struct_Header>::iterator It = vecHeader.begin(); It != vecHeader.end(); It++)
        {
            WriteCell(It->lRow,It->lColumn,It->strName);

            range = GetRange(It->lRow,It->lColumn,It->lRow,It->lColumn);

            COleVariant vtWidth((long)It->lWidth);
            range.put_ColumnWidth(vtWidth);
            //SetTextBold(It->lRow,It->lColumn,It->lRow,It->lColumn);
            range.ReleaseDispatch();
        }
    }
    else
    {
    #if defined EXCEL_ADO
        try
        {
            _CommandPtr pCmd = NULL;
            TESTHR(pCmd.CreateInstance(__uuidof(Command)));
            pCmd->ActiveConnection = m_ptrConnection;   
            _tstring strColumn ;
            unsigned uCount = 0;
            m_vecColumnName.clear();
            for (std::vector<Struct_Header>::iterator It = vecHeader.begin(); It != vecHeader.end(); It++,uCount++)
            {
                if (uCount ==  vecHeader.size()-1)
                {
                    strColumn += It->strName + _T(" varchar");
                }
                else
                {
                    strColumn += It->strName + _T(" varchar,");
                }  
                m_vecColumnName.push_back(It->strName);
            }
            _tstring strSql = StdtstringFormat(_T("CREATE TABLE %s (%s)"),
                EXCEL_2PCSZ(m_strSheetName),EXCEL_2PCSZ(strColumn));

            pCmd->CommandText = _bstr_t(EXCEL_2PCSZ(strSql));

            pCmd->Execute(NULL, NULL, adCmdText);
        }
        catch(_com_error &e)
        {
            OutPutError(_T("AddHeaders"),e);
        }
        catch (...)
        {
        }
        
    #endif
    }
		
}


/**	@fn SaveFile
*	@brief	保存Excel文件
*	@param  [INPUT] strFileName：Excel文件名称
*	@return	无
*/
void CExcel::SaveFile()
{  
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            m_Application.put_DisplayAlerts(FALSE);

            long eFileFormat = 51;//XlFileFormat.xlOpenXMLWorkbook;     Excel 2007
            COleVariant vtFileFormat(eFileFormat);
            if (EXCEL_VERSION_2007 == m_eSelectVersion)
            {
                eFileFormat = 51;//XlFileFormat.xlOpenXMLWorkbook;     Excel 2007
            }
            else if (EXCEL_VERSION_2003 == m_eSelectVersion)
            {
                //eFileFormat = -4143;
                vtFileFormat.lVal = 56;//XlFileFormat.xlExcel8 = 56;    Excel 2003
                vtFileFormat.vt = VT_I4;
            }
            else
            {
                vtFileFormat = m_covOptional;
            }

            COleVariant vtFileName(EXCEL_2PCSZ(m_strFileFullPath));

            m_Book.SaveAs(vtFileName,
                vtFileFormat,
                m_covOptional,
                m_covOptional,
                m_covOptional,
                m_covOptional,1,
                m_covOptional,
                m_covOptional,
                m_covOptional,
                m_covOptional,
                m_covOptional);
            m_Application.put_DisplayAlerts(TRUE);
            m_Book.Close(m_covOptional,vtFileName,m_covOptional);
            //m_Books.Close();
            m_Application.Quit();
        }
        else
        {
#if defined EXCEL_ADO
            if (NULL != m_ptrConnection && adStateOpen == m_ptrConnection->State) 
            {
                if (SUCCEEDED(m_ptrConnection->Close()))
                {

                }
            }
#endif
        }
    }
    catch(_com_error &e)
    {
        OutPutError(_T("SaveFile"),e);
    }
    catch (...)
    {
    }
	
}

/**	@fn CloseFile
*	@brief	关闭文件
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::CloseFile()
{
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            m_Application.put_DisplayAlerts(TRUE);
            //excel2003好像没有改方法，调用可能出错
            //m_Book.Close(m_covOptional,vtFileName,m_covOptional);
            //m_Books.Close();
            m_Application.Quit();
        }
        else
        {
#if defined EXCEL_ADO

            if (NULL != m_ptrConnection && adStateOpen == m_ptrConnection->State) 
            {
                if (SUCCEEDED(m_ptrConnection->Close()))
                {

                }
            }
#endif
        }
    }
    catch(_com_error &e)
    {
        OutPutError(_T("CloseFile"),e);
    }
    catch (...)
    {
    }
   
}

/**	@fn SetRange
*	@brief	设置Excel操作范围
*	@param  [INPUT] lRowCount：行数 
*           [INPUT] lColumnCount：列数
*	@return	无
*/
void CExcel::SetRange(LONG lRowCount,LONG lColumnCount)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        _tstring strStartCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(1)),1);
        _tstring strEndCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lColumnCount)),lRowCount);

        COleVariant vtStartCell(EXCEL_2PCSZ(strStartCell));
        COleVariant vtEndCell(EXCEL_2PCSZ(strEndCell));
        m_Range = m_Sheet.get_Range(vtStartCell,vtEndCell);
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }	
}

/**	@fn SetRange
*	@brief	设置Excel操作范围
*	@param  [INPUT] lRowCount：行数 
*           [INPUT] lColumnCount：列数
*           [INPUT] dRowHeight 行高
*	@return	无
*/
void CExcel::SetRange(LONG lRowCount,LONG lColumnCount,DOUBLE dRowHeight)
{

    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        _tstring strStartCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(1)),1);
        _tstring strEndCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lColumnCount)),lRowCount);

        COleVariant vtStartCell(EXCEL_2PCSZ(strStartCell));
        COleVariant vtEndCell(EXCEL_2PCSZ(strEndCell));

        m_Range = m_Sheet.get_Range(vtStartCell,vtEndCell);

        COleVariant vtRowHeight(dRowHeight);
        m_Range.put_RowHeight(vtRowHeight);

        SetHAlignCenter(1,1,lRowCount,lColumnCount);
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }	
}

/**	@fn BeginWrite
*	@brief	开始excel写入操作，采用ADO方式，需要调用此方法
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::BeginWrite()
{

	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{

	}
	else
	{
#if defined EXCEL_ADO
		try
		{
			TESTHR(m_pcordsetPtr.CreateInstance(__uuidof(Recordset)));

			_tstring strSql = StdtstringFormat(_T("SELECT * FROM [%s$]"),EXCEL_2PCSZ(m_strSheetName));
			m_pcordsetPtr->Open(_variant_t(EXCEL_2PCSZ(strSql)), _variant_t((IDispatch*)m_ptrConnection), adOpenKeyset, adLockOptimistic, adCmdText);
		}
		catch(_com_error &e)
		{
			OutPutError(_T("BeginWrite"),e);
		}
		catch (...)
		{
		}
#endif
	}
}

/**	@fn EndWrite
*	@brief	结束excel写入操作，采用ADO方式，需要调用此方法
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::EndWrite()
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{

	}
	else
	{
#if defined EXCEL_ADO
		try
		{
			TESTHR(m_pcordsetPtr->Close());
			m_pcordsetPtr = NULL;
		}
		catch(_com_error &e)
		{
			OutPutError(_T("EndWrite"),e);
		}
		catch (...)
		{
		}

#endif
	}
}

/**	@fn StartWriteRow
*	@brief	开始行写入操作，采用ADO方式，需要调用此方法
*	@param  [INPUT] lRow 行号
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::StartWriteRow(LONG lRow)
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{

	}
	else
	{
#if defined EXCEL_ADO
		try
		{
			TESTHR(m_pcordsetPtr->AddNew());
		}
		catch(_com_error &e)
		{
			OutPutError(_T("StartWriteRow"),e);
		}
		catch (...)
		{
		}

#endif
	}
}

/**	@fn EndWriteRow
*	@brief	结束行操作，采用ADO方式，需要调用此方法
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::EndWriteRow()
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{

	}
	else
	{
#if defined EXCEL_ADO
		try
		{
			TESTHR(m_pcordsetPtr->Update());
		}
		catch(_com_error &e)
		{
			OutPutError(_T("EndWriteRow"),e);
		}
		catch (...)
		{
		}
#endif
	}
}


/**	@fn WriteCell_
*	@brief	单元格写入数据
*	@param  [INPUT] lColumn 列号
*           [INPUT] vValue 数据
*           [INOUT] 
*           [OUTPUT] 

*	@return	无
*/
void CExcel::WriteCell_(LONG  lColumn,_variant_t vValue)
{
#if defined EXCEL_ADO
	if (lColumn > 0 && lColumn <= m_vecColumnName.size())
	{
		m_pcordsetPtr->Fields->GetItem(EXCEL_2PCSZ(m_vecColumnName[lColumn-1]))->Value = vValue;  
	}
#endif
}
void CExcel::WriteCell_(COleVariant vRow,COleVariant vColumn,COleVariant vValue)
{
	m_Range.put_Item(vRow,vColumn,vValue);	
}

/**	@fn WriteCell
*	@brief	单元格中写入整型值
*	@param  [INPUT] lRow：行号，从1开始
*           [INPUT] lColumn：列号，从1开始
*           [INPUT] lValue：整型值
*	@return	无
*/
void CExcel::WriteCell(LONG lRow,LONG lColumn,LONG lValue)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell_(lRow,lColumn,lValue);
    }
    else
    {
#if defined EXCEL_ADO
        WriteCell_(lColumn,lValue);
#endif
    }		
}

/**	@fn WriteCell
*	@brief	单元格中写入小数值
*	@param  [INPUT] lRow：行号，从1开始
*           [INPUT] lColumn：列号，从1开始
*           [INPUT] lValue：整型值
*	@return	无
*/
void CExcel::WriteCell(LONG lRow,LONG lColumn,DOUBLE dValue)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell_(lRow,lColumn,dValue);
    }
    else
    {
#if defined EXCEL_ADO
        WriteCell_(lColumn,dValue);
#endif
    }	
}




/**	@fn void CExcel::WriteCell(LONG lRow,LONG lColumn,const CString& strValue)
*	@brief	设置单元格中的字符串值
*	@param  [INPUT] lRow：行号
*           [INPUT] lColumn：列号
*           [INPUT] lValue：字符串值
*	@return	无
*/

void CExcel::WriteCell(LONG lRow,LONG lColumn,const _tstring& strValue)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell_(lRow,lColumn,EXCEL_2PCSZ(strValue));	
    }
    else
    {
#if defined EXCEL_ADO
        WriteCell_(lColumn,EXCEL_2PCSZ(strValue));	
#endif
    }
    
}

/**	@fn AddHyperlinks_
*	@brief	单元格设置超链接
*	@param  [INPUT] lRow：行号，从1开始 
*           [INPUT] lColumn：列号，从1开始
*           [INPUT] vValue：单元格数据
*           [INPUT] strLink：超链接信息
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::AddHyperlinks_(LONG lRow,LONG lColumn,COleVariant vValue,const _tstring& strLink)
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		COleVariant vMissing;
		vMissing.vt = VT_ERROR;
		vMissing.scode = DISP_E_PARAMNOTFOUND;

		CHyperlinks Hyperlinks(m_Sheet.get_Hyperlinks());

		_tstring strCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lColumn)),lRow);

		//Hyperlinks.Add(m_Sheet.get_Range(COleVariant(strCell), vMissing), strLink, vMissing, vMissing, vText);
		Hyperlinks.Add(m_Sheet.get_Range(COleVariant(EXCEL_2PCSZ(strCell)), vMissing), EXCEL_2PCSZ(strLink), vMissing, vMissing, vValue);

		Hyperlinks.ReleaseDispatch();
	}
	else
	{
#if defined EXCEL_ADO

#endif
	}
}

/**	@fn AddHyperlinks_
*	@brief	单元格设置超链接
*	@param  [INPUT] lRow：行号，从1开始 
*           [INPUT] lColumn：列号，从1开始
*           [INPUT] strValue：单元格数据
*           [INPUT] strLink：超链接信息
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::AddHyperlinks(LONG lRow,LONG lColumn,const _tstring& strValue,const _tstring& strLink)
{
	AddHyperlinks_(lRow,lColumn,EXCEL_2PCSZ(strValue),strLink);
}

void CExcel::AddHyperlinks(LONG lRow,LONG lColumn,const _tstring& strLink)
{
	AddHyperlinks_(lRow,lColumn,vtMissing,strLink);
}

/**	@fn MergeCell
*	@brief	合并单元格
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*	@return	无
*/
void CExcel::MergeCell(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        COleVariant vt((long)0);
        range.Merge(vt);
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn SetHAlignRight
*	@brief	设置水平右对齐
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*	@return	无
*/
void CExcel::SetHAlignRight(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        COleVariant vt(LONG(-4152));
        range.put_HorizontalAlignment(vt);
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }	
}

/**	@fn SetHAlignLeft
*	@brief	设置水平左对齐
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*	@return	无
*/
void CExcel::SetHAlignLeft(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        COleVariant vt(LONG(-4131));
        range.put_HorizontalAlignment(vt);
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn SetHAlignLeft
*	@brief	设置水平居中对齐
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*	@return	无
*/
void CExcel::SetHAlignCenter(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        COleVariant vt(LONG(-4108));
        range.put_HorizontalAlignment(vt);
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }	
}

/**	@fn SetTextBold
*	@brief	设置字体加粗
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*	@return	无
*/
void CExcel::SetTextBold(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        CFont0 font = range.get_Font();

        COleVariant vt((short)TRUE);
        font.put_Bold(vt);

        font.ReleaseDispatch();
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn SetTextColor(LONG lStartRow,
*	@brief	设置字体颜色
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*           [INPUT] colorValue：颜色值
*	@return	无
*/
void CExcel::SetTextColor(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,COLORREF colorValue)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        CFont0 font = range.get_Font();

        COleVariant vt((long)colorValue);
        font.put_Color(vt);

        font.ReleaseDispatch();
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn SetFontSize
*	@brief	设置字体大小
*	@param  [INPUT] lStartRow：起始单元格行号
*           [INPUT] lStartColumn：起始单元格列号
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*           [INPUT] nFontSize：字体大小
*	@return	无
*/
void CExcel::SetFontSize(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,int nFontSize)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        CFont0 font = range.get_Font();

        COleVariant vt((long)nFontSize);
        font.put_Size(vt);

        font.ReleaseDispatch();
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }	
}

/**	@fn AddPicture
*	@brief	插入图片
*	@param  [INPUT] lStartRow：起始单元格行号
*           [INPUT] lStartColumn：起始单元格列号
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*           [INPUT] strPicPath：图片路径
*	@return	无
*/
void CExcel::AddPicture(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,const _tstring& strPicPath)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        float fLeft = (float)range.get_Left().dblVal;
        float fTop = (float)range.get_Top().dblVal;
        float fWidth = (float)range.get_Width().dblVal;
        float fHeight = (float)range.get_Height().dblVal;

        CShapes shapes;
        shapes.AttachDispatch(m_Sheet.get_Shapes());
        CShape shape = shapes.AddPicture(EXCEL_2PCSZ(strPicPath),true,true,fLeft,fTop,fWidth,fHeight);
        shapes.ReleaseDispatch();

        shape.ReleaseDispatch();
        range.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	
}

/**	@fn SetBorder()
*	@brief	加粗当前Excel操作范围的边界
*	@param  无
*	@return	无
*/
void CExcel::SetBorder()
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CBorders borders;
        CBorder border;

        borders=m_Range.get_Borders();

        COleVariant vt((long)1);
        border=borders.get_Item(7);                         //左
        border.put_LineStyle(vt);

        border=borders.get_Item(8);                         //上
        border.put_LineStyle(vt);

        border=borders.get_Item(10);                        //右
        border.put_LineStyle(vt);

        border=borders.get_Item(9);                         //下
        border.put_LineStyle(vt);

        border=borders.get_Item(11);                        //内部垂直
        border.put_LineStyle(vt);

        border=borders.get_Item(12);                        //内部水平
        border.put_LineStyle(vt);

        border.ReleaseDispatch();
        borders.ReleaseDispatch();
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
}

/**	@fn LPDISPATCH GetRange
*	@brief	获取指定的操作范围
*	@param  [INPUT] lStartRow：起始单元格行号
*           [INPUT] lStartColumn：起始单元格列号
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*	@return	无
*/
LPDISPATCH CExcel::GetRange(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        _tstring strStartCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lStartColumn)),lStartRow);
        _tstring strEndCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lEndColumn)),lEndRow);

        COleVariant vtStartCell(EXCEL_2PCSZ(strStartCell));
        COleVariant vtEndCell(EXCEL_2PCSZ(strEndCell));

        return m_Range.get_Range(vtStartCell,vtEndCell);
    }
    else
    {
#if defined EXCEL_ADO

#endif
    }
	return NULL;
}

/**	@fn SetBKColor
*	@brief	设置单元格背景颜色
*	@param  [INPUT] lStartRow：起始单元格行号
*           [INPUT] lStartColumn：起始单元格列号
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*           [INPUT] colorValue RGB颜色值
*	@return	无
*/
void CExcel::SetBKColor( LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,COLORREF colorValue )
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CRange range;
        range = GetRange(lStartRow,lStartColumn,lEndRow,lEndColumn);

        CInterior interior = range.get_Interior();

        COleVariant vt((LONG)colorValue);
        interior.put_Color(vt);

        interior.ReleaseDispatch();
        range.ReleaseDispatch();
    }
    else
    {
    #if defined EXCEL_ADO
    
    #endif
    }
	
}

/**	@fn GetSheetRowCount
*	@brief	获取sheet数据行数
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	无
*/
LONG CExcel::GetSheetRowCount()
{
	return m_lSheetRowCount;
}

/**	@fn GetSheetColumnCount
*	@brief	获取sheet列数
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
 
*	@return	无
*/
LONG CExcel::GetSheetColumnCount()
{
	return m_lSheetColumnCount;
}

/**	@fn ExcelReadAllCell_
*	@brief	excel操作方式读取excel所以数据
*	@param  [INPUT] range excel操作范围
*           [INPUT] vecData 数据
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	无
*/
void CExcel::ExcelReadAllCell_(CRange& range,std::vector<Struct_RowData>& vecData)
{

	COleSafeArray oleDataArray = range.get_Value2();
	LONG lFirstLBound = 0;
	LONG lFirstUBound = 0;
	LONG lSecondLBound = 0;
	LONG lSecondUBound = 0;
	oleDataArray.GetLBound(1,&lFirstLBound);
	oleDataArray.GetUBound(1,&lFirstUBound);
	oleDataArray.GetLBound(2,&lSecondLBound);
	oleDataArray.GetUBound(2,&lSecondUBound);
	VARIANT varItem;
	long index[2] = {0,0};
	_tstring strValue;
	Struct_RowData struRowData;
	for (int i = lFirstLBound; i <= lFirstUBound;i++)
	{
		index[0] = i;
		for (int j = lSecondLBound; j <= lSecondUBound;j++)
		{
			index[1] = j;            
			oleDataArray.GetElement(index,&varItem);

			struRowData.vecRowData.push_back(CrackStrVariant(varItem));     
		}
		vecData.push_back(struRowData);
		struRowData.vecRowData.clear();
	}
	oleDataArray.Clear();
}

/**	@fn ReadAllCell
*	@brief	读取指定范围的excel数据
*	@param  [INPUT] lStartRow：起始单元格行号，从1开始
*           [INPUT] lStartColumn：起始单元格列号，从1开始
*           [INPUT] lEndRow：结束单元格行号
*           [INPUT] lEndColumn：结束单元格列号
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] vecData 数据
*	@return	无
*/
void CExcel::ReadAllCell(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,
						 std::vector<Struct_RowData>& vecData)
{

	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		_tstring strStartCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lStartColumn)),lStartRow);
		_tstring strEndCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lEndColumn)),lEndRow);

		CRange range;
		range = m_Sheet.get_Range(COleVariant(EXCEL_2PCSZ(strStartCell)),COleVariant(EXCEL_2PCSZ(strEndCell))); 
		ExcelReadAllCell_(range,vecData);
		range.ReleaseDispatch();
	}
	else
	{
#if defined EXCEL_ADO
		try
		{
			//TESTHR(pcordsetPtr.CreateInstance(__uuidof(Recordset)));
			//_ConnectionPtr pCon = NULL;
			//_RecordsetPtr pSchema = NULL;

			//TESTHR(pCon.CreateInstance(__uuidof(Connection)));
			//TESTHR(pCon->Open(connStr, "", "", NULL));

			//pSchema = m_ptrConnection->OpenSchema(adSchemaTables);  
			//while(!pSchema->adoEOF)
			//{

			//    _tstring sheetName = (TCHAR*)(_bstr_t)pSchema->Fields->GetItem(_T("TABLE_NAME"))->Value.bstrVal;
			//    pSchema->MoveNext();

			//}

			//for(int i = 0; i < sheetIndex; ++i) pSchema->MoveNext();


			//std::stringstream stream;
			//stream << "SELECT * FROM [" << sheetName << "]";

			m_lSheetRowCount = 0;
			m_lSheetColumnCount = 0;
			_tstring strSql = StdtstringFormat(_T("SELECT * FROM [%s$%s%d:%s%d]"),EXCEL_2PCSZ(m_strSheetName),
				EXCEL_2PCSZ(IntToLetter(lStartColumn)),lStartRow,
				EXCEL_2PCSZ(IntToLetter(lEndColumn)),lEndRow);
			_variant_t	var(0L);
			_RecordsetPtr pcordsetPtr= m_ptrConnection->Execute(_bstr_t(EXCEL_2PCSZ(strSql)), &var,adOptionUnspecified);
			//pcordsetPtr->Open(_variant_t(EXCEL_2PCSZ(strSql)), _variant_t((IDispatch*)m_ptrConnection), adOpenUnspecified, adLockOptimistic, adCmdText);
			Struct_RowData struRowData;

			while(!pcordsetPtr->adoEOF)
			{
				for(long i = 0; i < pcordsetPtr->Fields->GetCount(); ++i)
				{
					struRowData.vecRowData.push_back(CrackStrVariant(pcordsetPtr->Fields->GetItem(i)->Value)); 
				}  
				vecData.push_back(struRowData);
				struRowData.vecRowData.clear();

				pcordsetPtr->MoveNext();
			}
			m_lSheetRowCount = vecData.size();
			if (vecData.size() > 0)
			{
				m_lSheetColumnCount = vecData[0].vecRowData.size();
			}
			pcordsetPtr->Close();
			pcordsetPtr = NULL;
		}
		catch(_com_error &e)
		{

			OutPutError(_T("ReadAllCell"),e);
		}
		catch (...)
		{

		}
#endif
	}   
}

/**	@fn ReadAllCell
*	@brief	读取所有单元格数据
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] vecData 数据
*	@return	无
*/
void CExcel::ReadAllCell(std::vector<Struct_RowData>& vecData)
{   
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		CRange usedRange;  
		usedRange.AttachDispatch(m_Sheet.get_UsedRange());  

		ExcelReadAllCell_(usedRange,vecData);

		usedRange.ReleaseDispatch();
	}
	else
	{
#if defined EXCEL_ADO

		try
		{
			m_lSheetRowCount = 0;
			m_lSheetColumnCount = 0;
			_RecordsetPtr pcordsetPtr;
			TESTHR(pcordsetPtr.CreateInstance(__uuidof(Recordset)));

			_tstring strSql = StdtstringFormat(_T("SELECT * FROM [%s$]"),EXCEL_2PCSZ(m_strSheetName));
			pcordsetPtr->Open(_variant_t(EXCEL_2PCSZ(strSql)), _variant_t((IDispatch*)m_ptrConnection), adOpenStatic, adLockOptimistic, adCmdText);
			Struct_RowData struRowData;

			while(!pcordsetPtr->adoEOF)
			{
				for(long i = 0; i < pcordsetPtr->Fields->GetCount(); ++i)
				{
					struRowData.vecRowData.push_back(CrackStrVariant(pcordsetPtr->Fields->GetItem(i)->Value)); 
				}  
				vecData.push_back(struRowData);
				struRowData.vecRowData.clear();

				pcordsetPtr->MoveNext();
			}
			m_lSheetRowCount = vecData.size();
			if (vecData.size() > 0)
			{
				m_lSheetColumnCount = vecData[0].vecRowData.size();
			}
			if (NULL != pcordsetPtr)
			{
				pcordsetPtr->Close();
			}
		}
		catch(_com_error &e)
		{

			OutPutError(_T("ReadAllCell"),e);
		}
		catch (...)
		{

		}
#endif
	}    
}


/**	@fn BeginRead
*	@brief	开始读取excel数据
*	@param  [INPUT] 
*           [INPUT] 
*	@return	ture 成功 false 失败
*/
bool CExcel::BeginRead()
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		return true;
	}
	else
	{
#if defined EXCEL_ADO
		try
		{
			m_lSheetRowCount = 0;
			m_lSheetColumnCount = 0;
			_RecordsetPtr pcordsetPtr;
			TESTHR(pcordsetPtr.CreateInstance(__uuidof(Recordset)));

			_tstring strSql = StdtstringFormat(_T("SELECT * FROM [%s$]"),EXCEL_2PCSZ(m_strSheetName));
			pcordsetPtr->Open(_variant_t(EXCEL_2PCSZ(strSql)), _variant_t((IDispatch*)m_ptrConnection), adOpenStatic, adLockOptimistic, adCmdText);

			Struct_RowData struRowData;

			m_vecData.clear();
			while(!pcordsetPtr->adoEOF)
			{
				for(long i = 0; i < pcordsetPtr->Fields->GetCount(); ++i)
				{
					struRowData.vecRowData.push_back(CrackStrVariant(pcordsetPtr->Fields->GetItem(i)->Value)); 
				}  
				m_vecData.push_back(struRowData);
				struRowData.vecRowData.clear();

				pcordsetPtr->MoveNext();
			}
			pcordsetPtr->Close();
			pcordsetPtr = NULL;

			m_lSheetRowCount = m_vecData.size();
			if (m_vecData.size() > 0)
			{
				m_lSheetColumnCount = m_vecData[0].vecRowData.size();
			}

			return (m_lSheetRowCount > 0);
		}
		catch(_com_error &e)
		{
			OutPutError(_T("BeginRead"),e);
		}
		catch (...)
		{
		}
#endif
	}

	return false;
}

/**	@fn ReadCell_
*	@brief	读取单元格数据
*	@param  [INPUT] lRow 行号
*           [INPUT] lColumn 列号
*           [INOUT] 
*           [OUTPUT] 
*	@return	单元格数据
*/
VARIANT CExcel::ReadCell_(LONG lRow,LONG lColumn)
{
	_tstring strCell = StdtstringFormat(_T("%s%d"),EXCEL_2PCSZ(IntToLetter(lColumn)),lRow);
	CRange range;
	COleVariant vtRow(EXCEL_2PCSZ(strCell));
	range = m_Sheet.get_Range(vtRow,vtRow); 
	return range.get_Value2(); 
}

/**	@fn ReadCell_
*	@brief	读取单元格数据
*	@param  [INPUT] lRow 行号
*           [INPUT] lColumn 列号
*           [INOUT] 
*           [OUTPUT] strValue 单元格数据
*	@return	无
*/
void CExcel::ReadCell(LONG lRow,LONG lColumn,_tstring& strValue)
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		strValue = CrackStrVariant(ReadCell_(lRow,lColumn));
	}
	else
	{
#if defined EXCEL_ADO
		if (lRow > 0 && lRow <= m_vecData.size())
		{
			if (lColumn > 0 && lColumn <= m_vecData[lRow-1].vecRowData.size())
			{
				strValue = m_vecData[lRow-1].vecRowData[lColumn-1];
			}
		}
#endif
	}    
}

/**	@fn ReadCell_
*	@brief	读取单元格数据
*	@param  [INPUT] lRow 行号
*           [INPUT] lColumn 列号
*           [INOUT] 
*           [OUTPUT] lValue 单元格数据
*	@return	无
*/
void CExcel::ReadCell(LONG lRow,LONG lColumn,LONG& lValue)
{

	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		lValue = CrackIntVariant(ReadCell_(lRow,lColumn));
	}
	else
	{
#if defined EXCEL_ADO
		if (lRow > 0 && lRow <= m_vecData.size())
		{
			if (lColumn > 0 && lColumn <= m_vecData[lRow-1].vecRowData.size())
			{
				lValue = _tstol(EXCEL_2PCSZ(m_vecData[lRow-1].vecRowData[lColumn-1]));
			}
		}
#endif
	}
}

/**	@fn ReadCell_
*	@brief	读取单元格数据
*	@param  [INPUT] lRow 行号
*           [INPUT] lColumn 列号
*           [INOUT] 
*           [OUTPUT] dValue 单元格数据
*	@return	无
*/
void CExcel::ReadCell(LONG lRow,LONG lColumn,double& dValue)
{

	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		dValue = CrackIntVariant(ReadCell_(lRow,lColumn));
	}
	else
	{
#if defined EXCEL_ADO
		if (lRow > 0 && lRow <= m_vecData.size())
		{
			if (lColumn > 0 && lColumn <= m_vecData[lRow-1].vecRowData.size())
			{
				TCHAR * stopstring;
				dValue = _tcstod(EXCEL_2PCSZ(m_vecData[lRow-1].vecRowData[lColumn-1]),&stopstring);
			}
		}
#endif
	}
}

/**	@fn ReadCell_
*	@brief	读取单元格数据
*	@param  [INPUT] lRow 行号
*           [INPUT] lColumn 列号
*           [INOUT] 
*           [OUTPUT] dateTime 单元格数据
*	@return	无
*/
void CExcel::ReadCell(LONG lRow,LONG lColumn,COleDateTime& dateTime)
{
	if (EXCEL_OT_MS_EXCEL == m_eOperateType)
	{
		VARIANT var = ReadCell_(lRow,lColumn);
		switch(var.vt)
		{
		case VT_DATE: 
			{
				dateTime = var;
				break;
			}
		default:
			dateTime = COleDateTime::GetCurrentTime();
		}
	}
	else
	{
#if defined EXCEL_ADO
		if (lRow > 0 && lRow <= m_vecData.size())
		{
			if (lColumn > 0 && lColumn <= m_vecData[lRow-1].vecRowData.size())
			{
				dateTime.ParseDateTime(EXCEL_2PCSZ(m_vecData[lRow-1].vecRowData[lColumn-1]));
			}
		}
#endif
	}

}

