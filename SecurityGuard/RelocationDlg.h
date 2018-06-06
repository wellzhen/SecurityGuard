#pragma once
#include "ListCtrlCore.h"
#include "pe.h"

// CRelocationDlg �Ի���

class CRelocationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocationDlg)

public:
	CRelocationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelocationDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Relocation };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowRelocationInfo();

public:
	CPe* m_pPE;
	CListCtrlCore m_ListSectionBelong;
	CListCtrlCore m_ListEntryOfBlock;
//	afx_msg void OnLvnItemchangedListSectionblong(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnClickListEntriesofblock(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListSectionblongList(NMHDR *pNMHDR, LRESULT *pResult);
};
