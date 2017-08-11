/**	@file Excel.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief excel操作类。
*
*	@author		puzhibing
*	@date		2013/9/10
*
*	@note 功能描述：
*		  1、支持两种方式导出excel：
*			1.1 MS Office Excel工具，必须安装Office Excel应用程序；
*			1.2 ADO方式，如果没有安装MS Office，需要安装ACCESS连接驱动（AccessDatabaseEngine.exe），还需要定义宏 "EXCEL_ADO"
*		  2、ADO方式只能做基本的excel导入导出操作
*		  3、MS Office Excel工具导出excel支持：
*			3.1、设置字体大小、颜色、样式、对齐方式
*			3.2、设置单元格高度、背景颜色
*			3.3、合并单元格
*			3.4、插入图片
*			3.5、添加超链接
*		  4、支持Excel2003、2007的导入导出
*	@note 历史记录：
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

//标题信息
typedef struct _Struct_Title
{
	LONG lStartRow;            //起始行号 1 开始
	LONG lStartColumn;         //起始列号 1 开始
	LONG lEndRow;              //结束行号
	LONG lEndColumn;           //结束列号
	_tstring strName;           //名称
	BOOL bBold;                //是否粗体字
	LONG lFontSize;            //字体大小
	BOOL lHAlign;              //0-左对齐；1-居中对齐；2-右对齐
}Struct_Title;

//注释信息
typedef struct _Struct_Comment
{
	LONG lStartRow;            //起始行号 1 开始
	LONG lStartColumn;         //起始列号 1 开始
	LONG lEndRow;              //结束行号
	LONG lEndColumn;           //结束列号
	_tstring strName;           //名称
	BOOL lHAlign;              //0-左对齐；1-居中对齐；2-右对齐
}Struct_Comment;

//列头信息
typedef struct _Struct_Header
{
	LONG lRow;                 //行号 1 开始
	LONG lColumn;              //列号 1 开始
	_tstring strName;           //名称
	LONG lWidth;               //宽度
}Struct_Header;

//数据
typedef struct _Struct_RowData
{
    std::vector<_tstring> vecRowData;
}Struct_RowData;

//excel操作类型
typedef enum _Enum_OperateType
{
    EXCEL_OT_MS_EXCEL,
    #if defined EXCEL_ADO
    EXCEL_OT_ADO,
    #endif
}Enum_OperateType;

//excel版本格式
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

    //数据行数
    LONG m_lSheetRowCount;
    //数据列数
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
    //打开保存对话框
    BOOL ShowSaveFile(const _tstring strFileName,
        const _tstring strTitle,
        _tstring& OutputFileName);
    
    //设置excel导入导出路径
    void SetFilePath(const _tstring& strFileName){m_strFileFullPath = strFileName;}

    //创建新的excel文件
    BOOL NewExcelApp(BOOL bVisible);	
    //添加sheet
    void AddWorkSheet(const _tstring& strSheetName,LONG lSheetNum);
   
    //保存excel文件
    void SaveFile();
    
    //设置excel操作范围
    void SetRange(LONG lRowCount,LONG lColumnCount);
    //设置excel操作范围及单元格高度
    void SetRange(LONG lRowCount,LONG lColumnCount,DOUBLE dRowHeight);
    //将excel工作区的单元格边框加粗
    void SetBorder();
    //设置excel标题
    void SetTitle(const Struct_Title& struTitle);
    //设置excel注释
    void SetComment(const Struct_Comment& struComment);
    //设置excel列头
    void AddHeaders(std::vector<Struct_Header>& vecHeader);

    //开始excel写入操作,采用ADO方式导出excel需要调用此方法-----------
    void BeginWrite();
	//结束写操作，采用ADO方式，需要调用此方法
    void EndWrite();
	//开始行写入，采用ADO方式，需要调用此方法
    void StartWriteRow(LONG lRow);
	//结束行写入，采用ADO方式，需要调用此方法
    void EndWriteRow();
	
    //单元格写入数据
    void WriteCell(LONG lRow,LONG lColumn,LONG lValue);
    void WriteCell(LONG lRow,LONG lColumn,DOUBLE dValue);
    void WriteCell(LONG lRow,LONG lColumn,const _tstring& strValue);

    //单元格添加超链接（文件或网址）   
    void AddHyperlinks(LONG lRow,LONG lColumn,const _tstring& strValue,const _tstring& strLink);  
    void AddHyperlinks(LONG lRow,LONG lColumn,const _tstring& strLink);


    //合并单元格
    void MergeCell(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //单元格插入图片
    void AddPicture(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,const _tstring& strPicPath);
    //文本加粗
    void SetTextBold(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //设置文本颜色
    void SetTextColor(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,COLORREF colorValue);
    //设置背景颜色
    void SetBKColor(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,COLORREF colorValue);
    //设置字体大小
    void SetFontSize(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,int nFontSize);

    //文本右对其
    void SetHAlignRight(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //文本左对齐
    void SetHAlignLeft(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);
    //文本居中
    void SetHAlignCenter(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn);


public:
	//打开选择对话框
	BOOL ShowOpenFile(const _tstring strFileName,
		const _tstring strTitle,
		_tstring& OutputFileName);

    //打开已经存在的excel文件---------------------------------------------------------
    BOOL OpenExcelApp(const _tstring& strSheetName,BOOL bVisible = FALSE);   
    //打开sheet
    void OpenWorkSheet(const _tstring& strSheetName);
  
    //批量读取excel数据
    void ReadAllCell(std::vector<Struct_RowData>& vecData);
    void ReadAllCell(LONG lStartRow,LONG lStartColumn,LONG lEndRow,LONG lEndColumn,
        std::vector<Struct_RowData>& vecData);

    //开始读取excel数据，采用ADO方式读cell需要调用此方法
    bool BeginRead();
	//获取总的行号
    LONG GetSheetRowCount();
	//获取总的列号
    LONG GetSheetColumnCount();
    //读单元格内容
    void ReadCell(LONG lRow,LONG lColumn,_tstring& strValue);
    void ReadCell(LONG lRow,LONG lColumn,LONG& lValue);
    void ReadCell(LONG lRow,LONG lColumn,double& dValue);
    void ReadCell(LONG lRow,LONG lColumn,COleDateTime& dateTime);

    //关闭excel文件
    void CloseFile();

public:
	static _tstring StdtstringFormat(const _TCHAR *fmt,... ) ;

private:
    inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};
    void OutPutError(const _tstring& caller, _com_error & e);
};

