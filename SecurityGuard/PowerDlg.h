#pragma once


// CPowerDlg �Ի���

class CPowerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPowerDlg)

public:
	CPowerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPowerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POWER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPowerOff();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRestart();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonSleep();
	afx_msg void OnBnClickedButtonRest();
	afx_msg void OnBnClickedButtonHotKey();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	BOOL m_IsWindowHide = TRUE;  // ȫ���ȼ�ʹ��
};
