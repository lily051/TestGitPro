/**   @file		FDialog.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    自绘对话框,
 *
 *    @author	jddd
 *    @date		2015/07/28
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	1、还不支持可以拖拽改变大小
                2、XP下没有经过测试
                3、Win7 basic 下未测试
 */


#pragma once

#include "FuiBase.h"
#include "FNcButton.h"
#include "fuilib.h"
#include "WndShadow.h"
#include <map>

// CFDialog 对话框

class AFX_EXT_CLASS CFDialog : public CDialog
{
	DECLARE_DYNAMIC(CFDialog)

public:
    CFDialog(UINT nIDTemplate, CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CFDialog();

protected:
    DECLARE_MESSAGE_MAP()

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();

protected:	
    // 重新设置窗口的边框（包括标题栏）的尺寸
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    // 设置窗口最小尺寸
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    // 处理窗口大小改变时，按钮的位置的改变
    afx_msg void OnSize(UINT nType, int cx, int cy);
    // 非客户区绘制消息
    afx_msg void OnNcPaint();
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    afx_msg void OnNcMouseLeave();
    afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
    // 窗口激活消息
    afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
    // 非客户区点击
    afx_msg LRESULT OnNcHitTest(CPoint point);

    // 客户区绘制消息
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

    afx_msg void OnPaint();

	// 增加控件鼠标提示消息
	LRESULT OnMsgCtrlToolTip(WPARAM, LPARAM);
	LRESULT OnMsgCtrlToolTipEvent(WPARAM, LPARAM);

	void SetToolTipRect(DWORD dwControlID, CRect rcControl);
private:
    // 自绘非客户区
    void DrawNc(CDC* pDC);
    // 绘制对话框背景
    void DrawDialog(CDC* pDC);
    // 初始化系统按钮
    void InitSysBtn();
    // 销毁系统按钮
    void DestorySysBtn();
    // 对话框改变大小后移动系统按钮的位置
    void MoveSysBtn();

    // 判断鼠标指针是否在按钮上
    int SysBtnHitTest(CPoint pt);

private:
    // 控件名称
    CString m_strObjectName;
    // 标题文字的字体
    CString m_strCaptionFont;
    // 标题文字的颜色
    COLORREF m_clrTitle;
    // 标题文字的对齐方式：DT_LEFT，DT_CENTER
    int m_nTitleAlign;

    // 背景图片
    Image *m_pImgBkg;
    // 背景图片切割尺寸（9宫格）
    std::vector<int> m_vecMargin;
    // 背景图片切割后的区域
    std::vector<RectF> m_vecRcfBkg;
  
    // 自定义按钮<ID, ptr>
    std::map<int, CFNcButton*> m_mapSysBtn;
    CPPToolTip m_toolTip;

    // 图标热区
    CRect m_rcIcon;

    // 跟踪mouseleave消息
    BOOL m_bNcTracking;
    // 当前处于hover状态的按钮
    int m_nBtnHover;

    // 窗口边框处的阴影
    CWndShadow m_shadow;

    // 圆角
    CRgn m_Rgn;
public:
    // 设置控件名称
	void SetObjectName(LPCTSTR szObjectName);

    // 对话框标题的对齐方式
    void SetTitleAlign(int nAlign);

    // 设置标题的字体
    void SetTitleFont(const CString& strFontName);

    // 设置标题文字的颜色
    void SetTitleColor(COLORREF clrTitle);

    // 设置对话框的背景图片
    BOOL SetMargin(std::vector<int>& vecMargin);

    // 标题栏增加自定义按钮
    /*
       如何接收自定义标题栏按钮的点击消息：
       在对话框类中，重载 OnCommand 的实现，wParam 传出的就是自定义按钮的ID
       传入的按钮ID应该> 505
       按钮的顺序是根据ID排序的：大的在左，小的在右
    */
     CFNcButton * AddCustomBtnToSysBtn(LPCTSTR szObjectName, LPCTSTR szText, 
        LPCTSTR szBtnTip, const int nID, const int nFixedWidth = 0);

    // 从标题栏删除自定义的按钮
    // 该接口只是不管理自定义的按钮，按钮的销毁和删除还是需要上层自己搞定
    void DeleteCustomBtn(const int nID);

	LRESULT  ProcMsg(MSG msg);

	void SetDrawTitleIcon(BOOL bDraw = TRUE);

	void SetTitleLeftMargin(int nLeftMargin);

    void SetTitleTextLeft(int nLeft);
    //void SetShadow();
protected:
	CRect m_rcTitle;

	//是否需要绘制标题栏图标
	//默认绘制，在某些情况下设为FALSE表示不需要对话框绘制图标
	BOOL m_bDrawTitleIcon;

	//标题栏图标左边距
	int m_nTitleLeftMargin;
    //标题栏标题文字左边距
    int m_nTitleTextLeft;

};
