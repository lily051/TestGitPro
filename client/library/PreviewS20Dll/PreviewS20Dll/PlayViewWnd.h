#pragma once
/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			PlayViewWnd.h
Description:  		Ԥ������ ����Ԥ�����Ŵ��ں�Ԥ��������
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-4-7     qmx    ����
*****************************************************/
// CPlayViewWnd
#include "PlayViewToolbar.h"
#include "PlayView.h"
#include "LayoutMoreWnd.h"


class CPlayViewWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayViewWnd)

public:
	CPlayViewWnd();
	virtual ~CPlayViewWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CWnd *				m_pParentWnd;					//������ָ�� ȫ��ʱ���õ�
	CWnd *				m_pFullScreenWnd;				//ȫ����������
	CPlayView *			m_pPlayView;					//���Ŵ���
	CPlayViewToolbar *	m_pToolBarWnd;					//������
	CLayoutMoreWnd *	m_pLayoutMoreWnd;				//��չ�ָ�ģʽ����
	BOOL				m_bWndFullScreen;               //���Ŵ����Ƿ�ȫ��
	BOOL				m_bIsWndInit;					//�����Ƿ��ʼ�� �ؼ����ƶ���־
	WORD				m_wLayoutMode;                  //��ǰ�ָ�ģʽ
	UINT				m_nWndNum;                      //��ǰ������
	CRect				m_rcWndPos;                     //���洰��ȫ��ǰ��λ��
	PREVIEW_CAMINFO m_stCamInfo;                //��ص���Ϣ

public:
	afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateBtnState(WPARAM wParam, LPARAM lParam);

	//��������
	BOOL CreateWnd(const RECT & rect, CWnd * pParent, unsigned int uiID,DWORD dwSyle,DWORD dwFunStyle = 0);
	
	//��������Դ
	void CreateChildWnd();

	//��ʼ��λ��
	void InitPose();

	//���û���ָ�ģʽ
	BOOL SetLayoutMode(WORD wMode);
	
	//����ȫ��/�ָ�ģʽ
	BOOL SetPlayViewWndFullScreen();

	//��Ӧ������btn�¼�
	LRESULT OnToolBarBtnClk(WPARAM wParam, LPARAM lParam);

	//��Ӧ������btn�¼�
	LRESULT OnToolBarBtnExecute(WPARAM wParam, LPARAM lParam);

	// �Ӵ��ڷ�����ù�����
	LRESULT OnToolBarBtnClkRef(WPARAM wParam, LPARAM lParam);
	
	//������չ�ָ��
	void OnClkLayoutMoreBtn();

	//������չ�ָ��
	LRESULT OnLayoutMoreWndClose(WPARAM wparam, LPARAM lparam);
	
	//��ѡ�еĴ�����Ԥ��
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//ֹͣԤ��
	void StopPreview();

	//��ʼ¼��
	BOOL StartRecord();

	//ֹͣ¼��
	BOOL StopRecord();

	//������Ƶ
    BOOL StartSound();

	//�ر���Ƶ
    BOOL StopSound();

	//��ȡ¼���ļ���Ϣ
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	//��ʾOSD��Ϣ
	BOOL SetShowString(const CString &strBusinessInfo);

	BOOL OpenSound();


private:
	vector<int> m_veCamID;					//�������������ص�ID
	vector<int>::iterator m_iterFirstNonPreview;		//���ڲ�������ص����һ��λ��
	vector<int>::iterator m_iterFirstPreview;			//���ڲ��ŵĵ�һ����ص�
	int m_nResID;							//Ԥ����ѡ�����IDֵ
	int	m_nLastChgedResID;
};
