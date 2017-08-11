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
        va_start(marker, fmt);                            //��ʼ����������

        size_t nLength = _vsctprintf(fmt, marker) + 1;    //��ȡ��ʽ���ַ�������

        _TCHAR* pBuffer = new(std::nothrow) _TCHAR[nLength];

        _vsntprintf_s(pBuffer, nLength, nLength, fmt, marker); 

        strResult = pBuffer;

        delete [] pBuffer;
        va_end(marker);                                    //���ñ�������
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
		OutputDebugString(_T("Excel����������\n"));
	}
}
/**	@fn OutPutError
*	@brief ���󱨸����
*	@param  [INPUT] caller ������Ϣ
*           [INPUT] e ������Ϣ
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	����ֵת��ΪExcel��ͷ��ĸ
*	@param  [INPUT] n������ֵ
*	@return	��
*/
_tstring CExcel::IntToLetter(int n)
{
	if (n > 256)
	{
		AfxMessageBox(_T("����������Χ��Excel�����������ܳ���256��"));
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
*	@brief	��ʾ����Ի���
*	@param  [INPUT] strFileName Ĭ���ļ�����
*           [INPUT] strTitle �Ի�������
*           [INOUT] 
*           [OUTPUT] OutputFileName �ļ�����·��
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
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
			//�ж��ļ��Ƿ����
			if (PathFileExists(OutputFileName))
			{
				CString str;
				str.Format(_T("%s\r\n �ļ��Ѵ��ڣ��Ƿ��滻��"), OutputFileName);
				if (IDYES != FMessageBox::Show(str, _T("����"), MB_YESNO | MB_ICONQUESTION))
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
*	@brief	��ʾѡ��Ի���
*	@param  [INPUT] strFileName Ĭ���ļ�����
*           [INPUT] strTitle �Ի�������
*           [INOUT] 
*           [OUTPUT] ѡ���ļ�·��
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	true �ɹ� false ʧ��
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

/**	@fn ��ʾ�ļ������Ի���
*	@brief	
*	@param  [INPUT] strDefaultPath Ĭ���ļ�·��
*           [INPUT] strFileName �ļ�����
*           [INOUT] strTitle �Ի��������
*           [INOUT] strFilter �ļ���������
*           [INOUT] bType �Ի�������
*           [OUTPUT] OutputFileName �ļ�·��
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
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
        dlgFile.GetOFN().lpstrInitialDir = EXCEL_2PCSZ(strDefaultPath);// Ĭ��Ŀ¼
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
*	@brief	�������ݿ������ַ���
*	@param  [INPUT] strFileName �ļ�·��
*           [INPUT] bHeader �Ƿ������ͷ
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	���ݿ������ַ���
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
*	@brief ����excelʵ��
*	@param  [INPUT] bVisible �Ƿ���ʾexcelʵ��
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	true �ɹ� false ʧ��
*/
BOOL CExcel::CreateExcelApp(BOOL bVisible)
{
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            CLSID clsid;
            IUnknown *pUnk;

            //���excelʵ���Ƿ��Ѿ�����
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
                //�����ڴ����µ�excelʵ��
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
*	@brief	����Excel�ļ�
*	@param  [INPUT] bVisible��Excel�ļ��Ƿ�ɼ�
*	@return	TRUE���ɹ���FALSE��ʧ��
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
            //��һ��WorkBook
            m_Books.AttachDispatch(m_Application.get_Workbooks());
            m_Book.AttachDispatch(m_Books.Add(m_covOptional));

            //����WorkSheets
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
*	@brief	��Excel�ļ�
*	@param  [INPUT] strSheetName Ҫ�򿪵�excel sheet ����
*           [INPUT] bVisible excel�ļ��Ƿ�ɼ�
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
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
            //��һ��WorkBook
            m_Books.AttachDispatch(m_Application.get_Workbooks());
            LPDISPATCH lpDisp = m_Books.Open(EXCEL_2PCSZ(m_strFileFullPath), vtMissing, vtMissing, vtMissing, 
                vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
                vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);        
            m_Book.AttachDispatch(lpDisp);  

            //����WorkSheet
            m_Sheets.AttachDispatch(m_Book.get_Sheets());
            //��ָ�����Ƶ�WorkSheet
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
*	@brief	���sheet 
*	@param  [INPUT] strSheetName sheet����
*           [INPUT] lSheetNum sheet���
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
*/
void CExcel::AddWorkSheet(const _tstring& strSheetName,LONG lSheetNum)
{

    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        m_Sheet.ReleaseDispatch();
        //���WorkSheet����������-1�����ָ��������WorkSheet
        //��������һ��WorkSheet
        if (lSheetNum <= 3)
        {
            COleVariant vtItem((short)lSheetNum);
            m_Sheet.AttachDispatch(m_Sheets.get_Item(vtItem));
            m_Sheet.put_Name(EXCEL_2PCSZ(strSheetName));
        }
        else
        {
            //����ǰ������sheet
            m_Sheet.AttachDispatch(m_Sheets.Add(vtMissing,vtMissing,vtMissing,vtMissing));
            //���sheet���Ʋ�����ͬ
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
*	@brief	��sheet
*	@param  [INPUT] strSheetName sheet����
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
*/
void CExcel::OpenWorkSheet(const _tstring& strSheetName)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        m_Sheet.ReleaseDispatch();
        //��ָ�����Ƶ�WorkSheet
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
*	@brief	���ñ���
*	@param  [INPUT] struTitle��������Ϣ
*	@return	��
*/
void CExcel::SetTitle(const Struct_Title& struTitle)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell(struTitle.lStartRow,struTitle.lStartColumn,struTitle.strName);
        MergeCell(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);

        //���ô���
        if (struTitle.bBold)
        {
            SetTextBold(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn);
        }

        //���������С
        SetFontSize(struTitle.lStartRow,struTitle.lStartColumn,struTitle.lEndRow,struTitle.lEndColumn,struTitle.lFontSize);

        //����ˮƽ���뷽ʽ
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
*	@brief	���ñ�ע��Ϣ
*	@param  [INPUT] struComment����ע��Ϣ
*	@return	��
*/
void CExcel::SetComment(const Struct_Comment& struComment)
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        WriteCell(struComment.lStartRow,struComment.lStartColumn,struComment.strName);
        MergeCell(struComment.lStartRow,struComment.lStartColumn,struComment.lEndRow,struComment.lEndColumn);

        //����ˮƽ���뷽ʽ
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
*	@brief	�����ͷ
*	@param  [INPUT] vecHeader����ͷ�б�
*	@return	��
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
*	@brief	����Excel�ļ�
*	@param  [INPUT] strFileName��Excel�ļ�����
*	@return	��
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
*	@brief	�ر��ļ�
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
*/
void CExcel::CloseFile()
{
    try
    {
        if (EXCEL_OT_MS_EXCEL == m_eOperateType)
        {
            m_Application.put_DisplayAlerts(TRUE);
            //excel2003����û�иķ��������ÿ��ܳ���
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
*	@brief	����Excel������Χ
*	@param  [INPUT] lRowCount������ 
*           [INPUT] lColumnCount������
*	@return	��
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
*	@brief	����Excel������Χ
*	@param  [INPUT] lRowCount������ 
*           [INPUT] lColumnCount������
*           [INPUT] dRowHeight �и�
*	@return	��
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
*	@brief	��ʼexcelд�����������ADO��ʽ����Ҫ���ô˷���
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	����excelд�����������ADO��ʽ����Ҫ���ô˷���
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	��ʼ��д�����������ADO��ʽ����Ҫ���ô˷���
*	@param  [INPUT] lRow �к�
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	�����в���������ADO��ʽ����Ҫ���ô˷���
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	��Ԫ��д������
*	@param  [INPUT] lColumn �к�
*           [INPUT] vValue ����
*           [INOUT] 
*           [OUTPUT] 

*	@return	��
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
*	@brief	��Ԫ����д������ֵ
*	@param  [INPUT] lRow���кţ���1��ʼ
*           [INPUT] lColumn���кţ���1��ʼ
*           [INPUT] lValue������ֵ
*	@return	��
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
*	@brief	��Ԫ����д��С��ֵ
*	@param  [INPUT] lRow���кţ���1��ʼ
*           [INPUT] lColumn���кţ���1��ʼ
*           [INPUT] lValue������ֵ
*	@return	��
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
*	@brief	���õ�Ԫ���е��ַ���ֵ
*	@param  [INPUT] lRow���к�
*           [INPUT] lColumn���к�
*           [INPUT] lValue���ַ���ֵ
*	@return	��
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
*	@brief	��Ԫ�����ó�����
*	@param  [INPUT] lRow���кţ���1��ʼ 
*           [INPUT] lColumn���кţ���1��ʼ
*           [INPUT] vValue����Ԫ������
*           [INPUT] strLink����������Ϣ
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	��Ԫ�����ó�����
*	@param  [INPUT] lRow���кţ���1��ʼ 
*           [INPUT] lColumn���кţ���1��ʼ
*           [INPUT] strValue����Ԫ������
*           [INPUT] strLink����������Ϣ
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	�ϲ���Ԫ��
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*	@return	��
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
*	@brief	����ˮƽ�Ҷ���
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*	@return	��
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
*	@brief	����ˮƽ�����
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*	@return	��
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
*	@brief	����ˮƽ���ж���
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*	@return	��
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
*	@brief	��������Ӵ�
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*	@return	��
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
*	@brief	����������ɫ
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*           [INPUT] colorValue����ɫֵ
*	@return	��
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
*	@brief	���������С
*	@param  [INPUT] lStartRow����ʼ��Ԫ���к�
*           [INPUT] lStartColumn����ʼ��Ԫ���к�
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*           [INPUT] nFontSize�������С
*	@return	��
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
*	@brief	����ͼƬ
*	@param  [INPUT] lStartRow����ʼ��Ԫ���к�
*           [INPUT] lStartColumn����ʼ��Ԫ���к�
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*           [INPUT] strPicPath��ͼƬ·��
*	@return	��
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
*	@brief	�Ӵֵ�ǰExcel������Χ�ı߽�
*	@param  ��
*	@return	��
*/
void CExcel::SetBorder()
{
    if (EXCEL_OT_MS_EXCEL == m_eOperateType)
    {
        CBorders borders;
        CBorder border;

        borders=m_Range.get_Borders();

        COleVariant vt((long)1);
        border=borders.get_Item(7);                         //��
        border.put_LineStyle(vt);

        border=borders.get_Item(8);                         //��
        border.put_LineStyle(vt);

        border=borders.get_Item(10);                        //��
        border.put_LineStyle(vt);

        border=borders.get_Item(9);                         //��
        border.put_LineStyle(vt);

        border=borders.get_Item(11);                        //�ڲ���ֱ
        border.put_LineStyle(vt);

        border=borders.get_Item(12);                        //�ڲ�ˮƽ
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
*	@brief	��ȡָ���Ĳ�����Χ
*	@param  [INPUT] lStartRow����ʼ��Ԫ���к�
*           [INPUT] lStartColumn����ʼ��Ԫ���к�
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*	@return	��
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
*	@brief	���õ�Ԫ�񱳾���ɫ
*	@param  [INPUT] lStartRow����ʼ��Ԫ���к�
*           [INPUT] lStartColumn����ʼ��Ԫ���к�
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*           [INPUT] colorValue RGB��ɫֵ
*	@return	��
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
*	@brief	��ȡsheet��������
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
*	@return	��
*/
LONG CExcel::GetSheetRowCount()
{
	return m_lSheetRowCount;
}

/**	@fn GetSheetColumnCount
*	@brief	��ȡsheet����
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] 
 
*	@return	��
*/
LONG CExcel::GetSheetColumnCount()
{
	return m_lSheetColumnCount;
}

/**	@fn ExcelReadAllCell_
*	@brief	excel������ʽ��ȡexcel��������
*	@param  [INPUT] range excel������Χ
*           [INPUT] vecData ����
*           [INOUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*           [OUTPUT] 
*	@return	��
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
*	@brief	��ȡָ����Χ��excel����
*	@param  [INPUT] lStartRow����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lStartColumn����ʼ��Ԫ���кţ���1��ʼ
*           [INPUT] lEndRow��������Ԫ���к�
*           [INPUT] lEndColumn��������Ԫ���к�
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] vecData ����
*	@return	��
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
*	@brief	��ȡ���е�Ԫ������
*	@param  [INPUT] 
*           [INPUT] 
*           [INOUT] 
*           [OUTPUT] vecData ����
*	@return	��
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
*	@brief	��ʼ��ȡexcel����
*	@param  [INPUT] 
*           [INPUT] 
*	@return	ture �ɹ� false ʧ��
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
*	@brief	��ȡ��Ԫ������
*	@param  [INPUT] lRow �к�
*           [INPUT] lColumn �к�
*           [INOUT] 
*           [OUTPUT] 
*	@return	��Ԫ������
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
*	@brief	��ȡ��Ԫ������
*	@param  [INPUT] lRow �к�
*           [INPUT] lColumn �к�
*           [INOUT] 
*           [OUTPUT] strValue ��Ԫ������
*	@return	��
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
*	@brief	��ȡ��Ԫ������
*	@param  [INPUT] lRow �к�
*           [INPUT] lColumn �к�
*           [INOUT] 
*           [OUTPUT] lValue ��Ԫ������
*	@return	��
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
*	@brief	��ȡ��Ԫ������
*	@param  [INPUT] lRow �к�
*           [INPUT] lColumn �к�
*           [INOUT] 
*           [OUTPUT] dValue ��Ԫ������
*	@return	��
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
*	@brief	��ȡ��Ԫ������
*	@param  [INPUT] lRow �к�
*           [INPUT] lColumn �к�
*           [INOUT] 
*           [OUTPUT] dateTime ��Ԫ������
*	@return	��
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

