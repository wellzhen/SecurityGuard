// KillVirusDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "KillVirusDlg.h"
#include "afxdialogex.h"
#include "Md5.h"
#include <TlHelp32.h>


#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1,\
lpChar, _countof(lpChar), NULL, FALSE);

#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1,\
lpW_Char, _countof(lpW_Char));


// CKillVirusDlg �Ի���

IMPLEMENT_DYNAMIC(CKillVirusDlg, CDialogEx)

CKillVirusDlg::CKillVirusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_KillVirus, pParent)
	, m_BrowserPath(_T(""))
	, m_BrowserFile(_T(""))
{

}

CKillVirusDlg::~CKillVirusDlg()
{
}

void CKillVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_MFCEDITBROWSE_PATH, m_BrowserPath);
	//  DDX_Control(pDX, IDC_MFCEDITBROWSE_FILE, m_BrowserFile);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_PATH, m_BrowserPath);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_FILE, m_BrowserFile);
}


BEGIN_MESSAGE_MAP(CKillVirusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_KillVirus_online, &CKillVirusDlg::OnBnClickedBtnKillvirusonline)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_PATH, &CKillVirusDlg::OnEnChangeMfceditbrowsePath)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_FILE, &CKillVirusDlg::OnEnChangeMfceditbrowseFile)
	ON_BN_CLICKED(IDC_BUTTON_md5_save, &CKillVirusDlg::OnBnClickedButtonmd5save)
	ON_BN_CLICKED(IDC_BUTTON_kill_by_md5, &CKillVirusDlg::OnBnClickedButtonkillByMd5)
	ON_BN_CLICKED(IDC_BUTTON_kill_by_path, &CKillVirusDlg::OnBnClickedButtonkillByPath)
	ON_BN_CLICKED(IDC_BUTTON_kill_by_process, &CKillVirusDlg::OnBnClickedButtonkillByProcess)
END_MESSAGE_MAP()


// CKillVirusDlg ��Ϣ�������

//�Ʋ�ɱ
void CKillVirusDlg::OnBnClickedBtnKillvirusonline()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ӷ�����
	if (!m_client.ConnectServer(SERVERIP, SERVERPORT)) {
		MessageBox(L"���ӷ�����ʧ��");
		return;
	}
	if (m_BrowserFile.IsEmpty() && m_BrowserPath.IsEmpty()) {
		MessageBox(L"��ȷ�ϲ�ɱ·�����ļ�");
		return;
	}
	if (!m_BrowserFile.IsEmpty()) { // �ļ���ɱ
		CheckSingleFileOnLine();
	}
	else if (!m_BrowserPath.IsEmpty()) { //·�������ļ���ɱ
		CheckPathOnLine();
	}

}



void CKillVirusDlg::OnEnChangeMfceditbrowsePath()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_BrowserFile = L"";
	UpdateData(false);

}


void CKillVirusDlg::OnEnChangeMfceditbrowseFile()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_BrowserPath = L"";
	UpdateData(false);
}

void CKillVirusDlg::CheckSingleFileOnLine()
{
	if (!m_BrowserFile.IsEmpty()) { // �ļ���ɱ
		char chFullPath[256] = { 0 };
		WCHAR_TO_CHAR(m_BrowserFile.GetBuffer(), chFullPath);
		char* md5 = md5FileValue(chFullPath);
		if (strlen(md5) == 0) {
			MessageBox(L"�޷���ȡmd5");
			return;
		}

		m_client.Send(eSAMPLE, md5, 33);
		char* ret = m_client.Recv();
		if (!strcmp(ret, "true")) {
			MessageBox(L"Warning : ���ļ��ж�");
			
			return;
		}
		else {
			MessageBox(L"���ļ�����");
			return;
		}
		m_client.Close();
	}
}
void CKillVirusDlg::CheckPathOnLine()
{
	if (!m_BrowserPath.IsEmpty()) { //·����ɱ
		TraverseCheckFilesOnLine(m_BrowserPath);
		MessageBox(L"������");
	}
}

void CKillVirusDlg::TraverseCheckFilesOnLine(CString fullPath)
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
																	   //ǰ����ʾ
			CString info;
			info.Format(L"%s", findFileData.cFileName);
			//��ȡ����·��
			CString strFullPath;
			strFilePath.TrimRight(L"\\*");
			strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
			TraverseCheckFilesOnLine(strFullPath);
		}
		else { //�ļ�
			//����md5;
			CString strFullPath;
			strFullPath.Format(L"%s\\%s", fullPath, findFileData.cFileName);
			char chFullPath[256] = { 0 };
			WCHAR_TO_CHAR(strFullPath.GetBuffer(), chFullPath);
			char* md5 = md5FileValue(chFullPath);
			//��������֤
			CString strTip;
			if (strlen(md5) == 0) {
				strTip = L"�޷���ȡmd5:";
				strTip.Append(strFullPath);
				MessageBox(strTip);
				continue;
			}

			m_client.Send(eSAMPLE, md5, 33);
			char* ret = m_client.Recv();
			if (!strcmp(ret, "true")) {
				strTip = L"���ֲ����ļ�:";
				strTip.Append(strFullPath);
				MessageBox(strTip);
				continue;
				
			}
			else {
				//MessageBox(L"���ļ�����");
				continue;
			}
			continue;
		}


	} while (FindNextFile(hFileList, &findFileData));
	//CloseHandle(hFileList);
	return;
}



void CKillVirusDlg::OnBnClickedButtonmd5save()
{
	WATCHINFO WatchList[7] = { 0 };
	//md5;
	WatchList[0].type = 1;
	strcpy_s(WatchList[0].md5, "be61576bad377a38a85baecb93cf4b96"); // valid
	WatchList[1].type = 1;
	strcpy_s(WatchList[1].md5, "be61576bad377a38a85baecb93cf4b97");
	WatchList[2].type = 1;
	strcpy_s(WatchList[2].md5, "be61576bad377a38a85baecb93cf4b99");
	// path md5
	WatchList[3].type = 2;
	strcpy_s(WatchList[3].md5, "be61576bad377a38a85baecb93cf4b96");
	strcpy_s(WatchList[3].path, "C:\\Users\\moonq\\Desktop\\dll");
	WatchList[4].type = 2;
	strcpy_s(WatchList[4].md5, "be61576bad377a38a85baecb93cf4b96");
	strcpy_s(WatchList[4].path, "C:\\Users\\moonq\\Desktop");
	WatchList[5].type = 2;
	strcpy_s(WatchList[5].md5, "be61576bad377a38a85baecb93cf4b96");
	strcpy_s(WatchList[5].path, "C:\\Users\\moonq\\Desktop\\delete");
	//blacklist
	WatchList[6].type = 3;
	strcpy_s(WatchList[6].process, "snake_cpp.exe");


	
	FILE* pFile;
	errno_t ResRead = fopen_s(&pFile, "watchlist.db", "wb");
	if (ResRead != 0) {
		MessageBox(L"���ļ�ʧ��");
		return;
	}
	//���볤��
	int nCount = _countof(WatchList);
	fwrite(&nCount, sizeof(int),1, pFile);
	//����md5������Ϣ
	fwrite(WatchList, sizeof(WATCHINFO), nCount, pFile);
	fclose(pFile);
	
	//��ȡ����
	// FILE* pFile;
	errno_t Res2= fopen_s(&pFile, "watchlist.db", "rb");
	if (Res2 != 0) {
		MessageBox(L"�򿪱��ز�����ʧ��");
		return;
	}
	int nLength;
	fread_s(&nLength, sizeof(int), sizeof(int), 1, pFile);
	WATCHINFO *pInfo = new WATCHINFO[nLength];
	for (int i = 0; i < nLength; i++) {
		fread_s(&pInfo[i], sizeof(WATCHINFO), sizeof(WATCHINFO), 1, pFile);
	}
	fclose(pFile);

	

}



//
void CKillVirusDlg::OnBnClickedButtonkillByMd5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_BrowserFile.IsEmpty() && m_BrowserPath.IsEmpty()) {
		MessageBox(L"��ȷ�ϲ�ɱ·�����ļ�");
		return;
	}
	if (!m_BrowserFile.IsEmpty()) { // �ļ���ɱ
		CheckVirusOffLine(1);
	}
	else if (!m_BrowserPath.IsEmpty()) { //·�������ļ���ɱ
		CheckVirusOffLine(1);
	}
}


void CKillVirusDlg::OnBnClickedButtonkillByPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_BrowserFile.IsEmpty() && m_BrowserPath.IsEmpty()) {
		MessageBox(L"��ȷ�ϲ�ɱ·�����ļ�");
		return;
	}
	if (!m_BrowserFile.IsEmpty()) { // �ļ���ɱ
		CheckVirusOffLine(2);
	}
	else if (!m_BrowserPath.IsEmpty()) { //·�������ļ���ɱ
		CheckVirusOffLine(2);
	}
}


void CKillVirusDlg::OnBnClickedButtonkillByProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CheckVirusOffLine(3); //��Ҫ��ȡ�������ļ�
	
}

// 1, md5 2. path, 3, process
void CKillVirusDlg::CheckVirusOffLine(int type)
{
	//��ȡ�ļ�
	FILE* pFile;
	errno_t Res2 = fopen_s(&pFile, "watchlist.db", "rb");
	if (Res2 != 0) {
		MessageBox(L"�򿪱��ز�����ʧ��");
		return;
	}
	int nLength;
	fread_s(&nLength, sizeof(int), sizeof(int), 1, pFile);
	WATCHINFO *pInfo = new WATCHINFO[nLength];
	for (int i = 0; i < nLength; i++) {
		fread_s(&pInfo[i], sizeof(WATCHINFO), sizeof(WATCHINFO), 1, pFile);
	}
	fclose(pFile);

	m_pVirusBase = pInfo;
	m_nVirtusBaseCount = nLength;   //��������
	if (type == 3) {
		CheckProcessOffLine();
	}
	else if (!m_BrowserFile.IsEmpty()) { // �ļ���ɱ

		CheckSingleFileOffLine(type);

	}
	else if (!m_BrowserPath.IsEmpty()) { //·�������ļ���ɱ
		CheckPathOffLine(type);
	}

}

void CKillVirusDlg::CheckSingleFileOffLine(int type)
{
		char chFullPath[256] = { 0 };
		WCHAR_TO_CHAR(m_BrowserFile.GetBuffer(), chFullPath);
		char* md5 = md5FileValue(chFullPath);
		if (strlen(md5) == 0) {
			MessageBox(L"�޷���ȡmd5");
			return;
		}
		//����������
		for (int i = 0; i < m_nVirtusBaseCount; i++) {
			if (strcmp(m_pVirusBase[i].md5, md5) == 0) { //�ǲ���
				if (type == 2) { //ȫ·��Ҫ���·��
					//��ȡ�ļ�·��
					CString strPath;
					int n = m_BrowserFile.ReverseFind('\\');
					strPath = m_BrowserFile.Left(n);
					char chFilePath[256] = { 0 };
					WCHAR_TO_CHAR(strPath, chFilePath);
					if (m_pVirusBase[i].type == 2 && strcmp(m_pVirusBase[i].path, chFilePath)==0) {
						MessageBox(L"���ļ���·��md5����");
						return;
					}
				}
				else { //����md��ɱ
					MessageBox(L"���ļ���md5����");
					return;
				}
			}

		}

		MessageBox(L"���ļ�����");
		
}



void CKillVirusDlg::CheckPathOffLine(int type)
{
	TraverseCheckFilesOffLine(m_BrowserPath, type);
	MessageBox(L"������");
}

void CKillVirusDlg::TraverseCheckFilesOffLine(CString fullPath, int type)
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
																	   //ǰ����ʾ
			CString info;
			info.Format(L"%s", findFileData.cFileName);
			//��ȡ����·��
			CString strFullPath;
			strFilePath.TrimRight(L"\\*");
			strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
			TraverseCheckFilesOffLine(strFullPath, type);
		}
		else { //�ļ�
			   //����md5;
			CString strFullPath;
			strFullPath.Format(L"%s\\%s", fullPath, findFileData.cFileName);
			char chFullPath[256] = { 0 };
			WCHAR_TO_CHAR(strFullPath.GetBuffer(), chFullPath);
			char* md5 = md5FileValue(chFullPath);
			//������֤
			CString strTip;
			if (strlen(md5) == 0) {
				strTip = L"�޷���ȡmd5:";
				strTip.Append(strFullPath);
				MessageBox(strTip);
				continue;
			}
			//����������
			for (int i = 0; i < m_nVirtusBaseCount; i++) {
				if (strcmp(m_pVirusBase[i].md5, md5) == 0) { //�ǲ���
					if (type == 2 && m_pVirusBase[i].type == 2) { //ȫ·��Ҫ���·��
									 //��ȡ�ļ�·��
						CString strPath;
						int n = strFullPath.ReverseFind('\\');
						strPath = strFullPath.Left(n);
						char chFilePath[256] = { 0 };
						WCHAR_TO_CHAR(strPath, chFilePath);
						if (m_pVirusBase[i].type == 2 && strcmp(m_pVirusBase[i].path, chFilePath) == 0) {
							strTip = L"����·������:";
							strTip.Append(strFullPath);
							MessageBox(strTip);
							break;
						}
					}
					else if(type == 1){ //����md��ɱ
						strTip = "����md5����:";
						strTip.Append(strFullPath);
						MessageBox(strTip);
						break;
					}
				}

			}
			
			continue;
		}


	} while (FindNextFile(hFileList, &findFileData));
	//CloseHandle(hFileList);
	return;

}
void CKillVirusDlg::CheckProcessOffLine()
{
	//��ӽ��̱�������
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(L"�޷���ȡ���̿���");
		return ;
	}
	PROCESSENTRY32  pe32 = { sizeof(PROCESSENTRY32) };
	if (Process32First(hSnapshot, &pe32) != TRUE) {
		MessageBox(L"�޷���ȡ���̿����е��׸�����");
		CloseHandle(hSnapshot);
		return ;
	}
	do {
		CString  strExeFile;
		strExeFile = pe32.szExeFile;
		char chProcName[256] = { 0 };
		WCHAR_TO_CHAR(strExeFile, chProcName);
		//����������
		for (int i = 0; i < m_nVirtusBaseCount; i++) {
			if (strcmp(chProcName, m_pVirusBase[i].process) == 0) {
				CString strTip = L"���ֽ��̲���:";
				strTip.Append(strExeFile);
				MessageBox(strTip);
			}
		}
			

	} while (Process32Next(hSnapshot, &pe32));


	CloseHandle(hSnapshot);
	MessageBox(L"������");
}