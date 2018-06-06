// ResourceDirDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ResourceDirDlg.h"
#include "afxdialogex.h"


// CResourceDirDlg �Ի���

IMPLEMENT_DYNAMIC(CResourceDirDlg, CDialogEx)

CResourceDirDlg::CResourceDirDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Resource, pParent)
	, m_RootNumOfNamedEntries(_T(""))
	, m_RootNumOfIdEntries(_T(""))
	, m_NumOfNamedEntries2(_T(""))
	, m_NumOfIdEntries2(_T(""))
	, m_3LayerRVA(_T(""))
	, m_3LayerOffset(_T(""))
	, m_3LayerSize(_T(""))
{

}

CResourceDirDlg::~CResourceDirDlg()
{
}

void CResourceDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Root_NumOfNamedEntries, m_RootNumOfNamedEntries);
	DDX_Text(pDX, IDC_EDIT_Root_NumOfIdEntries, m_RootNumOfIdEntries);
	DDX_Text(pDX, IDC_EDIT_2_NumOfNamedEntries, m_NumOfNamedEntries2);
	DDX_Text(pDX, IDC_EDIT_2_NumOfIdEntries, m_NumOfIdEntries2);
	DDX_Text(pDX, IDC_EDIT_3_RVA, m_3LayerRVA);
	DDX_Text(pDX, IDC_EDIT_3_Offset, m_3LayerOffset);
	DDX_Text(pDX, IDC_EDIT_3_Size, m_3LayerSize);
	DDX_Control(pDX, IDC_TREE_Resource, m_RescTreeCtrl);
}


BEGIN_MESSAGE_MAP(CResourceDirDlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_Resource, &CResourceDirDlg::OnTvnSelchangedTreeResource)
END_MESSAGE_MAP()

void CResourceDirDlg::ShowRescInfo()
{
	//������ʽ
	DWORD dwStyle = m_RescTreeCtrl.GetStyle();
	dwStyle = dwStyle | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SINGLEEXPAND;
	m_RescTreeCtrl.ModifyStyle(0, dwStyle);
	//
	WCHAR*  pResourceType[] = { L"0", L"���ָ��",L"bitmap",L"icon", L"menu", L"dialog", L"string table", L"fontDir", L"font", L"shortCut", L"noFormatResource", L"MessageList", L"mouserCursorGroup",L"D",  L"iconGroup",L"F", L"versionInfo" };
	DWORD dwRescAddrRVA = m_pPE->m_pDataDir[2].VirtualAddress;
	DWORD dwRescAddrFOA = m_pPE->RVA2FOA(dwRescAddrRVA);
	DWORD dwRescAddrOfFile = dwRescAddrFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_RESOURCE_DIRECTORY* pRescDir = (IMAGE_RESOURCE_DIRECTORY*)dwRescAddrOfFile;
	WORD dwRootNumOfNamedEntries = pRescDir->NumberOfNamedEntries;
	WORD dwRootNumOfIdEntries = pRescDir->NumberOfIdEntries;
	WORD dwRootCountEntries = dwRootNumOfNamedEntries + dwRootNumOfIdEntries;
	//��Ŀ¼����չʾ
	m_RootNumOfNamedEntries.Format(L"%04X", dwRootNumOfNamedEntries);
	m_RootNumOfIdEntries.Format(L"%04X", dwRootNumOfIdEntries);
	UpdateData(false);
	//������Ϣ
	IMAGE_SECTION_HEADER* pSectionHeader = m_pPE->GetSectionHeaderByRva(dwRescAddrRVA);
	DWORD dwSectionFOA = pSectionHeader->PointerToRawData; // ��ǰ����ʼλ�õ�FOA
	//�����Դ���ؼ�
	HTREEITEM hRootTree = TVI_ROOT; // ����
	HTREEITEM hSecondTree;
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pRootDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(DWORD)(pRescDir + 1);
	for (WORD i = 0; i < dwRootCountEntries; i++) {
		//���ƻ����
		if(pRootDirEntry->NameIsString) {//������Դ
			DWORD dwRootNameOffset = pRootDirEntry->NameOffset;//����ڵ�ǰ�ļ����ε�ƫ��
			DWORD dwRootNameFOA = dwSectionFOA + dwRootNameOffset;
			DWORD dwRootNameAddrOfFile = dwRootNameFOA + (DWORD)m_pPE->m_pFileBuffer;
			IMAGE_RESOURCE_DIR_STRING_U* pRootStringU = (IMAGE_RESOURCE_DIR_STRING_U*)dwRootNameAddrOfFile;
			WORD wRootNameLen = pRootStringU->Length;
			WCHAR * pRootName = new WCHAR[wRootNameLen + 1]{ 0 };
			memcpy_s(pRootName, wRootNameLen * 2, pRootStringU->NameString, wRootNameLen * 2);
			//�������ؼ�
			hSecondTree = m_RescTreeCtrl.InsertItem(pRootName, hRootTree);

		}
		else { //id��Դ
			DWORD dwRootId = pRootDirEntry->Name;
			CString strRootIdType;
			if (dwRootId <= 16) {
				strRootIdType.Format(L"%s", pResourceType[dwRootId]);
			}
			else {
				strRootIdType.Format(L"%d", dwRootId, dwRootId);
			}
			hSecondTree = m_RescTreeCtrl.InsertItem(strRootIdType, hRootTree);
		}
		//�ڶ���, ÿ����Դ�ĸ���, ����
		if (pRootDirEntry->DataIsDirectory) { //˵��������һ��
			DWORD dwSecondDirOffset = pRootDirEntry->OffsetToDirectory;
			DWORD dwSecondDirFOA = dwSecondDirOffset + dwSectionFOA;
			DWORD dwSecondDirAddrOfFile = dwSecondDirFOA + (DWORD)m_pPE->m_pFileBuffer;
			IMAGE_RESOURCE_DIRECTORY* pSecondDir = (IMAGE_RESOURCE_DIRECTORY*)dwSecondDirAddrOfFile;
			//����
			WORD dwSecondNumOfNamedEntries = pSecondDir->NumberOfNamedEntries;
			WORD dwSecondNumOfIdEntries = pSecondDir->NumberOfIdEntries;
			WORD dwSecondCountEntries = dwSecondNumOfNamedEntries + dwSecondNumOfIdEntries;
			IMAGE_RESOURCE_DIRECTORY_ENTRY* pSecondDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pSecondDir + 1);
			for (WORD j = 0; j < dwSecondCountEntries; j++) {
				if (pSecondDirEntry->NameIsString) {
					DWORD dwSecondNameOffset = pSecondDirEntry->NameOffset;
					DWORD dwSecondNameFOA = dwSecondNameOffset + dwSectionFOA;
					DWORD dwSecondNameAddrOfFile = dwSecondNameFOA + (DWORD)m_pPE->m_pFileBuffer;
					IMAGE_RESOURCE_DIR_STRING_U* pSecondStringU = (IMAGE_RESOURCE_DIR_STRING_U*)dwSecondNameAddrOfFile;
					WORD wSecondNameLen = pSecondStringU->Length;
					WCHAR * pSecondName = new WCHAR[wSecondNameLen + 1]{ 0 };
					memcpy_s(pSecondName, wSecondNameLen * 2, pSecondStringU->NameString, wSecondNameLen * 2);
					//�������ؼ�
					m_RescTreeCtrl.InsertItem(pSecondName, hSecondTree);
				}
				else { // �������
					DWORD dwSecondId = pSecondDirEntry->Name;
					CString strSecondIdType;
					strSecondIdType.Format(L"%d", dwSecondId, dwSecondId);
					m_RescTreeCtrl.InsertItem(strSecondIdType, hSecondTree);

				}
				pSecondDirEntry++;
			}
		}


		pRootDirEntry++;
	}
	

}

// CResourceDirDlg ��Ϣ�������


BOOL CResourceDirDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowRescInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CResourceDirDlg::UpdateByClick(CString strLay1, CString strLay2)
{
	WCHAR*  pResourceType[] = { L"0", L"���ָ��",L"bitmap",L"icon", L"menu", L"dialog", L"string table", L"fontDir", L"font", L"shortCut", L"noFormatResource", L"MessageList", L"mouserCursorGroup",L"D",  L"iconGroup",L"F", L"versionInfo" };
	DWORD dwRescAddrRVA = m_pPE->m_pDataDir[2].VirtualAddress;
	DWORD dwRescAddrFOA = m_pPE->RVA2FOA(dwRescAddrRVA);
	DWORD dwRescAddrOfFile = dwRescAddrFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_RESOURCE_DIRECTORY* pRescDir = (IMAGE_RESOURCE_DIRECTORY*)dwRescAddrOfFile;
	WORD dwRootNumOfNamedEntries = pRescDir->NumberOfNamedEntries;
	WORD dwRootNumOfIdEntries = pRescDir->NumberOfIdEntries;
	WORD dwRootCountEntries = dwRootNumOfNamedEntries + dwRootNumOfIdEntries;
	//��Ŀ¼����չʾ
	m_RootNumOfNamedEntries.Format(L"%04X", dwRootNumOfNamedEntries);
	m_RootNumOfIdEntries.Format(L"%04X", dwRootNumOfIdEntries);
	UpdateData(false);
	//������Ϣ
	IMAGE_SECTION_HEADER* pSectionHeader = m_pPE->GetSectionHeaderByRva(dwRescAddrRVA);
	DWORD dwSectionFOA = pSectionHeader->PointerToRawData; // ��ǰ����ʼλ�õ�FOA
														   //�����Դ���ؼ�
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pRootDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(DWORD)(pRescDir + 1);
	BOOL bThisLayer1 = false;
	for (WORD i = 0; i < dwRootCountEntries; i++) {
		//���ƻ����
		if (pRootDirEntry->NameIsString) {//������Դ
			DWORD dwRootNameOffset = pRootDirEntry->NameOffset;//����ڵ�ǰ�ļ����ε�ƫ��
			DWORD dwRootNameFOA = dwSectionFOA + dwRootNameOffset;
			DWORD dwRootNameAddrOfFile = dwRootNameFOA + (DWORD)m_pPE->m_pFileBuffer;
			IMAGE_RESOURCE_DIR_STRING_U* pRootStringU = (IMAGE_RESOURCE_DIR_STRING_U*)dwRootNameAddrOfFile;
			WORD wRootNameLen = pRootStringU->Length;
			WCHAR * pRootName = new WCHAR[wRootNameLen + 1]{ 0 };
			memcpy_s(pRootName, wRootNameLen * 2, pRootStringU->NameString, wRootNameLen * 2);
			//�������ؼ�
			//�ж��Ƿ��ǵ���Ĵ˴�
			CString strRootName;
			strRootName.Format(L"%s", pRootName);
			if (strRootName == strLay1) {
				bThisLayer1 = true;
			}

		}
		else { //id��Դ
			DWORD dwRootId = pRootDirEntry->Name;
			CString strRootIdType;
			if (dwRootId <= 16) {
				strRootIdType.Format(L"%s", pResourceType[dwRootId]);
			}
			else {
				strRootIdType.Format(L"%d", dwRootId, dwRootId);
			}
			//�������ؼ�
			//�ж��Ƿ��ǵ���Ĵ˴�
			if (strRootIdType == strLay1) {
				bThisLayer1 = true;
			}
		}

		if (bThisLayer1 == false) {
			pRootDirEntry++;    // error: �������Ӵ�ָ��
			continue; //���Ǵ˽ڵ�
		}
		//�ڶ���, ÿ����Դ�ĸ���, ����
		if (pRootDirEntry->DataIsDirectory) { //˵��������һ��
			DWORD dwSecondDirOffset = pRootDirEntry->OffsetToDirectory;
			DWORD dwSecondDirFOA = dwSecondDirOffset + dwSectionFOA;
			DWORD dwSecondDirAddrOfFile = dwSecondDirFOA + (DWORD)m_pPE->m_pFileBuffer;
			IMAGE_RESOURCE_DIRECTORY* pSecondDir = (IMAGE_RESOURCE_DIRECTORY*)dwSecondDirAddrOfFile;
			//����
			WORD dwSecondNumOfNamedEntries = pSecondDir->NumberOfNamedEntries;
			WORD dwSecondNumOfIdEntries = pSecondDir->NumberOfIdEntries;
			WORD dwSecondCountEntries = dwSecondNumOfNamedEntries + dwSecondNumOfIdEntries;
			IMAGE_RESOURCE_DIRECTORY_ENTRY* pSecondDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pSecondDir + 1);
			if (bThisLayer1) {
				//�޸���Ŀ������ʾ
				m_NumOfIdEntries2.Format(L"%04X", dwSecondNumOfIdEntries);
				m_NumOfNamedEntries2.Format(L"%04X", dwSecondNumOfNamedEntries);
				UpdateData(false);
				if (strLay2 == L"") { //�ҵ��˸��ڵ�, ����û�е���ӽڵ�, ����
						return;
				}
			}
			BOOL bThisLayer2 = false;
			for (WORD j = 0; j < dwSecondCountEntries; j++) {
				if (pSecondDirEntry->NameIsString) {
					DWORD dwSecondNameOffset = pSecondDirEntry->NameOffset;
					DWORD dwSecondNameFOA = dwSecondNameOffset + dwSectionFOA;
					DWORD dwSecondNameAddrOfFile = dwSecondNameFOA + (DWORD)m_pPE->m_pFileBuffer;
					IMAGE_RESOURCE_DIR_STRING_U* pSecondStringU = (IMAGE_RESOURCE_DIR_STRING_U*)dwSecondNameAddrOfFile;
					WORD wSecondNameLen = pSecondStringU->Length;
					WCHAR * pSecondName = new WCHAR[wSecondNameLen + 1]{ 0 };
					memcpy_s(pSecondName, wSecondNameLen * 2, pSecondStringU->NameString, wSecondNameLen * 2);
					//�������ؼ�
					CString strSecondName;
					strSecondName.Format(L"%s", pSecondName);
					if (strLay2 == strSecondName) {
						bThisLayer2 = true;
					}
				}
				else { // �������
					DWORD dwSecondId = pSecondDirEntry->Name;
					CString strSecondIdType;
					strSecondIdType.Format(L"%d",  dwSecondId);
					//�������ؼ�
					if (strLay2 == strSecondIdType) {
						bThisLayer2 = true;
					}

				}
				if (bThisLayer2 == false) { // ���ǵ���Ĵ˽ڵ�, ���ٱ���������
					pSecondDirEntry++; // error: continue, ��������ָ��
					continue;
				}
				//�ж��Ƿ�����һ��
				if (pSecondDirEntry->DataIsDirectory) { // ������һ��: ������
					DWORD dwThirdDirOffset = pSecondDirEntry->OffsetToDirectory;
					DWORD dwThirdDirFOA = dwThirdDirOffset + dwSectionFOA;
					DWORD dwThirdDirAddrOfFile = dwThirdDirFOA + (DWORD)m_pPE->m_pFileBuffer;
					IMAGE_RESOURCE_DIRECTORY* pThirdDir = (IMAGE_RESOURCE_DIRECTORY*)dwThirdDirAddrOfFile;
					IMAGE_RESOURCE_DIRECTORY_ENTRY* pThirdDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pThirdDir + 1);
					//ֱ��������data, ��ʹ��for
					DWORD dwThirdDataOffset = pThirdDirEntry->OffsetToData;
					DWORD dwThirdDataFOA = dwThirdDataOffset + dwSectionFOA;
					DWORD dwThirdDataAddrOfFile = dwThirdDataFOA + (DWORD)m_pPE->m_pFileBuffer;
					IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)dwThirdDataAddrOfFile;
					
					//�����Ҳ�����
					m_3LayerRVA.Format(L"%08x", pDataEntry->OffsetToData);
					m_3LayerSize.Format(L"%08X", pDataEntry->Size);
					m_3LayerOffset.Format(L"%08X", m_pPE->RVA2FOA(pDataEntry->OffsetToData));
					UpdateData(false);
					return;

				}
				pSecondDirEntry++;
			}
		}


		pRootDirEntry++;
	}

}

void CResourceDirDlg::OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	HTREEITEM hClickedItem = m_RescTreeCtrl.GetSelectedItem();
	HTREEITEM hParentItem = m_RescTreeCtrl.GetParentItem(hClickedItem);
	CString strLay1;
	CString strLay2;
	if (hParentItem == NULL) { // ����ĵ�һ��
		strLay1 = m_RescTreeCtrl.GetItemText(hClickedItem);
		strLay2 = L"";
	}
	else { //����ĵڶ���
		strLay2 = m_RescTreeCtrl.GetItemText(hClickedItem);
		strLay1 = m_RescTreeCtrl.GetItemText(hParentItem);

	}

	UpdateByClick(strLay1, strLay2);
}


