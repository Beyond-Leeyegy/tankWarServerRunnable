#pragma once
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "def.h"
#include "MainTank.h"

#define dMAX_SOCK_BUFF 1024	// buffer length
#define dNAME_LEN 50		// name length 
#define dIP_LEN 20			// IP length

struct client_data
{
	/*
		�������ݴ��͵��׽��ֵȸ������ݱ���
	*/

	// network communication
	int m_scok;			// ���������׽���
	char m_IP[dIP_LEN];     // IP  address
	
	// recv data from client
	char m_recvBuff[dMAX_SOCK_BUFF];	//A packet
	int  m_recvSize;	//�ܵ����ݰ����ϵĳ��ȣ�һ���ͻ������ж�����ݰ��ȴ�������
	int  m_recvPos;     //��Ӧ���ݵĵ�ַƫ�ƣ������m_recvBuff�׵�ַ��

	// send data to client
	char m_sendBuff[dMAX_SOCK_BUFF];	//A packet
	int  m_sendSize;

	/*
		new added 
	*/
	int in_Game = 0; //������û��Ƿ����ڽ�����Ϸ

	/*
		new added
	*/
	MainTank mainTank;

	/*
		new added here
	*/
	int is_Ready = 0;//��������ӵĿͻ��Ƿ�׼��������Ϸ


	
	/*
		 name
	 */
	char m_Name[dNAME_LEN];	
	
	
	
	/*
		��������״̬ȷ�ϵ����recvʱ��ֵ
	*/
	unsigned long m_lastRecvTime;



	/*
		�����������ӵ�ָ��
	*/
	struct client_data *m_prev;
	struct client_data *m_next;

};

/*
	�ṹ��������
*/

typedef struct client_data sCLIENT_DATA, *sPCLIENT_DATA;


#endif 
