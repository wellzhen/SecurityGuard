#pragma once
//Э��
#define BUFMSG 2048 //һ���Խ��յ�����ֽ���
#define SERVERIP  "127.0.0.1"
#define SERVERPORT  1234

//ͷ��: ����ͨѶĿ��, ���� ������Ϣmd5��Ϣ
enum INTERACTPURPOSE {
	eSAMPLE = 1
};


//������ϢMD5�Ľṹ��
typedef struct _SAMPLEMD5 {
	char szMD5[32]; // md5ֵ
} SAMPLEMD5;

//��������
typedef struct _CONTENTSEND {
	INTERACTPURPOSE  m_type;
	union {
		char buf[BUFMSG];
		SAMPLEMD5  sample;
	} m_content;
} CONTENTSEND;


typedef struct _WATCHINFO {
	int type; // 1 md5  ; 2  path md5;  3; process;
	char md5[33];
	char path[256];
	char process[256];
} WATCHINFO;