#pragma once
#include "afxwin.h"
#include "Pe.h"


// CNtHeaderDlg �Ի���

class CNtHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNtHeaderDlg)

public:
	CNtHeaderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNtHeaderDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Dlg_NT_HEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void ShowNtHeaderInfo();
public:
	CPe*	m_pPE;

	CString m_strOEP;
	CString m_strImageBase;
	CString m_strSizeOfImage;
	CString m_strBaseOfCode;
	CString m_strBaseOfData;
	CString m_strSectionAlignment;
	CString m_strFileAlignment;
	CString m_strMagic;
	CString m_strSubSystem;
	CString m_strNumOfSection;
	CString m_strTimeDateStamp;
	CString m_strSizeOfHeader;
	CString m_strDllCharacteristic;
	CString m_strCheckSum;
	CString m_strSizeOfOptionalHeader;
	CString m_strNumOfDataDir;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSectionHeader();
	afx_msg void OnBnClickedBtnDatadir();
};
