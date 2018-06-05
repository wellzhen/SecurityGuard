// WndDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "WndDlg.h"
#include "afxdialogex.h"


// CWndDlg �Ի���

IMPLEMENT_DYNAMIC(CWndDlg, CDialogEx)

CWndDlg::CWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WND, pParent)
{

}

CWndDlg::~CWndDlg()
{
}

void CWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WND, m_WndList);
}


BEGIN_MESSAGE_MAP(CWndDlg, CDialogEx)
END_MESSAGE_MAP()


// CWndDlg ��Ϣ�������


BOOL CWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�������б�ؼ�
	m_WndList.InsertColumnCustom(2, 0.7, L"��������", 0.3, L"״̬");
	//�õ����洰��
	HWND  hWnd = ::GetDesktopWindow();
	//�õ���Ļ�ϵĵ�һ���Ӵ���
	hWnd = ::GetWindow(hWnd, GW_CHILD);
	WCHAR szName[266] = { 0 };
	//ѭ����ȡ�Ӵ���
	while (hWnd != NULL) {
		memset(szName, 0, sizeof(szName));
		::GetWindowText(hWnd, szName, 266);
		if (wcslen(szName) != 0) {
			if (::IsWindowVisible(hWnd)) {
				m_WndList.InsertItemCustom(2, szName, L"");
			}
			else {
				m_WndList.InsertItemCustom(2, szName, L"���ɼ�");
			}

		}

		hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
