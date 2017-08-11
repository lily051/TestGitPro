#pragma once

#include "PlayViewWnd.h"
#include "PreviewS20API.h"


// CPreviewMainDlg 对话框

class CPreviewMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewMainDlg)

public:
	CPreviewMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreviewMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PREVIEW_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:

    /*
	*  @fn         CreateChildWnd
    *  @brief      创建预览子窗口
    *  @return     void
    */
    void CreateChildWnd();

    /*
	*  @fn         DeleteChildWnd
    *  @brief      销毁预览子窗口
    *  @return     void
    */
    void DeleteChildWnd();

    /*
	*  @fn         SetPreviewLayoutMode
    *  @brief      设置预览窗口画面分割模式
    *  @param[IN]  nLayoutMode: 画面分割模式
    *  @return     TRUE: 成功 FALSE: 失败
    */
    BOOL SetPreviewLayoutMode(int nLayoutMode);

    /** @fn         StartPlayOnSelWnd
    *  @brief      在选中的窗口中开始预览
    *  @param[IN]  struPreviewInfo: 预览参数信息
    *  @return     TRUE: 成功 FALSE: 失败
    */
    BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

    /** @fn         SetWinPos
    *  @brief      设置预览窗口位置
    *  @return     void
    */
    void SetWinPos();

    /*
	*  @fn         StopPreview
    *  @brief      停止所有预览
    *  @return     TRUE: 成功 FALSE: 失败
    */
    BOOL StopPreview();

	//开始录像
	BOOL StartRecord();

	//停止录像
	BOOL StopRecord();

	//获取录像文件信息
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	//设置OSD显示信息
	BOOL SetShowString(const CString &strBusinessInfo);

	//打开声音
	BOOL OpenSound();
private:
    BOOL m_bIsWndInit;
    CPlayViewWnd *m_pPlayviewWnd;      // 预览视频窗口

};
