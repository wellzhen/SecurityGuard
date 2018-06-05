#pragma once


// CPeDlg 对话框

class CPeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPeDlg)

public:
	CPeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOADPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
