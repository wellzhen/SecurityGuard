#pragma once
#include "afxcmn.h"
#include "HeapList.h"


// CHeapDlg �Ի���

class CHeapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHeapDlg)

public:
	CHeapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHeapDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_HeapList;
	INT m_pid;
	CHeapList m_HeapList;
	virtual BOOL OnInitDialog();
};
