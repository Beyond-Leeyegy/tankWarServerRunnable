#pragma once
/*
	FILE:network.h

	�׽��ִ�������ͷ�ļ�

*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "def.h"
#include "structs.h"

/*
	�����׽���ΪNoblocking
*/
void nonblock(SOCKET s);

/*
	��ʼ��������listen�׽���
*/
SOCKET InitServerSock(int port, int backlog);

/*
	�µ���������Ĵ���
*/
void AcceptNewClient(SOCKET MotherSocket);

/*
	�Ͽ�����
*/
void DisconnectClient(sPCLIENT_DATA pClient);

/*
	�Ѵ��͵����ݸ��Ƶ�������
*/
void SendData(sPCLIENT_DATA pClient, const char *data, int size);

/*
	�����������еĿͻ��˷�������
*/
void SendToAll(const char* data, int size);

/*
	��շ��ͻ�����
*/
int FlushSendBuff(sPCLIENT_DATA pClient);

/*
	recv
*/
BOOL RecvFromClient(sPCLIENT_DATA pClient);

#endif // !__NETWORK_H__

