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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMFCPlayerControlDlg::CMFCPlayerControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCPlayerControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, mProgress);
	DDX_Control(pDX, IDC_LIST1, mMsgListCtrl);
	DDX_Control(pDX, IDC_SLIDER1, mPlayerSlider);
}

BEGIN_MESSAGE_MAP(CMFCPlayerControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMFCPlayerControlDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMFCPlayerControlDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_FAST, &CMFCPlayerControlDlg::OnBnClickedButtonFast)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCPlayerControlDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_SEEK, &CMFCPlayerControlDlg::OnBnClickedButtonSeek)
	ON_BN_CLICKED(IDC_BUTTON_SEEK2, &CMFCPlayerControlDlg::OnBnClickedButtonSeek2)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCPlayerControlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_PLAY2, &CMFCPlayerControlDlg::OnBnClickedButtonPlay2)
	ON_BN_CLICKED(IDC_BUTTON_PLAY4, &CMFCPlayerControlDlg::OnBnClickedButtonPlay4)
	ON_BN_CLICKED(IDC_BUTTON_STOP2, &CMFCPlayerControlDlg::OnBnClickedButtonStop2)	
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CMFCPlayerControlDlg::OnNMReleasedcaptureSlider1)
END_MESSAGE_MAP()


// CMFCPlayerControlDlg ��Ϣ�������
BOOL CMFCPlayerControlDlg::OnInitDialog()
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
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCPlayerControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCPlayerControlDlg::OnPaint()
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
HCURSOR CMFCPlayerControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCPlayerControlDlg::OnBnClickedButtonPlay()
{
	HWND hWnd = mDialogPlayer.GetSafeHwnd();	
	mDialogPlayer.DestroyWindow();
	mDialogPlayer.Create(IDD_DIALOG_PLAYER);
	mDialogPlayer.ShowWindow(SW_SHOW);
	hWnd = mDialogPlayer.GetSafeHwnd();
	
	mPlayer.create(hWnd);
	mPlayer.setListener(this);

	CStringA movicUrl="d://movic//sintel.ts";
	if (!mVideoFile.IsEmpty()) {
		movicUrl = mVideoFile;
	}
	mPlayer.setUrl(movicUrl);
	mPlayer.prepare();

	mMsgListCtrl.ResetContent();
	mMsgListCtrl.InsertString(0,L"��ʼ����");

	SetTimer(1000, 1000,NULL);
}
void CMFCPlayerControlDlg::OnBnClickedButtonStop()
{
	mPlayer.stop();
	// ֹͣ����
	KillTimer(1000);	
	mMsgListCtrl.InsertString(0, L"ֹͣ����");
}
void CMFCPlayerControlDlg::OnBnClickedButtonFast()
{
	//// ����	
	uint32_t speed = mPlayer.getSpeed();
	speed = FFL_MIN(speed + 5, 300);
	mPlayer.setSpeed(speed);

	CString info;
	info.Format(L"�����ٶȣ�%d", speed);
	mMsgListCtrl.InsertString(0, info);
}
void CMFCPlayerControlDlg::OnBnClickedButton4()
{	
	//// ����
	uint32_t speed = mPlayer.getSpeed();
	speed = FFL_MAX(speed - 5, 10);
	mPlayer.setSpeed(speed);

	CString info;
	info.Format(L"�����ٶȣ�%d", speed );
	mMsgListCtrl.InsertString(0, info);
}


void CMFCPlayerControlDlg::OnBnClickedButtonSeek()
{
	////���һ��	
	int64_t cur = mPlayer.getCurrentPosition();
	int64_t target = cur + 2000 * 1000 * 60;
	if (target > mPlayer.getDuration()) {
		target = mPlayer.getDuration() - 10;
	}
	mPlayer.seekTo(target);

	CString info;
	info.Format(L"���һ�£�%" lld64, target);
	mMsgListCtrl.InsertString(0, info);
}


void CMFCPlayerControlDlg::OnBnClickedButtonSeek2()
{
	////����һ��

	int64_t cur = mPlayer.getCurrentPosition();
	int64_t target = cur - 2000 * 1000 * 60;
	if (target <= 0) {
		target = 0;
	}
	mPlayer.seekTo(target);

	CString info;
	info.Format(L"����һ�£�%" lld64, target);
	mMsgListCtrl.InsertString(0, info);
}



void CMFCPlayerControlDlg::OnTimer(UINT_PTR nIDEvent)
{	
	if (nIDEvent == 1000) {
		if (1) {
			int den = 1000;
			int64_t duration = mPlayer.getDuration();
			if (duration >= 0x0FFFFFFF) {
				den = 1000000;
			}
			int64_t cur = mPlayer.getCurrentPosition();


			float fNUm = duration>0 ? (float)cur / duration:1;
			int nRangeMax = 10000;
			int32_t pos= fNUm*nRangeMax;


			mProgress.SetRange(0, nRangeMax);
			mProgress.SetPos(pos);

			CString info;
			info.Format(L"%I64d / %I64d", cur, duration);
			this->SetDlgItemText(IDC_STATIC_PROGRESS, info);


			mPlayerSlider.SetTicFreq(10);//�̶��߼�� 
			mPlayerSlider.SetPos(20);//���û���λ�� 
								 //��Χ..100 ��-100,100 
								 //m_slider1.SetRange(-100,100); 
			mPlayerSlider.SetRangeMin(0);
			mPlayerSlider.SetRangeMax(nRangeMax);
			mPlayerSlider.SetPos(pos);
			mPlayerSlider.SetLineSize(2); //��Թ���� 
			mPlayerSlider.SetPageSize(10); //�������page down,page up��Ϣ
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMFCPlayerControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	if ((&mPlayerSlider) == (CSliderCtrl *)pScrollBar)         // mSlider Ϊ���slider�ؼ�
	{
		
	}

}

void CMFCPlayerControlDlg::OnBnClickedButton1()
{
	mMsgListCtrl.ResetContent();
}


void CMFCPlayerControlDlg::OnBnClickedButtonPlay2()
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
		mVideoFile = dlg.GetPathName();		
	}
	else
	{
		mVideoFile = L"";;
	}

	CString info;
	info.Format(L"ѡ���ļ�:%s", mVideoFile);
	mMsgListCtrl.InsertString(0, info);	
}


void CMFCPlayerControlDlg::OnBnClickedButtonPlay4()
{
	mPlayer.pause(1);	
	mMsgListCtrl.InsertString(0, L"�����ͣ");
}


void CMFCPlayerControlDlg::OnBnClickedButtonStop2()
{
	mPlayer.pause(0);
	mMsgListCtrl.InsertString(0, L"�������");
}

void CMFCPlayerControlDlg::showPlayerInfo(CStringA& info)
{
	CString tmp;
	tmp = info;
	mMsgListCtrl.InsertString(0, tmp);
}
void CMFCPlayerControlDlg::onPrepared(int suc) {
	CStringA info;
	info.Format("onPrepared succ=%d",suc);
	showPlayerInfo(info);
	if (suc) {
		mPlayer.start();
	}
}
//
// ��Ƶ��С�ı���
// widht:��� ��height���߶�
// aspectRatioNum/aspectRatioDen  ��߱�
//
void CMFCPlayerControlDlg::onVideoSizeChanged(int32_t width, int32_t height, int32_t aspectRatioNum, int32_t aspectRatioDen) {
	printf("onVideoSizeChanged: width=%d height=%d aspectRatio=%d/%d\n",
		width, height,
		aspectRatioNum,
		aspectRatioDen);
	//mPlayer->setSurfaceSize(width,height);
}
//
//  ��Ƶ���Ž����ص�
//
void CMFCPlayerControlDlg::onComplete() {
	CStringA info;
	info.Format("onComplete:  \n");
	showPlayerInfo(info);
}
//
//  seek�����ص�
//
void CMFCPlayerControlDlg::onSeekComplete(int64_t pos, int64_t duration) {
	char durationBuf[256] = {};
	FFL_usToString(duration, durationBuf);

	char posBuf[256] = {};
	FFL_usToString(pos, posBuf);
	CStringA info;
	info.Format("onSeekComplete: pos=%s duration=%s \n", posBuf, durationBuf);
	showPlayerInfo(info);
}
//
// ������
//
void CMFCPlayerControlDlg::onError(int32_t errCode, int32_t errVal) {
	CStringA info;
	info.Format("onSeekComplete: errCode=%d errVal=%d \n", errCode, errVal);
	showPlayerInfo(info);
}
//
// ������
//
void CMFCPlayerControlDlg::onBufferingStart() {
	CStringA info;
	info.Format("onBufferingStart:  \n");
	showPlayerInfo(info);
}
void CMFCPlayerControlDlg::onBufferingUpdate() {
	CStringA info;
	info.Format("onBufferingUpdate:  \n");
	showPlayerInfo(info);
}
//
// һЩ��������Ϣ
//
void CMFCPlayerControlDlg::onMessage(int32_t msg, int32_t parma1, int32_t param2) {
	CStringA info;
	info.Format("onMessage: msg=%d param1=%d parma2=%d\n", msg, parma1, param2);
	showPlayerInfo(info);
}

void CMFCPlayerControlDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos=  mPlayerSlider.GetPos();
	
	*pResult = 0;
}


