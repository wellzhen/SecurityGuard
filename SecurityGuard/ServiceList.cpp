// ServiceList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ServiceList.h"

#include <winsvc.h>

// CServiceList

IMPLEMENT_DYNAMIC(CServiceList, CListCtrl)

CServiceList::CServiceList()
{

}

CServiceList::~CServiceList()
{
}


BEGIN_MESSAGE_MAP(CServiceList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CServiceList::OnNMRClick)
	ON_COMMAND(ID_StartService, &CServiceList::OnStartService)
END_MESSAGE_MAP()

void CServiceList::InsertColumnCustom(DWORD dwCount, ...)
{
	//������չ��ʽ
	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//��ȡ�ߴ�
	CRect rc;
	GetClientRect(&rc);
	INT nTotalWidth = (INT)rc.Width();
	INT nUsedWidth = 0;
	INT nWidth; //ÿ��column�Ŀ��
	va_list list;
	va_start(list, dwCount);
	for (DWORD i = 0; i < dwCount; i++) {
		DOUBLE  fWidthRate = va_arg(list, DOUBLE);
		if (i != dwCount - 1) {
			nWidth = (INT)(nTotalWidth * fWidthRate);
		}
		else {
			nWidth = nTotalWidth - nUsedWidth; //����п�ʹ��ʣ������п��
		}

		if (nWidth < 5) {
			MessageBox(L"Warning: �߳��б�Ŀ��̫С��\n");
		}

		WCHAR* wStr = va_arg(list, WCHAR*);

		InsertColumn(i, wStr, 0, nWidth);
		nUsedWidth += nWidth;
	}
	va_end(list);

	return;
}


void CServiceList::InsertItemCustom(DWORD dwCount, ...)
{
	va_list list;
	va_start(list, dwCount);
	//����λ�ò���һ��
	InsertItem(0, L"");
	for (DWORD i = 0; i < dwCount; i++) {
		WCHAR* wStr = va_arg(list, WCHAR*);
		SetItemText(0, i, wStr);
	}

	va_end(list);
	return;
}

// CServiceList ��Ϣ�������




void CServiceList::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����̲߳���  
		VERIFY(menu.LoadMenu(IDR_MENU3));           //������������1�ж����MENU���ļ�����  
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//��������д�����Ҫ��Ϊ�˺���Ĳ���Ϊ׼����  
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��    
		POSITION m_pstion = GetFirstSelectedItemPosition();
		//�ú�����ȡ��posָ�����б����������Ȼ��pos����Ϊ��һ��λ�õ�POSITIONֵ  
		INT m_nIndex = GetNextSelectedItem(m_pstion);

		//��ȡPID
		int nColumn = 0; //pid���ڵ���
		CString strServiceName = GetItemText(m_nIndex, nColumn);
		m_strClickedServiceName = strServiceName;
	}
}



//ʵ��Ϊ�л�����״̬
void CServiceList::OnStartService()
{
	// TODO: �ڴ���������������
	// �򿪷���������  
	SC_HANDLE hSC = OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		MessageBox(L"open SCManager(GENERIC_EXECUTE) error");
		return;
	}
	// ��www����  
	SC_HANDLE hSvc = OpenService(hSC, m_strClickedServiceName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		MessageBox(L"OpenService erron");
		::CloseServiceHandle(hSC);
		return;
	}
	// ��÷����״̬  
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		MessageBox(L"Get Service state error");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	//�������ֹͣ״̬���������񣬷���ֹͣ����  
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// ֹͣ����  
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"stop service error");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// �ȴ�����ֹͣ  
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"stop success");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	else if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// ��������  
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			MessageBox(L"start service error");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// �ȴ���������  
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"start success");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	MessageBox(L"start error");
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return;
}


