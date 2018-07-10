// DialogPlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCPlayer.h"
#include "DialogPlayer.h"
#include "afxdialogex.h"

#pragma comment(lib, "FFL.lib")
#pragma comment(lib, "playerlib.lib")
// CDialogPlayer 对话框

IMPLEMENT_DYNAMIC(CDialogPlayer, CDialog)

CDialogPlayer::CDialogPlayer(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PLAYER, pParent)
{
}

CDialogPlayer::~CDialogPlayer()
{
}

void CDialogPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPlayer, CDialog)
END_MESSAGE_MAP()


// CDialogPlayer 消息处理程序
