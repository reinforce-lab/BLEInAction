// HOGPDlg.cpp : インプリメンテーション ファイル
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
// CHOGPDlg ダイアログ

CHOGPDlg::CHOGPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHOGPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHOGPDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHOGPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHOGPDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHOGPDlg, CDialog)
	//{{AFX_MSG_MAP(CHOGPDlg)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHOGPDlg メッセージ ハンドラ

BOOL CHOGPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}


// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。


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
