#pragma once
#include "SectionHeaderList.h"
#include "Pe.h"

// CSectionHeaderDlg �Ի���

class CSectionHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionHeaderDlg)

public:
	CSectionHeaderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSectionHeaderDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SectionHeader };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowSectionHeaderInfo();
public:
	CPe* m_pPE;
	CSectionHeaderList m_SectionHeaderList;
	virtual BOOL OnInitDialog();
};
