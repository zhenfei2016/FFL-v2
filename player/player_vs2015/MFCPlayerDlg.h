
// MFCPlayerDlg.h : ͷ�ļ�
//

#pragma once

#include "FFLPlayerWindow.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMFCPlayerDlg �Ի���
class CMFCPlayerDlg : public CDialogEx
{
// ����
public:
	CMFCPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonFast();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonSeek();
	afx_msg void OnBnClickedButtonSeek2();

private:
	
public:
	CString  mVideoFile;
	CProgressCtrl mProgress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox mMsgListCtrl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonPlay2();
};
