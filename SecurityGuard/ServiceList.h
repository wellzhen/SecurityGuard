#pragma once


// CServiceList

class CServiceList : public CListCtrl
{
	DECLARE_DYNAMIC(CServiceList)

public:
	CServiceList();
	virtual ~CServiceList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartService(); // ���������/ֹͣ�л�
public:
	CString m_strClickedServiceName;
};


