#pragma once


// CDialogPlayer �Ի���

class CDialogPlayer : public CDialog
{
	DECLARE_DYNAMIC(CDialogPlayer)

public:
	CDialogPlayer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPlayer();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
