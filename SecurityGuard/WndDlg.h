#pragma once
#include "WndList.h"


// CWndDlg �Ի���

class CWndDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWndDlg)

public:
	CWndDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWndDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CWndList m_WndList;
	virtual BOOL OnInitDialog();
};
