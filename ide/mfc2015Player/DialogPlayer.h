#pragma once


// CDialogPlayer 对话框

class CDialogPlayer : public CDialog
{
	DECLARE_DYNAMIC(CDialogPlayer)

public:
	CDialogPlayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPlayer();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
