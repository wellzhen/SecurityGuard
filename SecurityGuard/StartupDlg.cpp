// StartupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "StartupDlg.h"
#include "afxdialogex.h"


// CStartupDlg 对话框

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


// CStartupDlg 消息处理程序


BOOL CStartupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化表格
	m_StartupList.InsertColumnCustom(3, 0.2, L"Index", 0.3, L"keyName", 0.5, L"keyValue");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CStartupDlg::UpdateStartupInfo()
{
	m_StartupList.DeleteAllItems();
	
}
