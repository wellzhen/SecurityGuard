// StartupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "StartupDlg.h"
#include "afxdialogex.h"


// CStartupDlg �Ի���

IMPLEMENT_DYNAMIC(CStartupDlg, CDialogEx)

CStartupDlg::CStartupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_StartUp, pParent)
{

}

CStartupDlg::~CStartupDlg()
{
}

void CStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_StartUp, m_StartupList);
}


BEGIN_MESSAGE_MAP(CStartupDlg, CDialogEx)
END_MESSAGE_MAP()


// CStartupDlg ��Ϣ�������


BOOL CStartupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����
	m_StartupList.InsertColumnCustom(3, 0.2, L"Index", 0.3, L"keyName", 0.5, L"keyValue");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CStartupDlg::UpdateStartupInfo()
{
	m_StartupList.DeleteAllItems();
	
}
