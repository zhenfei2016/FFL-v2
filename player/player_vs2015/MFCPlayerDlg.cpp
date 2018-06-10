/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  MFCPlayerDlg.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/10 
 *  
 *  播放器测试工程
 *
*/

#include "stdafx.h"
#include "MFCPlayer.h"
#include "MFCPlayerDlg.h"
#include "afxdialogex.h"
#include "FFLPlayerWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCPlayerDlg 对话框



CMFCPlayerDlg::CMFCPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, mProgress);
	DDX_Control(pDX, IDC_LIST1, mMsgListCtrl);
}

BEGIN_MESSAGE_MAP(CMFCPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMFCPlayerDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMFCPlayerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_FAST, &CMFCPlayerDlg::OnBnClickedButtonFast)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCPlayerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_SEEK, &CMFCPlayerDlg::OnBnClickedButtonSeek)
	ON_BN_CLICKED(IDC_BUTTON_SEEK2, &CMFCPlayerDlg::OnBnClickedButtonSeek2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCPlayerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_PLAY2, &CMFCPlayerDlg::OnBnClickedButtonPlay2)
END_MESSAGE_MAP()


// CMFCPlayerDlg 消息处理程序

BOOL CMFCPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	createPlayer();
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCPlayerDlg::OnBnClickedButtonPlay()
{
	if (getPlayer() == NULL) {
		AfxMessageBox(L"等一会，播放器还没创建出来");
		return;
	}

	// 开始播放
	if (mVideoFile.IsEmpty()) {
		getPlayer()->play("d://movic//sintel.ts");
	}
	else {
		CStringA url;
		url = mVideoFile;
		getPlayer()->play(url);
	}
	SetTimer(1000, 500,NULL);
	mMsgListCtrl.ResetContent();
	mMsgListCtrl.InsertString(0,L"play");
}
void CMFCPlayerDlg::OnBnClickedButtonStop()
{
	// 停止播放
	KillTimer(1000);
	getPlayer()->stop();
	mMsgListCtrl.InsertString(0, L"stop");
}
void CMFCPlayerDlg::OnBnClickedButtonFast()
{
	// 加速
	uint32_t speed=getPlayer()->getSpeed();
	getPlayer()->setSpeed(speed + 5);
	CString info;
	info.Format(L"加速：%d", speed + 5);
	mMsgListCtrl.InsertString(0, info);
}
void CMFCPlayerDlg::OnBnClickedButton4()
{	
	// 减速
	uint32_t speed = getPlayer()->getSpeed();
	getPlayer()->setSpeed(speed - 5);

	CString info;
	info.Format(L"减速：%d", speed - 5);
	mMsgListCtrl.InsertString(0, info);
}


void CMFCPlayerDlg::OnBnClickedButtonSeek()
{
	//快进一下	
	int64_t cur=getPlayer()->getPositionUs();
	getPlayer()->setPositionUs(cur+ 5*1000*1000);

	CString info;
	info.Format(L"快进一下：%d", cur + 5 * 1000 * 1000);
	mMsgListCtrl.InsertString(0, info);
}


void CMFCPlayerDlg::OnBnClickedButtonSeek2()
{
	//快退一下
	int64_t cur = getPlayer()->getPositionUs();
	getPlayer()->setPositionUs(cur - 5 * 1000 * 1000);

	CString info;
	info.Format(L"快退一下：%d", cur - 5 * 1000 * 1000);
	mMsgListCtrl.InsertString(0, info);
}



void CMFCPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{	
	if (nIDEvent == 1000) {
		player::FFLPlayer* player=getPlayer();
		if (player) {
			int64_t duration=player->getDurationUs();
			int64_t cur=player->getPositionUs();
			
			mProgress.SetRange(0, duration / 1000);
			mProgress.SetPos(cur/1000);

			CString info;
			info.Format(L"%I64d / %I64d", cur, duration);
			this->SetDlgItemText(IDC_STATIC_PROGRESS, info);
			
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMFCPlayerDlg::OnBnClickedButton1()
{
	mMsgListCtrl.ResetContent();
}


void CMFCPlayerDlg::OnBnClickedButtonPlay2()
{
	CString FilePathName;
	CFileDialog dlg(TRUE, 
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("视频文件 (*.MP4)|*.*|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		mVideoFile = dlg.GetPathName(); //文件名保存在了FilePathName里
	}
	else
	{
		mVideoFile = L"";;
	}
}
