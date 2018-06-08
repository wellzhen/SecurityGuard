#include "stdafx.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "ClientSocket.h"


CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}
//��ʼ���ͻ����׽���, ���������ӷ�����
bool CClientSocket::ConnectServer(char* szIp, WORD port)
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
		return false;
	}
	//��鷵�صĿ�汾�Ƿ���2.2
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2) {
		return false;
	}
	//��������socket
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}
	//���÷�������ַ
	SOCKADDR_IN servAddr = {};
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons((short)port);
	//���ӷ�����
	int nRetValue = connect(m_sClient, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (nRetValue == SOCKET_ERROR) {
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}
	return true;
}
//���𴴽��߳��������շ�������Ϣ
char* CClientSocket::Recv()
{
	CONTENTSEND ct = {};
	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CONTENTSEND), NULL)) {
		MessageBox(NULL,L"recv  msg error !", L"ClientSocket", MB_OK);
		return nullptr;
	}
	//change
	if (!strcmp(ct.m_content.sample.szMD5 , "true") ){
		return "true";
	}
	else {
		return "false";
	}
	/*
	switch (ct.m_type) {
	case eSAMPLE:
		return RecvForSampleMd5();
		break;
	default:
		OutputDebugString(L"��Ч����Ϣ����");
		break;
	}
	return nullptr;
	*/

}
bool CClientSocket::Send(INTERACTPURPOSE purpose, char* bufSend, DWORD dwLen)
{
	switch (purpose) {
	case eSAMPLE:
		SendForSampleMd5(bufSend, dwLen);
		break;
	default:
		break;
		return false;
	}
	return true;
}
bool CClientSocket::Close()
{
	WSACleanup();
	closesocket(m_sClient);
	return true;
}

// send
void  CClientSocket::SendForSampleMd5(char* bufSend, DWORD dwLen)
{
	CONTENTSEND ct = { eSAMPLE };
	memcpy_s(ct.m_content.sample.szMD5, dwLen, bufSend, dwLen);
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);


}
// recv
char* CClientSocket::RecvForSampleMd5()
{
	if (!strcmp(m_pObjRecv->m_content.sample.szMD5, "true")) {
		return "true";
	}
	else {
		return nullptr;
	}
	return nullptr;
}