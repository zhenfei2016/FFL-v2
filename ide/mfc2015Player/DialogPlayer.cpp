// DialogPlayer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCPlayer.h"
#include "DialogPlayer.h"
#include "afxdialogex.h"

#pragma comment(lib, "FFL.lib")
#pragma comment(lib, "playerlib.lib")
// CDialogPlayer �Ի���

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


// CDialogPlayer ��Ϣ�������
