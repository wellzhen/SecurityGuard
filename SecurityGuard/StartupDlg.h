#pragma once
#include "StartupList.h"


// CStartupDlg �Ի���

class CStartupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartupDlg)

public:
	CStartupDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartupDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_StartUp };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateStartupInfo();

public:

	CStartupList m_StartupList;
};
