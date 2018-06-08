// ServiceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"
#include <winsvc.h>

// CServiceDlg �Ի���

IMPLEMENT_DYNAMIC(CServiceDlg, CDialogEx)

CServiceDlg::CServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Service, pParent)
{

}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Service, m_ServiceList);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialogEx)
	//ON_NOTIFY(NM_RCLICK, IDC_LIST_Service, &CServiceDlg::OnRclickListService)
END_MESSAGE_MAP()


// CServiceDlg ��Ϣ�������


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����
	m_ServiceList.InsertColumnCustom(6, 0.12, L"ServiceName", 0.2, L"DisplayName", 0.1, L"Status", 0.1, L"startType", 0.08, L"SerType", 0.1, L"Path");
	UpdateServiceInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CServiceDlg::UpdateServiceInfo()
{
	
	// 1. ��Զ�̼����������ƹ�����
	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (NULL == hScm)
	{
		MessageBox(L"�޷��򿪷���");
		return ;
	}

	DWORD dwSize = 0;
	DWORD dwServiceNum = 0;
	EnumServicesStatusEx(hScm, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	LPENUM_SERVICE_STATUS_PROCESS pcBuff = NULL;
	pcBuff = (LPENUM_SERVICE_STATUS_PROCESS)malloc(dwSize);
	memset(pcBuff, 0, dwSize);

	EnumServicesStatusEx(hScm, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		(LPBYTE)pcBuff, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//��Ӧ��ϵ
	WCHAR* ServiceStateMap[5] = { L"0", L"��ֹͣ", L"2", L"3", L"��������" };
	

	//������Ϣ  
	for (DWORD i = 0; i<dwServiceNum; i++)
	{
		DWORD dwServiceState = pcBuff[i].ServiceStatusProcess.dwCurrentState;
		DWORD dwServiceType = pcBuff[i].ServiceStatusProcess.dwServiceType;
		CString strServiceType, strServiceState;
		strServiceType.Format(L"%d", dwServiceType);
		if (dwServiceState < 5) {
			strServiceState = ServiceStateMap[dwServiceState];
		}
		else {
			strServiceState.Format(L"%d", dwServiceType);
		}
		//��ȡ��������
		SC_HANDLE hService = OpenService(hScm, pcBuff[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//��ȡ�����С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//�ڶ��ε���, ��ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//��ȡ��������
		DWORD dwStartType =  pServiceConfig->dwStartType;
		CString strStartType;
		switch (dwStartType) {
		case 2:
			strStartType = L"�Զ�";
			break;
		case 3:
			strStartType = L"�ֶ�";
			break;
		case 4:
			strStartType = L"����";
			break;
		default:
			strStartType.Format(L"%d", dwStartType);
			break;
		}
		//��ȡ·����Ϣ
		WCHAR* dwPath = pServiceConfig->lpBinaryPathName;
		m_ServiceList.InsertItemCustom(6, pcBuff[i].lpServiceName, pcBuff[i].lpDisplayName, strServiceState,
			strStartType, strServiceType, dwPath);
	}

	getchar();

	CloseServiceHandle(hScm);
	return ;
}


//void CServiceDlg::OnRclickListService(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//	
//}
