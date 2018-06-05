// PeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PeDlg.h"
#include "afxdialogex.h"
#include "NtHeaderDlg.h"


// CPeDlg �Ի���

IMPLEMENT_DYNAMIC(CPeDlg, CDialogEx)

CPeDlg::CPeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOADPE, pParent)
	, m_strChoosedFile(_T(""))
{

}

CPeDlg::~CPeDlg()
{
}

void CPeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_PE_FILE, m_EditBrowseCtrl);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_PE_FILE, m_strChoosedFile);
}


BEGIN_MESSAGE_MAP(CPeDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_PE_FILE, &CPeDlg::OnEnChangeMfceditbrowsePeFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PE, &CPeDlg::OnBnClickedButtonLoadPe)
END_MESSAGE_MAP()


// CPeDlg ��Ϣ�������


void CPeDlg::OnEnChangeMfceditbrowsePeFile()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if (m_strChoosedFile.IsEmpty()) {
		MessageBox(L"��ѡ��PE�ļ�");
		return;
	}

}


void CPeDlg::OnBnClickedButtonLoadPe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if (m_strChoosedFile.IsEmpty()) {
		MessageBox(L"�ļ�����Ϊ��");
		return;
	}
	//��ʼ����;��֤ǩ��
	WCHAR  pathBuffer[256] = { 0 };
	CStringW  strWide = CT2CW(m_strChoosedFile);
	wcscpy_s(pathBuffer,wcslen(strWide)*2 + 1,  strWide);
	m_pPE = new CPe();
	if (!m_pPE->LoadFile(pathBuffer)) {
		MessageBox(L"�޷����ش��ļ�");
		return;
	}
	if (!m_pPE->IsPeFile()) {
		MessageBox(L"���ļ�����PE��ʽ");
		return;
	}
	//��ʾntͷ��ϢDlg
	CNtHeaderDlg* pNtHeaderDlg = new CNtHeaderDlg;
	pNtHeaderDlg->m_pPE = m_pPE;
	pNtHeaderDlg->DoModal();
	return;
}
