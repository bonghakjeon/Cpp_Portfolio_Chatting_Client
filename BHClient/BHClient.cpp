
// BHClient.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "BHClient.h"
#include "BHClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBHClientApp

BEGIN_MESSAGE_MAP(CBHClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBHClientApp 생성

CBHClientApp::CBHClientApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CBHClientApp 개체입니다.

CBHClientApp theApp;

// CBHClientApp 초기화

BOOL CBHClientApp::InitInstance()
{

	CWinApp::InitInstance();

	CBHClientDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

