// PeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PeDlg.h"
#include "afxdialogex.h"


// CPeDlg �Ի���

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


// CPeDlg ��Ϣ�������
