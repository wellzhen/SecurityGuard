// PowerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PowerDlg.h"
#include "afxdialogex.h"
#include <PowrProf.h>
#pragma comment(lib,"PowrProf.lib")


// CPowerDlg �Ի���

IMPLEMENT_DYNAMIC(CPowerDlg, CDialogEx)

CPowerDlg::CPowerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_POWER, pParent)
{

}

CPowerDlg::~CPowerDlg()
{
}

void CPowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPowerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_POWER_OFF, &CPowerDlg::OnBnClickedButtonPowerOff)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CPowerDlg::OnBnClickedButtonRestart)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CPowerDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CPowerDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_SLEEP, &CPowerDlg::OnBnClickedButtonSleep)
	ON_BN_CLICKED(IDC_BUTTON_REST, &CPowerDlg::OnBnClickedButtonRest)
	ON_BN_CLICKED(IDC_BUTTON_HOT_KEY, &CPowerDlg::OnBnClickedButtonHotKey)
END_MESSAGE_MAP()


// CPowerDlg ��Ϣ�������

BOOL CPowerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����������Ȩ
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//���ú�������Ȩ��
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CPowerDlg::OnBnClickedButtonPowerOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindowsEx(EWX_POWEROFF, EWX_FORCE);
}





void CPowerDlg::OnBnClickedButtonRestart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindowsEx(EWX_REBOOT, EWX_FORCE);
}




void CPowerDlg::OnBnClickedButtonLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindowsEx(EWX_LOGOFF, EWX_FORCE);
}




void CPowerDlg::OnBnClickedButtonLock()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LockWorkStation();
}


void CPowerDlg::OnBnClickedButtonSleep()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetSuspendState(TRUE, FALSE, FALSE);
}


void CPowerDlg::OnBnClickedButtonRest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetSuspendState(TRUE, FALSE, FALSE);
}




BOOL CPowerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_HOTKEY && pMsg->wParam == 0xa001) {
		MessageBox(L"ee");
		if (m_IsWindowHide == TRUE) {
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		else {
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPowerDlg::OnBnClickedButtonHotKey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL res = ::RegisterHotKey(this->GetSafeHwnd(), 0xa001, MOD_CONTROL | MOD_SHIFT, 'k');
	if (res) {
		MessageBox(L"ע��ɹ�");
	}
	else {
		MessageBox(L"ע��ʧ��");
	}
}
