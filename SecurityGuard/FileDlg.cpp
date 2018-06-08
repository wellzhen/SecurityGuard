// FileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "FileDlg.h"
#include "afxdialogex.h"
#include "Md5.h"

#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1,\
lpChar, _countof(lpChar), NULL, FALSE);

#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1,\
lpW_Char, _countof(lpW_Char));

// CFileDlg �Ի���

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILE, pParent)
	, m_EditFilePostfix(_T(""))
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_FilesTree);
	DDX_Text(pDX, IDC_EDIT_FILE_POSTFIX, m_EditFilePostfix);
	DDX_Control(pDX, IDC_FILE_BROWSER, m_FileBrowser);
}

void CFileDlg::TraverseFiles(CString fullPath, HTREEITEM  hRootNode)
{

	CString strFileName;
	CString strFilePath;

	strFilePath = fullPath;
	strFilePath.Append(L"\\*");

	WIN32_FIND_DATA findFileData;
	//��ȡ��һ���ļ�/Ŀ¼, ����ò��Ҿ��
	HANDLE hFileList = FindFirstFile(strFilePath, &findFileData);
	if (INVALID_HANDLE_VALUE == hFileList) {
		return;
	}
	DWORD dwStyle = m_FilesTree.GetStyle();
	dwStyle = dwStyle | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SINGLEEXPAND;
	m_FilesTree.ModifyStyle(0,dwStyle);
	do {
		strFileName = findFileData.cFileName;
		if (strFileName[0] == '.' || strFileName[0] == '..') {
			continue;
		}
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {// Ŀ¼
																	   //ǰ����ʾ
			CString info;
			info.Format(L"%s", findFileData.cFileName);
			HTREEITEM hChildNode = m_FilesTree.InsertItem(info, hRootNode);
			//��ȡ����·��
			CString strFullPath;
			strFilePath.TrimRight(L"\\*");
			strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
			TraverseFiles(strFullPath, hChildNode);
		}
		else { //�ļ�
			CString info;
			SYSTEMTIME sysCreationTime, sysAccessTime, sysWriteTime;
			FileTimeToSystemTime(&findFileData.ftCreationTime, &sysCreationTime);
			FileTimeToSystemTime(&findFileData.ftLastAccessTime, &sysAccessTime);
			FileTimeToSystemTime(&findFileData.ftLastWriteTime, &sysWriteTime);
			//����ʱ��
			sysCreationTime.wHour = (sysCreationTime.wHour + 8) % 24;
			sysAccessTime.wHour = (sysAccessTime.wHour + 8) % 24;
			sysWriteTime.wHour = (sysWriteTime.wHour + 8) % 24;
			//����md5;
			CString strFullPath;
			strFullPath.Format(L"%s\\%s", fullPath, findFileData.cFileName);
			char chFullPath[256] = { 0 };
			WCHAR_TO_CHAR(strFullPath.GetBuffer(), chFullPath);
			char* md5 = md5FileValue(chFullPath);
			WCHAR wMd5[256] = { 0 };
			CHAR_TO_WCHAR(md5, wMd5);
			
			info.Format(L"%s  %d byte <%s> [c]%d/%d/%d %d:%02d:%d [a]%d/%d/%d %d:%02d:%d [m]%d/%d/%d %d:%02d:%d ", findFileData.cFileName, findFileData.nFileSizeLow, wMd5,
				sysCreationTime.wYear, sysCreationTime.wMonth, sysCreationTime.wDay, sysCreationTime.wHour, sysCreationTime.wMinute, sysCreationTime.wSecond,
				sysAccessTime.wYear, sysAccessTime.wMonth, sysAccessTime.wDay, sysAccessTime.wHour, sysAccessTime.wMinute, sysAccessTime.wSecond,
				sysWriteTime.wYear, sysWriteTime.wMonth, sysWriteTime.wDay, sysWriteTime.wHour, sysWriteTime.wMinute, sysWriteTime.wSecond
			);
			m_FilesTree.InsertItem(info, hRootNode);
			continue;
		}


	} while (FindNextFile(hFileList, &findFileData));
	//CloseHandle(hFileList);

	return;
}

void CFileDlg::TraverseDeleteFiles(CString fullPath, CString strDeletePostFix)
{

	CString strFileName;
	CString strFilePath;

	strFilePath = fullPath;
	strFilePath.Append(L"\\*");

	WIN32_FIND_DATA findFileData;
	//��ȡ��һ���ļ�/Ŀ¼, ����ò��Ҿ��
	HANDLE hFileList = FindFirstFile(strFilePath, &findFileData);
	if (INVALID_HANDLE_VALUE == hFileList) {
		return;
	}

	do {
		strFileName = findFileData.cFileName;
		if (strFileName[0] == '.' || strFileName[0] == '..') {
			continue;
		}
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {// Ŀ¼

																	   //��ȡ����·��
			CString strFullPath;
			strFilePath.TrimRight(L"\\*");
			strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
			TraverseDeleteFiles(strFullPath, strDeletePostFix);
		}
		else { //�ļ�
			   //�ж��Ƿ����ɾ��
			   //��ȡ�ļ�.��λ��
			int DotPos = strFileName.ReverseFind('.');
			if (DotPos == -1) {
				continue;
			}
			int length = strFileName.GetLength();
			int postFixLenght = length - DotPos;
			if (postFixLenght <= 1) {
				MessageBox(L"�ļ���׺���쳣");
				continue;
			}
			CString strPostfix = strFileName.Right(postFixLenght);
			strPostfix.Append(L"\\");
			if (strDeletePostFix.Find(strPostfix) != -1) {
				//ɾ���ļ�
				CString deleteInfo;
				deleteInfo.Format(L"ɾ���ļ�:%s", strFileName);
				CString strFullPath;
				strFilePath.TrimRight(L"\\*");
				strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
				//DeleteFile(strFullPath);
				MessageBox(deleteInfo);
			}
			continue;
		}


	} while (FindNextFile(hFileList, &findFileData));
	//CloseHandle(hFileList);

	return;
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_EN_CHANGE(IDC_FILE_BROWSER, &CFileDlg::OnEnChangeFileBrowser)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_FILE, &CFileDlg::OnBnClickedButtonClearFile)
END_MESSAGE_MAP()


// CFileDlg ��Ϣ�������


void CFileDlg::OnEnChangeFileBrowser()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString selectedPath;
	GetDlgItemText(IDC_FILE_BROWSER, selectedPath);
	//MessageBox(selectedPath);
	if (selectedPath.IsEmpty()) {
		MessageBox(L"��ѡ��Ŀ¼");
		return;
	}
	//ɾ���������ڵ�
	m_FilesTree.DeleteAllItems();

	HTREEITEM hRoot = TVI_ROOT;
	TraverseFiles(selectedPath, hRoot);
}


void CFileDlg::OnBnClickedButtonClearFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString selectedPath;
	GetDlgItemText(IDC_FILE_BROWSER, selectedPath);
	if (m_EditFilePostfix.IsEmpty()) {
		MessageBox(L"����дҪɾ�����ļ���׺");
		return;
	}
	else if (selectedPath.IsEmpty()) {
		MessageBox(L"��ѡ��Ŀ¼");
		return;
	}


	//MessageBox(m_EditFilePostfix);
	TraverseDeleteFiles(selectedPath, m_EditFilePostfix);
}
