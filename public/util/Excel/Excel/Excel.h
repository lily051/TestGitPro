/**	@file Excel.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief excel�����ࡣ
*
*	@author		puzhibing
*	@date		2013/9/10
*
*	@note ����������
*		  1��֧�����ַ�ʽ����excel��
*			1.1 MS Office Excel���ߣ����밲װOffice ExcelӦ�ó���
*			1.2 ADO��ʽ�����û�а�װMS Office����Ҫ��װACCESS����������AccessDatabaseEngine.exe��������Ҫ����� "EXCEL_ADO"
*		  2��ADO��ʽֻ����������excel���뵼������
*		  3��MS Office Excel���ߵ���excel֧�֣�
*			3.1�����������С����ɫ����ʽ�����뷽ʽ
*			3.2�����õ�Ԫ��߶ȡ�������ɫ
*			3.3���ϲ���Ԫ��
*			3.4������ͼƬ
*			3.5����ӳ�����
*		  4��֧��Excel2003��2007�ĵ��뵼��
*	@note ��ʷ��¼��
*	@warning 
*/
#pragma once
#include <vector>
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
#include "CWorksheet.h"
#include "CBorders.h"
#include "CBorder.h"
#include "CShapes.h"
#include "CShape.h"
#include "CInterior.h"
//#define EXCEL_STL
#define EXCEL_ADO
#if defined EXCEL_ADO
#import "../bin/msado15.dll"  rename("EOF", "adoEOF") rename("BOF", "adoBOF")
using namespace ADODB;
#endif



#if defined(EXCEL_STL) // STL
#include <string>
#if (defined _UNICODE) || (defined UNICODE)
#ifndef _tstring   
#define _tstring std::wstring
#define EXCEL_BSTRToSTR(s) s
#define EXCEL_NOPS std::wstring::npos
#endif
#else
#ifndef _tstring   
#define _tstring std::string
#define EXCEL_BSTRToSTR(s) _com_util::ConvertBSTRToString(s)
#define EXCEL_NOPS std::string::npos
#endif
#endif
#define EXCEL_2PCSZ(s) s.c_str()
#define EXCEL_EMPTY(s) s.empty()
#define EXCEL_FIND(s,fs,i) s.find(fs,i)
#else // not STL, i.e. MFC
#include <afx.h>
#define _tstring CString
#define EXCEL_2PCSZ(s) ((LPCTSTR)s)
#define EXCEL_EMPTY(s) s.IsEmpty()
#define EXCEL_BSTRToSTR(s) s
#define EXCEL_FIND(s,fs,i) s.Find(fs,i)
#define EXCEL_NOPS (-1)
#endif

//������Ϣ
typedef struct _Struct_Title
{
	LONG lStartRow;            //��ʼ�к� 1 ��ʼ
	LONG lStartColumn;         //��ʼ�к� 1 ��ʼ
	LONG lEndRow;              //�����к�
	LONG lEndColumn;           //�����к�
	_tstring strName;           //����
	BOOL bBold;                //�Ƿ������
	LONG lFontSize;            //�����С
	BOOL lHAlign;              //0-����룻1-���ж��룻2-�Ҷ���
}Struct_Title;

//ע����Ϣ
typedef struct _Struct_Comment
{
	LONG lStartRow;            //��ʼ�к� 1 ��ʼ
	LONG lStartColumn;         //��ʼ�к� 1 ��ʼ
	LONG lEndRow;              //�����к�
	LONG lEndColumn;           //�����к�
	_tstring strName;           //����
	BOOL lHAlign;              //0-����룻1-���ж��룻2-�Ҷ���
}Struct_Comment;

//��ͷ��Ϣ
typedef struct _Struct_Header
{
	LONG lRow;                 //�к� 1 ��ʼ
	LONG lColumn;              //�к� 1 ��ʼ
	_tstring strName;           //����
	LONG lWidth;               //���
}Struct_Header;

//����
typedef struct _Struct_RowData
{
    std::vector<_tstring> vecRowData;
}Struct_RowData;

//excel��������
typedef enum _Enum_OperateType
{
    EXCEL_OT_MS_EXCEL,
    #if defined EXCEL_ADO
    EXCEL_OT_ADO,
    #endif
}Enum_OperateType;

//excel�汾��ʽ
typedef enum _Enum_Excel_Version
{
    EXCEL_VERSION_2007 = 1,
    EXCEL_VERSION_2003,
    EXCEL_VERSION_97,
}Enum_ExcelVersion;
class CExcel
{
public:
    CExcel();
	CExcel(const _tstring& strFileFullPath);
    CExcel(Enum_OperateType eOperateType,Enum_ExcelVersion eVersion,const _tstring& strFileFullPath = _T(""));
	~CExcel(void);
private:
	COleVariant m_covOptional;

	CApplication m_Application;
	CWorkbooks m_Books;
	CWorkbook m_Book;

	CWorksheets m_Sheets;
	CWorksheet m_Sheet;

	CRange m_Range;

    Enum_ExcelVersion m_eSelectVersion;
    Enum_OperateType m_eOperateType;

    _tstring m_strFileFullPath;
    _tstring m_strSheetName;
    LONG m_lSheetNum;

    //��������
    LONG m_lSheetRowCount;
    //��������
    LONG m_lSheetColumnCount;
#if defined EXCEL_ADO
    _ConnectionPtr	m_ptrConnection;
    _RecordsetPtr m_pcordsetPtr;
    std::vector<_tstring> m_vecColumnName;
    std::vector<Struct_RowData> m_vecData;
#endif
private:		
    BOOL ShowFileDlg(const _tstring& strDefaultPath, const _tstring strFileName,
        const _tstring strTitle,const _tstring strFilter,BOOL bType,
        _tstring& OutputFileName);

    _tstring MakeConnStr(const _tstring& strFileName, bool header = true);

    BOOL CreateExcelApp(BOOL bVisible);
	LPDISPATCH GetRange(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
	_tstring IntToLetter(int n);
    
     
    void WriteCell_(LONG  lColumn,_variant_t vValue);
    void WriteCell_(COleVariant vRow,COleVariant vColumn,COleVariant vValue);
    void AddHyperlinks_(LONG lRow,LONG lColumn,COleVariant vValue,const _tstring& strLink);

    void ExcelReadAllCell_(CRange& range,std::vector<Struct_RowData>& vecData);
    VARIANT ReadCell_(LONG lRow,LONG lColumn);
    VARIANT ReadCell_(const _tstring& szCell);
public:
    //�򿪱���Ի���
    BOOL ShowSaveFile(const _tstring strFileName,
        const _tstring strTitle,
        _tstring& OutputFileName);
    
    //����excel���뵼��·��
    void SetFilePath(const _tstring& strFileName){m_strFileFullPath = strFileName;}

    //�����µ�excel�ļ�
    BOOL NewExcelApp(BOOL bVisible);	
    //���sheet
    void AddWorkSheet(const _tstring& strSheetName,LONG lSheetNum);
   
    //����excel�ļ�
    void SaveFile();
    
    //����excel������Χ
    void SetRange(LONG lRowCount,LONG lColumnCount);
    //����excel������Χ����Ԫ��߶�
    void SetRange(LONG lRowCount,LONG lColumnCount,DOUBLE dRowHeight);
    //��excel�������ĵ�Ԫ��߿�Ӵ�
    void SetBorder();
    //����excel����
    void SetTitle(const Struct_Title& struTitle);
    //����excelע��
    void SetComment(const Struct_Comment& struComment);
    //����excel��ͷ
    void AddHeaders(std::vector<Struct_Header>& vecHeader);

    //��ʼexcelд�����,����ADO��ʽ����excel��Ҫ���ô˷���-----------
    void BeginWrite();
	//����д����������ADO��ʽ����Ҫ���ô˷���
    void EndWrite();
	//��ʼ��д�룬����ADO��ʽ����Ҫ���ô˷���
    void StartWriteRow(LONG lRow);
	//������д�룬����ADO��ʽ����Ҫ���ô˷���
    void EndWriteRow();
	
    //��Ԫ��д������
    void WriteCell(LONG lRow,LONG lColumn,LONG lValue);
    void WriteCell(LONG lRow,LONG lColumn,DOUBLE dValue);
    void WriteCell(LONG lRow,LONG lColumn,const _tstring& strValue);

    //��Ԫ����ӳ����ӣ��ļ�����ַ��   
    void AddHyperlinks(LONG lRow,LONG lColumn,const _tstring& strValue,const _tstring& strLink);  
    void AddHyperlinks(LONG lRow,LONG lColumn,const _tstring& strLink);


    //�ϲ���Ԫ��
    void MergeCell(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //��Ԫ�����ͼƬ
    void AddPicture(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,const _tstring& strPicPath);
    //�ı��Ӵ�
    void SetTextBold(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //�����ı���ɫ
    void SetTextColor(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,COLORREF colorValue);
    //���ñ�����ɫ
    void SetBKColor(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,COLORREF colorValue);
    //���������С
    void SetFontSize(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,int nFontSize);

    //�ı��Ҷ���
    void SetHAlignRight(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //�ı������
    void SetHAlignLeft(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //�ı�����
    void SetHAlignCenter(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);


public:
	//��ѡ��Ի���
	BOOL ShowOpenFile(const _tstring strFileName,
		const _tstring strTitle,
		_tstring& OutputFileName);

    //���Ѿ����ڵ�excel�ļ�---------------------------------------------------------
    BOOL OpenExcelApp(const _tstring& strSheetName,BOOL bVisible = FALSE);   
    //��sheet
    void OpenWorkSheet(const _tstring& strSheetName);
  
    //������ȡexcel����
    void ReadAllCell(std::vector<Struct_RowData>& vecData);
    void ReadAllCell(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,
        std::vector<Struct_RowData>& vecData);

    //��ʼ��ȡexcel���ݣ�����ADO��ʽ��cell��Ҫ���ô˷���
    bool BeginRead();
	//��ȡ�ܵ��к�
    LONG GetSheetRowCount();
	//��ȡ�ܵ��к�
    LONG GetSheetColumnCount();
    //����Ԫ������
    void ReadCell(LONG lRow,LONG lColumn,_tstring& strValue);
    void ReadCell(LONG lRow,LONG lColumn,LONG& lValue);
    void ReadCell(LONG lRow,LONG lColumn,double& dValue);
    void ReadCell(LONG lRow,LONG lColumn,COleDateTime& dateTime);

    //�ر�excel�ļ�
    void CloseFile();

public:
	static _tstring StdtstringFormat(const _TCHAR *fmt,... ) ;

private:
    inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};
    void OutPutError(const _tstring& caller, _com_error & e);
};

