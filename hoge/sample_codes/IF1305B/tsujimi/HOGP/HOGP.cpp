// HOGP.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
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
// CHOGPApp �N���X�̍\�z

CHOGPApp::CHOGPApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CHOGPApp �I�u�W�F�N�g

CHOGPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHOGPApp �N���X�̏�����

BOOL CHOGPApp::InitInstance()
{
	// �W���I�ȏ���������

	CHOGPDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();


	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
