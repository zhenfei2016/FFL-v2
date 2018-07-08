
// MFCPlayerDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "DialogPlayer.h"
#include "Player.hpp"


class TestListener;
// CMFCPlayerDlg �Ի���
class CMFCPlayerControlDlg : public CDialogEx ,public player::IPlayerListener
{
// ����
public:
	CMFCPlayerControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

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
	CDialogPlayer mDialogPlayer;

	TestListener* mPlayerListener;
	player::FFLPlayer mPlayer;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CListBox mMsgListCtrl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonPlay2();
	afx_msg void OnBnClickedButtonPlay4();
	afx_msg void OnBnClickedButtonStop2();

protected:
	void showPlayerInfo(CStringA& info);

	virtual void onPrepared(int suc);
	//
	// ��Ƶ��С�ı���
	// widht:��� ��height���߶�
	// aspectRatioNum/aspectRatioDen  ��߱�
	//
	virtual void onVideoSizeChanged(int32_t width, int32_t height, int32_t aspectRatioNum, int32_t aspectRatioDen);
	//
	//  ��Ƶ���Ž����ص�
	//
	virtual void onComplete();
	//
	//  seek�����ص�
	//
	virtual void onSeekComplete(int64_t pos, int64_t duration);
	//
	// ������
	//
	virtual void onError(int32_t errCode, int32_t errVal);
	//
	// ������
	//
	virtual void onBufferingStart();
	virtual void onBufferingUpdate();
	//
	// һЩ��������Ϣ
	//
	virtual void onMessage(int32_t msg, int32_t parma1, int32_t param2);

public:	
	CSliderCtrl mPlayerSlider;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);	
};
