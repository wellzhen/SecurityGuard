#pragma once


// CPeDlg �Ի���

class CPeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPeDlg)

public:
	CPeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOADPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
