// HOGPDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "HOGP.h"
#include "HOGPDlg.h"
#include "Usbhidioc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUsbhidioc USB;
/////////////////////////////////////////////////////////////////////////////
// CHOGPDlg �_�C�A���O

CHOGPDlg::CHOGPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHOGPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHOGPDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHOGPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHOGPDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHOGPDlg, CDialog)
	//{{AFX_MSG_MAP(CHOGPDlg)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHOGPDlg ���b�Z�[�W �n���h��

BOOL CHOGPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}


// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B


HCURSOR CHOGPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CHOGPDlg::OnOK() 
{
}

void CHOGPDlg::OnSend() 
{
	bool result;
	int i;
	HKEY hKey;
	DWORD cbData;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Enum\\HID\\CSRHIDDevice04D8003F", 0, KEY_QUERY_VALUE, &hKey)){
		USB.VendorID = 0;
		USB.ProductID =0;

	}	
	else{
		USB.VendorID = 0x04d8;
		USB.ProductID =0x003f;
	}

	for(i=0;i<19;i++) buf[i]=0;
	GetDlgItemText( IDC_OUT, buf,19 );
    USB.WriteReport ((unsigned char *)buf, 19);
 
	for(i=0;i<19;i++) buf[i]=0;
    result=USB.ReadReport ((unsigned char *)buf);

	SetDlgItemText( IDC_IN, buf);
}
