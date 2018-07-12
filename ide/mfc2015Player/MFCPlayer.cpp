
// MFCPlayer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MFCPlayer.h"
#include "MFCPlayerDlg.h"
#include "Player.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const char* ShowLogTag[] = {
	//"audio",
	//"timestamp",
	//"MsgQueue",
	"debug",
	NULL
};

int printLog(int level, const char* tag, const char *format, va_list v)
{
	bool showLog = false;
	if (level <= FFL_LOG_LEVEL_ERROR) {
		showLog = true;
	}else if (tag) {
		for (int i = 0; i < FFL_ARRAY_ELEMS(ShowLogTag); i++) {
			if (ShowLogTag[i] && strcmp(tag, ShowLogTag[i]) == 0) {
				showLog = true;
				break;
			}
		}
	}

	if (showLog) {
		char str[1024] = {};
		vsnprintf(str + 2, 1024 - 1, format, v);
		str[0] = ' ';
		str[1] = ' ';

		char timeStr[128] = { 0 };
		FFL_getNowString(timeStr);
		::OutputDebugStringA(timeStr);
		::OutputDebugStringA(str);
		::OutputDebugStringA("\r\n");
		return 1;
	}
	
	return 1;
}

// CMFCPlayerApp

BEGIN_MESSAGE_MAP(CMFCPlayerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCPlayerApp ����

CMFCPlayerApp::CMFCPlayerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMFCPlayerApp ����

CMFCPlayerApp theApp;


// CMFCPlayerApp ��ʼ��

BOOL CMFCPlayerApp::InitInstance()
{
	CWinApp::InitInstance();

	FFL_LogHook(printLog);

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMFCPlayerControlDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

