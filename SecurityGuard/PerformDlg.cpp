// PerformDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PerformDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

// CPerformDlg �Ի���

IMPLEMENT_DYNAMIC(CPerformDlg, CDialogEx)

CPerformDlg::CPerformDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PERFORMANCE, pParent)
{

}

CPerformDlg::~CPerformDlg()
{
}

void CPerformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_PROGRESS1, m_Progress_MemoryLoad);
	DDX_Control(pDX, IDC_STATIC_TOTAL_PHYS, m_Static_TotalPhys);
	DDX_Control(pDX, IDC_STATIC_AVAIL_PHYS, m_static_AvailPhys);
	DDX_Control(pDX, IDC_STATIC_TOTAL_PAGE_FILE, m_Static_TotalPageFile);
	DDX_Control(pDX, IDC_STATIC_AVAIL_PAGE_FILE, m_Static_AvailPageFile);
	DDX_Control(pDX, IDC_STATIC_TOTAL_VIRTUAL, m_Static_TotalVirtual);
	DDX_Control(pDX, IDC_STATIC_AVAIL_VIRTUAL, m_Static_AvailVirtual);
	//  DDX_Control(pDX, IDC_PROGRESS_USAGE, m_Static_MemoryLoad);
	DDX_Control(pDX, IDC_PROGRESS_USAGE, m_Progress_CpuUsage);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress_MemoryLoad);
	DDX_Control(pDX, IDC_STATIC_MEMORY_LOAD, m_Static_MemoryLoad);
	DDX_Control(pDX, IDC_STATIC_CPU_USAGE, m_Static_CpuUsage);
}


BEGIN_MESSAGE_MAP(CPerformDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Clear_Mem, &CPerformDlg::OnBnClickedButtonClearMem)
	ON_BN_CLICKED(IDC_BUTTON_SystemInfo, &CPerformDlg::OnBnClickedButtonSysteminfo)
END_MESSAGE_MAP()

void CPerformDlg::UpdatePerformanceData()
{
	MEMORYSTATUS  memStatus;
	GlobalMemoryStatus(&memStatus);


	//��ʾ����
	DWORD m = 1024 * 1024;
	CString str;

	m_Progress_MemoryLoad.SetPos(memStatus.dwMemoryLoad);
	str.Format(L"%d / 100", memStatus.dwMemoryLoad);
	m_Static_MemoryLoad.SetWindowTextW(str);
	str.Format(L"%d M", memStatus.dwTotalPhys / m);

	m_Static_TotalPhys.SetWindowTextW(str);
	str.Format(L"%d M", memStatus.dwAvailPhys / m);
	m_static_AvailPhys.SetWindowTextW(str);
	str.Format(L"%d M", memStatus.dwTotalPageFile / m);
	m_Static_TotalPageFile.SetWindowTextW(str);
	str.Format(L"%d M", memStatus.dwAvailPageFile / m);
	m_Static_AvailPageFile.SetWindowTextW(str);
	str.Format(L"%d M", memStatus.dwTotalVirtual / m);
	m_Static_TotalVirtual.SetWindowTextW(str);
	str.Format(L"%d M", memStatus.dwAvailVirtual / m);
	m_Static_AvailVirtual.SetWindowTextW(str);


	//CPUռ����
	INT cpuUsage = GetCpuUsage();
	m_Progress_CpuUsage.SetPos(cpuUsage);
	str.Format(L"%d / 100", cpuUsage);
	m_Static_CpuUsage.SetWindowTextW(str);
}


INT CPerformDlg::GetCpuUsage()
{
	//		    �����¼�  �ں��¼�   �û��¼�
	_FILETIME  idleTime, kernelTime, userTime;
	//��ȡʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	//�ȴ�1000����
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	//��ȡ�µ�ʱ��
	_FILETIME  newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//������ʱ��ת��
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//�����ʹ����
	return  int(100.0 - (dNewIdleTime - dOldIdleTime) /
		(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
}

double CPerformDlg::FILETIME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);

}

// CPerformDlg ��Ϣ�������


BOOL CPerformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdatePerformanceData();
	SetTimer(100, 10000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}





void CPerformDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 100) {
		UpdatePerformanceData();
	}

	CDialogEx::OnTimer(nIDEvent);
}

//�Ż��ڴ�
void CPerformDlg::OnBnClickedButtonClearMem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//1. ��ȡ��ǰ���ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	//2. �����ڴ�
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	//3. ��ȡ�������ڴ�״̬
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	
	//self:update
	UpdatePerformanceData();

}

DWORD PASCAL CPerformDlg::GetVersion(void)
{  
	//from:  https://msdn.microsoft.com/en-us/library/aa370663.aspx
	DWORD dwLevel = 102;
	LPWKSTA_INFO_102 pBuf = NULL;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;
	//
	// Call the NetWkstaGetInfo function, specifying level 102.
	//
	nStatus = NetWkstaGetInfo(pszServerName,
		dwLevel,
		(LPBYTE *)&pBuf);
	//
	// If the call is successful,
	//  print the workstation data.
	//
	if (nStatus == NERR_Success)
	{
		CString strVersion;
		CString strPlatform = L"Not  NT platform";
		if (pBuf->wki102_platform_id == PLATFORM_ID_NT) {
			strPlatform = L"Windows NT";
		}
		strVersion.Format(L" Platform: %s \n Name:     %s \n Version:  %d.%d   \n Domain:   %s\n Lan Root: %s \n Logged On Users: %d ", 
			strPlatform, pBuf->wki102_computername, pBuf->wki102_ver_major,
			pBuf->wki102_ver_minor, pBuf->wki102_langroup, pBuf->wki102_lanroot, pBuf->wki102_logged_on_users);
		MessageBox(strVersion);
	}
	//
	// Otherwise, indicate the system error.
	//
	else
		MessageBox(L"A system error has occurred");
	//
	// Free the allocated memory.
	//
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return 0;
}


void CPerformDlg::OnBnClickedButtonSysteminfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetVersion();
}
