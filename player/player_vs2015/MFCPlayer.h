
// MFCPlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCPlayerApp: 
// �йش����ʵ�֣������ MFCPlayer.cpp
//

class CMFCPlayerApp : public CWinApp
{
public:
	CMFCPlayerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCPlayerApp theApp;