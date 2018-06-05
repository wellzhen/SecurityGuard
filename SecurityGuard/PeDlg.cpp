// PeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PeDlg.h"
#include "afxdialogex.h"


// CPeDlg 对话框

IMPLEMENT_DYNAMIC(CPeDlg, CDialogEx)

CPeDlg::CPeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOADPE, pParent)
{

}

CPeDlg::~CPeDlg()
{
}

void CPeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPeDlg, CDialogEx)
END_MESSAGE_MAP()


// CPeDlg 消息处理程序
