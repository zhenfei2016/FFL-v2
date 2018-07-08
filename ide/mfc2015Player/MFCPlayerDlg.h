
// MFCPlayerDlg.h : 头文件
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "DialogPlayer.h"
#include "Player.hpp"


class TestListener;
// CMFCPlayerDlg 对话框
class CMFCPlayerControlDlg : public CDialogEx ,public player::IPlayerListener
{
// 构造
public:
	CMFCPlayerControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 视频大小改变了
	// widht:宽度 ，height：高度
	// aspectRatioNum/aspectRatioDen  宽高比
	//
	virtual void onVideoSizeChanged(int32_t width, int32_t height, int32_t aspectRatioNum, int32_t aspectRatioDen);
	//
	//  视频播放结束回调
	//
	virtual void onComplete();
	//
	//  seek结束回调
	//
	virtual void onSeekComplete(int64_t pos, int64_t duration);
	//
	// 错误码
	//
	virtual void onError(int32_t errCode, int32_t errVal);
	//
	// 缓冲中
	//
	virtual void onBufferingStart();
	virtual void onBufferingUpdate();
	//
	// 一些其他的消息
	//
	virtual void onMessage(int32_t msg, int32_t parma1, int32_t param2);

public:	
	CSliderCtrl mPlayerSlider;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);	
};
