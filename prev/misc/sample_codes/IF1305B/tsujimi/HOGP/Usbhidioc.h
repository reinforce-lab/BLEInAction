// Usbhidioc.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CUsbhidioc �E�B���h�E

class CUsbhidioc : public CWnd
{
// �R���X�g���N�V����
public:
	CUsbhidioc();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

	unsigned int VendorID;
	unsigned int ProductID;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CUsbhidioc)
	//}}AFX_VIRTUAL

public:
    bool ReadReport (unsigned char *);
    bool WriteReport(unsigned char *, unsigned int);
    void CloseReport ();

protected:
    bool FindTheHID();
    void GetDeviceCapabilities();
    void PrepareForOverlappedTransfer();

// �C���v�������e�[�V����
public:
	virtual ~CUsbhidioc();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CUsbhidioc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
