#pragma once
#include "afxcmn.h"
#include "ModuleList.h"


// CModuleDlg �Ի���

class CModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModuleDlg)

public:
	CModuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModuleDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_ModuleList;
	INT m_pid;
//	virtual void PreInitDialog();
	virtual BOOL OnInitDialog();
	CModuleList m_ModuleList;
};
