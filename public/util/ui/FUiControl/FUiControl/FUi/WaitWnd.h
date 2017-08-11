
#pragma once
#include "FuiBase.h"
#include <vector>


class CWaitWnd : public CWnd
{
	DECLARE_DYNAMIC(CWaitWnd)

public:
	CWaitWnd();
	virtual ~CWaitWnd();

protected:
	DECLARE_MESSAGE_MAP()


public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    // 窗口大小
    CRect m_rcWnd;  
    // 动画图片、背景图片
    Image *m_pImgWait, *m_pImgBkg;
    // 文字
    CString m_strText;
    // 字体显示区域
    CRect m_rcText;
    // 动画图片偏移
    int m_nImageOffset;
    // 动画图片显示区域
    CRect m_rcImage;

    // 九宫格背景图片分割
    std::vector<RectF> m_vecRcfBkg;
    // 九宫格窗口分割
    std::vector<RectF> m_vecRcfWnd;
    
    // 界面刷新线程
    HANDLE m_hExit, m_hThread;

    // 刷新界面
    void SplashStep(CDC* pDC);
    // 重绘背景
    void DrawBkg(CDC* pDC);
public:
    // 创建窗口
    BOOL CreateWnd(CWnd* pParent);
    // 显示窗口
    BOOL ShowWaitWnd(LPCTSTR szText);
    // 刷新线程调用函数
    unsigned UpdateImage();
};


