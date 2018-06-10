/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  MFCPlayerDlg.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/10 
 *  
 *  ���������Թ���
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCPlayerDlg �Ի���



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


// CMFCPlayerDlg ��Ϣ�������

BOOL CMFCPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	createPlayer();
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCPlayerDlg::OnBnClickedButtonPlay()
{
	if (getPlayer() == NULL) {
		AfxMessageBox(L"��һ�ᣬ��������û��������");
		return;
	}

	// ��ʼ����
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
	// ֹͣ����
	KillTimer(1000);
	getPlayer()->stop();
	mMsgListCtrl.InsertString(0, L"stop");
}
void CMFCPlayerDlg::OnBnClickedButtonFast()
{
	// ����
	uint32_t speed=getPlayer()->getSpeed();
	getPlayer()->setSpeed(speed + 5);
	CString info;
	info.Format(L"���٣�%d", speed + 5);
	mMsgListCtrl.InsertString(0, info);
}
void CMFCPlayerDlg::OnBnClickedButton4()
{	
	// ����
	uint32_t speed = getPlayer()->getSpeed();
	getPlayer()->setSpeed(speed - 5);

	CString info;
	info.Format(L"���٣�%d", speed - 5);
	mMsgListCtrl.InsertString(0, info);
}


void CMFCPlayerDlg::OnBnClickedButtonSeek()
{
	//���һ��	
	int64_t cur=getPlayer()->getPositionUs();
	getPlayer()->setPositionUs(cur+ 5*1000*1000);

	CString info;
	info.Format(L"���һ�£�%d", cur + 5 * 1000 * 1000);
	mMsgListCtrl.InsertString(0, info);
}


void CMFCPlayerDlg::OnBnClickedButtonSeek2()
{
	//����һ��
	int64_t cur = getPlayer()->getPositionUs();
	getPlayer()->setPositionUs(cur - 5 * 1000 * 1000);

	CString info;
	info.Format(L"����һ�£�%d", cur - 5 * 1000 * 1000);
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
		(LPCTSTR)_TEXT("��Ƶ�ļ� (*.MP4)|*.*|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		mVideoFile = dlg.GetPathName(); //�ļ�����������FilePathName��
	}
	else
	{
		mVideoFile = L"";;
	}
}
