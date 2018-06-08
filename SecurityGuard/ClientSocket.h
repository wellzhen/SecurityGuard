#pragma once
#include "Data.h"
#include <vector>
using std::vector;
#define BUFNAME 50


class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
public:
	//��ʼ���ͻ����׽���, ���������ӷ�����
	bool ConnectServer(char* szIp, WORD port);
	//���𴴽��߳��������շ�������Ϣ
	char* Recv();
	bool Send(INTERACTPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();

private:
	// send
	void SendForSampleMd5(char* bufSend, DWORD dwLen);
	// recv
	char* RecvForSampleMd5();
public:
	//�ͻ����׽���
	SOCKET m_sClient = NULL;
	//����Ϣ��ʱ����
	CONTENTSEND* m_pObjRecv;

private:

	char m_bufRecv[BUFMSG] = {};


};

