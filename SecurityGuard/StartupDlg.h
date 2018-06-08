#pragma once
#include "StartupList.h"


// CStartupDlg 对话框

class CStartupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartupDlg)

public:
	CStartupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartupDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_StartUp };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateStartupInfo();

public:

	CStartupList m_StartupList;
};
