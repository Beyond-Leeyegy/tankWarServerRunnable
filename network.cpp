/*
	
	FILE:network.cpp
	
	�����׽��ִ����Դ�����ļ�

*/

#include "def.h"
#include "structs.h"
#include "network.h"
#include "packet.h"
#include "linked_list.h"
#include "utils.h"
#include "message.h"

#include <string.h>
#include <stdio.h>

/*
	���������׽���
*/
extern SOCKET g_SOCK;

/*
	�ͻ���������
*/
extern sPCLIENT_DATA g_ClientList;

/*
	��������
*/
extern int g_TotalClient;

/*
	��ƽ̨��for win32
*/
#ifdef WIN32
void nonblock(SOCKET s) {
	u_long u10n = 1L;
	ioctlsocket(s, FIONBIO, (unsigned long *)&u10n);
}

#else


/*
	�����׽���ΪNoblocking
	��ƽ̨��for linux/freeBSD
*/
void nonblock(SOCKET s) {
	int flags;

	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0)
		log("* nonblock Error \r\n");
}

#endif
/*
��ʼ��������listen�׽���
*/
SOCKET InitServerSock(int port, int backlog) {
	struct  sockaddr_in sa;
	SOCKET sock = socket(AF_INET,SOCK_STREAM,0);

	if (sock < 0) {
		log(" InitServerSock(),socket(..)failed [PORT:%d]..\r\n", port);
		return -1;
	}

	/*
		���óɣ���ʹ��Ϊ�����������������Ҳ�������ͬ���ĵ�ַ����bind
	*/
#ifndef WIN32
	//���linuxƽ̨
	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
	{
		log("* Error: setsockopt REUSEADDR \r\n");
		exit(1);
	}
#endif // !WIN32

	/*
		LINGER���趨 ������
	*/
	struct linger ld;
	ld.l_onoff = 0;
	ld.l_linger = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ld, sizeof(ld)) < 0) {
		log("* Error: setsockopt SO_LINGER...\r\n");
	}


	/*
		�ṹ���ʼ��
	*/
	memset((char*)&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = INADDR_ANY;

	/*
		bind
	*/
	if (bind(sock, (const struct sockaddr *) &sa, sizeof(sa)) < 0) {

		log("InitServerSock(),bind(..) failed..[PORT:%d]\r\n", port);
		closesocket(sock);
		return -1;
	}

	/*
		����ΪNonblockingģʽ
	*/
	
}

/*
	�µ���������Ĵ���
	step1. accept()
	step2. �������ɵ��׽������ó�nonBlocking
	step3. ��ʼ��sPCLIENT_DATA 
	step4. �������ɵ�client���뵽�û������������ӵ��û���g_TotalClient
	step5. ������־
*/
void AcceptNewClient(SOCKET MotherSocket) {

	struct sockaddr_in peer;   //����¼�������ӵ��û���ip��ַ����Ϣ
	SOCKET newDesc;

#if defined (WIN32)
	/*
	WIN32
	*/
	int peersize;
#else
	/*
	linux
	*/
	socklen_t peersize;
#endif

	peersize = sizeof(peer);

	newDesc = accept(MotherSocket,(struct sockaddr*)&newDesc,&peersize);

	/*
		Error: accpet����ֵ�쳣
	*/
	if (newDesc < 0) {
		log("Accept new client failed!\r\n");
		return;
	}
	/*
		����nonBlocking
	*/
	nonblock(newDesc);

	/*
		�����ڴ�����û�
	*/
	sPCLIENT_DATA newClient = (sPCLIENT_DATA )malloc(sizeof(sCLIENT_DATA));

	/*
		Error:�ڴ����ʧ��
	*/
	if (!newClient) {
		closesocket(newDesc);
		return;
	}

	/*
		��ʼ�����û�������
	*/
	newClient->m_scok = newDesc;

	strcpy(newClient->m_IP, (char *)inet_ntoa(peer.sin_addr));

	//recv data 
	*newClient->m_recvBuff = '\0'; //�ɼ�->������������*
	newClient->m_recvPos = 0;
	newClient->m_recvSize = 0;


	*newClient->m_Name = '\0';

	//send data
	*newClient->m_sendBuff = '\0';
	newClient->m_sendSize = 0;

	// set time
	newClient->m_lastRecvTime = timeGetTime();

	// set pointer
	newClient->m_prev = NULL;
	newClient->m_next = NULL;

	/*
		��������
	*/
	INSERT_TO_LIST(g_ClientList, newClient, m_prev, m_next); // ???m_next m_prev�����ã�����
	g_TotalClient++;

	/*
		������־
	*/
	log("*Accept New Connection: %d [%s] \r\n", newDesc, newClient->m_IP);




}

/*
�Ͽ�����
*/
void DisconnectClient(sPCLIENT_DATA pClient) {
	REMOVE_FROM_LIST(g_ClientList, pClient, m_prev, m_next);
	if (pClient->m_scok != INVALID_SOCKET)
		closesocket(pClient->m_scok);

	free(pClient);
	pClient = NULL;
}

/*
�Ѵ��͵����ݸ��Ƶ�������m_sendBuff
*/
void SendData(sPCLIENT_DATA pClient, const char *data, int size) {
	if (pClient->m_sendSize + size > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient->m_scok);
		return;
	}

	memcpy(&pClient->m_sendBuff[pClient->m_sendSize], data, size);
	pClient->m_sendSize += size;

}

/*
	�����������еĿͻ��˷�������(ֻ�ŵ���Ӧ�ͻ������ݻ��������ȴ��������������ͣ�)
*/
void SendToAll(const char* data, int size) {
	sPCLIENT_DATA client, next_client;

	//????????
	LIST_WHILE(g_ClientList, client, next_client,m_next);
	SendData(client, data, size);
	LIST_WHILEEND(g_ClientList, client, next_client);
}

/*
	��շ��ͻ�����������ĳ�ͻ������͵����ݷ��ͳ�ȥ�����������ȫ������Ҳ�п���ֻ������һ����
	ʹ�ó�����ĳ���׽����з��������ʱ�򣬵��ô˺���
*/
int FlushSendBuff(sPCLIENT_DATA pClient) {

	int sendSize;   //��¼�������ͳ�ȥ�����ݵĳ��ȣ���λ�ֽڣ�������-���ͻ���
	sendSize = send(pClient->m_scok, pClient->m_sendBuff, pClient->m_sendSize, 0);


	/*
		Error: caused by send function
	*/
	if (sendSize <= 0) {
		log("Error:client[%d] call send function but failed...", pClient->m_scok);
		return -1;
	}


	/*
		if some data sent failed
	*/
	if (sendSize < pClient->m_sendSize) {
		memmove(&pClient->m_sendBuff[0], &pClient->m_sendBuff[sendSize], pClient->m_sendSize - sendSize);
		pClient->m_sendSize -= sendSize;
	}
	else
	{
		*pClient->m_sendBuff = '\0';
		pClient->m_sendSize = 0;
	}

	return sendSize;
	

}

/*
	ĳ���׽�����read�������ʱ�򣬵��ô˺���
*/
BOOL RecvFromClient(sPCLIENT_DATA pClient) {
	int recvSize;
	char recvBuff[dMAX_SOCK_BUFF];

	recvSize = recv(pClient->m_scok, recvBuff, 1024, 0);
	if (recvSize == 0)
		return 0;

	if (recvSize < 0) {

		/*
			���ֳ�Nonblock����Ĵ���ʱ����Ϊ���õ���nonblockingģʽ����û�����ݵ�ʱ���������ȴ������������ش�����룬�����������������Ҫ��������״̬������������״̬
		*/
#if defined(Win32)
	/*
		WIN32
	*/
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return 0;
#else
		/*
			LINUX
		*/
		if (errno != EWOULDBLOCK)
			return 0;  //������Ϊnonblocking���ƶ����ֵĴ��󡪡���Ĵ���
#endif
		else
		{
			return 1; //�ٵĴ��󡪡���nonBlocking��������
		}
	}
	/*
		Buffer Overflow
	*/
	if ((pClient->m_recvSize + recvSize) >= dMAX_SOCK_BUFF) {
		return 0;
	}

	// record time 
	pClient->m_lastRecvTime = timeGetTime();

	memcpy(&pClient->m_recvBuff[pClient->m_recvSize], recvBuff, recvSize);
	pClient->m_recvSize += recvSize;

	return 1;		// һ������
}
