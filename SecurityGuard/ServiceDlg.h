#pragma once
#include "ServiceList.h"


// CServiceDlg �Ի���

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Service };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateServiceInfo();

public:
	CServiceList m_ServiceList;
//	afx_msg void OnRclickListService(NMHDR *pNMHDR, LRESULT *pResult);
};
