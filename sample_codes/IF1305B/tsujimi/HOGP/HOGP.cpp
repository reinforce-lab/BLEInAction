// HOGP.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "HOGP.h"
#include "HOGPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHOGPApp

BEGIN_MESSAGE_MAP(CHOGPApp, CWinApp)
	//{{AFX_MSG_MAP(CHOGPApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHOGPApp クラスの構築

CHOGPApp::CHOGPApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CHOGPApp オブジェクト

CHOGPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHOGPApp クラスの初期化

BOOL CHOGPApp::InitInstance()
{
	// 標準的な初期化処理

	CHOGPDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();


	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
