#pragma once
#include "Pe.h"


// CTlsDlg �Ի���

class CTlsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTlsDlg)

public:
	CTlsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTlsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CPe* m_pPE;
	CString m_StartAddressOfRawData;
	CString m_EndAddressOfRawData;
	CString m_AddressOfIndex;
	CString m_AddressOfCallBacks;
	CString m_SizeOfZeroFill;
	CString m_Characteristics;
};
