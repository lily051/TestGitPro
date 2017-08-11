#pragma once
#include "hcs_sdk.h"
#include <string>
#include "afxcmn.h"
#include <vector>
#include "afxwin.h"
#include "plaympeg4.h"
// CVideoWriteFile �Ի���
#define MAX_LEN    256
#define HEAD_SIZE 40
#define MAX_PLAYDATA_LEN 256 * 1024
class CVideoWriteFile : public CPropertyPage
{
	DECLARE_DYNAMIC(CVideoWriteFile)

public:
	CVideoWriteFile();
	virtual ~CVideoWriteFile();

// �Ի�������
	enum { IDD = IDD_DIALOG_WRITE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
private:
    CString m_camerID;
    CString m_poolID;
    CString m_repoolID;
    UINT m_nType;
    UINT m_nReNum;

    CArray<CString, CString> ary_filename;//��CString�������ļ���·��
    CArray<CString, CString> ary_fileTitle;//����ļ��ı���

    unsigned long long m_nLogID;//��¼���
    int m_write_handle;//д�ļ����ؾ��
    int m_ItemIndex;
    int m_ItemIndexInfo;
    std::vector<HCS_PoolInfo> VecPoolInfo;
    int m_handlePlayback;//�طŲ������
    int m_handleDownLoad;
    LONG m_nPort;
public:
    afx_msg void OnBnClickedButtonSelectFile();
    afx_msg void OnBnClickedButtonStartWriteAsy();

    void SetLoginID(unsigned long long m_nLogID);
    void GetPoolIDInfo(std::vector<HCS_PoolInfo> &PoolInfo){VecPoolInfo = PoolInfo;}
    afx_msg void OnBnClickedButtonStartWriteSyn();
    void StartVideoPlayBack();
private:
    CListCtrl m_listFileInfo;
    CListCtrl m_listInfo;
    bool m_bPlayBacking;
    HANDLE m_hThreadVideoPlayBack;

public:
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonVideoDownload();
    char m_FileName[MAX_LEN];
    int m_download_error_code;
    int m_download_flag;
    int m_writefile_speed;//�ļ��ϴ�����
    int m_writefile_error_code;//����������
    afx_msg void OnBnClickedButtonPlayback();
    CStatic m_PlayShow;
    afx_msg void OnBnClickedButtonPalybackStop();
    afx_msg void OnBnClickedButtonVideoFind();
    afx_msg void OnBnClickedButtonVideoDelete();
    afx_msg void OnBnClickedButtonVideoRead();
    afx_msg void OnBnClickedButtonIndexSearch();

    std::vector<HCS_FileIndexData> vecFileIndexData;
};
