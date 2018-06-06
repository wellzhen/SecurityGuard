// RelocationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "RelocationDlg.h"
#include "afxdialogex.h"


// CRelocationDlg �Ի���

IMPLEMENT_DYNAMIC(CRelocationDlg, CDialogEx)

CRelocationDlg::CRelocationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Relocation, pParent)
{

}

CRelocationDlg::~CRelocationDlg()
{
}

void CRelocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SectionBlong, m_ListSectionBelong);
	DDX_Control(pDX, IDC_LIST_EntriesOfBlock, m_ListEntryOfBlock);
}


BEGIN_MESSAGE_MAP(CRelocationDlg, CDialogEx)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SectionBlong, &CRelocationDlg::OnLvnItemchangedListSectionblong)
	//ON_NOTIFY(NM_CLICK, IDC_LIST_EntriesOfBlock, &CRelocationDlg::OnClickListEntriesofblock)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SectionBlong, &CRelocationDlg::OnClickListSectionblongList)
END_MESSAGE_MAP()

void CRelocationDlg::ShowRelocationInfo()
{
	DWORD dwRelocRVA = m_pPE->m_pDataDir[5].VirtualAddress;
	DWORD dwRelocFOA = m_pPE->RVA2FOA(dwRelocRVA);
	DWORD dwRelocAddrOfFile = dwRelocFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_BASE_RELOCATION* pBaseReloc = (IMAGE_BASE_RELOCATION*)dwRelocAddrOfFile;
	DWORD index = 1;
	while (pBaseReloc->SizeOfBlock) {
		CString strIndex, strSection, strRVA, strItems;
		DWORD dwCountItems = ((pBaseReloc->SizeOfBlock) - 8) / 2;
		DWORD dwRelocRVA = pBaseReloc->VirtualAddress;
		IMAGE_SECTION_HEADER* pSectionHeader = m_pPE->GetSectionHeaderByRva(dwRelocRVA);
		strIndex.Format(L"%d", index);
		char* name = (char*)pSectionHeader->Name;
		WCHAR wName[100] = { 0 };
		swprintf_s(wName, L"%hs", name);
		strSection.Format(L"[ %s ]", wName);
		strRVA.Format(L"%08X", dwRelocRVA);
		strItems.Format(L"%02X / %d", dwCountItems, dwCountItems);
		//������
		m_ListSectionBelong.InsertItemCustom(4, strIndex, strSection, strRVA, strItems);
		index++;
		pBaseReloc = (IMAGE_BASE_RELOCATION*)((DWORD)pBaseReloc + (DWORD)pBaseReloc->SizeOfBlock);
	}
}

// CRelocationDlg ��Ϣ�������


BOOL CRelocationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����
	m_ListSectionBelong.InsertColumnCustom(4, 0.1, L"index", 0.2, L"Section", 0.2, L"RVA", 0.2, L"Items");
	m_ListEntryOfBlock.InsertColumnCustom(5, 0.1, L"Index", 0.15, L"RVA", 0.15, L"Offset", 0.2, L"Type", 0.15, L"Far Address");
	ShowRelocationInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//void CRelocationDlg::OnLvnItemchangedListSectionblong(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//
//	//�հ�������Ӧ
//	MessageBox(L"ok");
//}


//void CRelocationDlg::OnClickListEntriesofblock(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//	//�հ�������Ӧ
//	MessageBox(L"clicked");
//	if (pNMItemActivate->iItem == -1) {
//		return;
//	}
//	MessageBox(L"ok");
//}


void CRelocationDlg::OnClickListSectionblongList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//�հ�������Ӧ
	if (pNMItemActivate->iItem == -1) {
		return;
	}
	//�������
	m_ListEntryOfBlock.DeleteAllItems();
	//������
	CString strClickedIndex = m_ListSectionBelong.GetItemText(pNMItemActivate->iItem, 0);//ʹ��Index��Ϊ����

	DWORD dwRelocRVA = m_pPE->m_pDataDir[5].VirtualAddress;
	DWORD dwRelocFOA = m_pPE->RVA2FOA(dwRelocRVA);
	DWORD dwRelocAddrOfFile = dwRelocFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_BASE_RELOCATION* pBaseReloc = (IMAGE_BASE_RELOCATION*)dwRelocAddrOfFile;
	DWORD index = 1;

	typedef struct _TypeOffset {
		WORD Offset : 12;
		WORD Type : 4;
	} TypeOffset;


	while (pBaseReloc->SizeOfBlock) {
		CString curIndex;
		curIndex.Format(L"%d", index);
			

		DWORD dwCountItems = ((pBaseReloc->SizeOfBlock) - 8) / 2;
		DWORD dwRelocRVA = pBaseReloc->VirtualAddress;
		if (curIndex == strClickedIndex) { //���������
			CString strIndex, strRVA, strOffset, strType, strFarAddress;
			for (DWORD j = 0; j < dwCountItems; j++) {
				TypeOffset* pTypeOffset =(TypeOffset*) ((DWORD)pBaseReloc + 8 + j * 2);
				DWORD dwDataRVA = pTypeOffset->Offset + pBaseReloc->VirtualAddress;
				DWORD dwDataFOA = m_pPE->RVA2FOA(dwDataRVA);
				WORD  dwDataType = pTypeOffset->Type;
				DWORD dwDataAddrOfFile = dwDataFOA + (DWORD)m_pPE->m_pFileBuffer;
				DWORD dwDataValue = *(DWORD*)dwDataAddrOfFile;
				if (dwDataType == 3) {
					strRVA.Format(L"%08X", dwDataRVA);
					strOffset.Format(L"%08X", dwDataFOA);
					strType.Format(L"HIGHLOW [%d]", dwDataType);
					strFarAddress.Format(L"%08X", dwDataValue);
				}
				else {
					strRVA = L"-";
					strOffset = L"-";
					strType.Format(L"ABSOLUTE [%d]", dwDataType);
					strFarAddress = L"-";
				}
				strIndex.Format(L"%d", j+1);
				
				
				
				m_ListEntryOfBlock.InsertItemCustom(5, strIndex, strRVA, strOffset, strType, strFarAddress);
			}
			return;
		}







		index++;
		pBaseReloc = (IMAGE_BASE_RELOCATION*)((DWORD)pBaseReloc + (DWORD)pBaseReloc->SizeOfBlock);
	}
}
