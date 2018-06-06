#pragma once
#include "Pe.h"


// CDataDirDlb �Ի���

class CDataDirDlb : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDirDlb)

public:
	CDataDirDlb(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataDirDlb();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DATA_DIR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowDataDirInfo();
	virtual BOOL OnInitDialog();
public:
	CPe*  m_pPE;
	
	CString m_strExportRva;
	CString m_strExportSize;
	CString m_strImportRVA;
	CString m_strImportSize;
	CString m_strResourceRVA;
	CString m_strResourceSize;
	CString m_strRelocRVA;
	CString m_strRelocSize;
	CString m_strDelayRVA;
	CString m_strDelaySize;
	CString m_strTLSRVA;
	CString m_strTLSSize;
	afx_msg void OnBnClickedBtnExportDir();
	afx_msg void OnBnClickedBtnImportDesc();
	afx_msg void OnBnClickedBtnResourceDir();
	afx_msg void OnBnClickedBtnRelocation();
	afx_msg void OnBnClickedBtnTls();
};
