// TabMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "TabMain.h"


// CTabMain

IMPLEMENT_DYNAMIC(CTabMain, CTabCtrl)

CTabMain::CTabMain()
{

}

CTabMain::~CTabMain()
{
}


BEGIN_MESSAGE_MAP(CTabMain, CTabCtrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CTabMain::OnTcnSelchangeTabMain)
//	ON_NOTIFY_REFLECT(TCN_SELCHANGING, &CTabMain::OnTcnSelchanging)
ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabMain::OnTcnSelchange)
END_MESSAGE_MAP()




void CTabMain::InsertItemCustom(DWORD dwCount, ...)
{
	va_list list;
	va_start(list, dwCount);

	for (DWORD i = 0; i < dwCount; i++) {
		WCHAR* title = va_arg(list, WCHAR*);
		InsertItem(i, title);
	}
	va_end(list);
}

void CTabMain::AddDialogCustom(DWORD dwCount, ...)
{
	va_list list;
	va_start(list, dwCount);
	//��ȡ�ߴ�
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(2, 23, 2, 2);
	for (DWORD i = 0; i < dwCount; i++) {
		UINT IDD = va_arg(list, UINT); //��ԴID
		m_Dialog[i] = va_arg(list, CDialogEx *);
		m_Dialog[i]->Create(IDD, this);
		m_Dialog[i]->MoveWindow(&rc);
	}
	va_end(list);

	m_dwDialogCount = dwCount;
	return;
}

void CTabMain::ShowDialogCustom(INT nIndex)
{
	for (DWORD i = 0; i < m_dwDialogCount; i++) {
		if (i == nIndex) {
			m_Dialog[i]->ShowWindow(SW_SHOW);
		}
		else {
			m_Dialog[i]->ShowWindow(SW_HIDE);
		}
	}

	return;
}

// CTabMain ��Ϣ�������

void CTabMain::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	
}


//void CTabMain::OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CTabMain::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//���Tab��ǩʱ�л�ҳ��
	DWORD nNum = GetCurSel();
	if (nNum > m_dwDialogCount - 1) {
		MessageBox(L"��ǰ��ǩ����δ����");
		return;
	}

	for (DWORD i = 0; i < m_dwDialogCount; i++) {
		if (nNum == i) {
			m_Dialog[i]->ShowWindow(SW_SHOW);
		}
		else {
			m_Dialog[i]->ShowWindow(SW_HIDE);
		}
	}
}
