#pragma once
#include "ThreadList.h"

// CThreadDlg �Ի���

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThreadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CThreadList m_ThreadList;
	INT m_pid;
	virtual BOOL OnInitDialog();
};
