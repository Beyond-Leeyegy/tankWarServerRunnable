/*
	FILE: message.cpp

	������յ����ݰ��ĺ���

*/
#include "def.h"
#include "structs.h"
#include "network.h"
#include "packet.h"
#include "linked_list.h"
#include "utils.h"
#include "message.h"
#include "game.h"
#include "Bullet.h"
#include <list>
#include <string.h>
#include <stdio.h>

/*
	ά������
*/
void PACKET_AliveCheck(sPCLIENT_DATA pClient) {

}

/*
	�޸Ĵº�
*/
void PACKET_ChangeNick(sPCLIENT_DATA pClient) {
	char nick[64];
	GetString(pClient->m_recvBuff, nick, pClient->m_recvPos);
	strcpy(pClient->m_Name, nick);


}

/*
	������Ϣ�����ͻ���-����������
	@param: sPCLIENT_DATA
	@���ܣ����ͻ��˷�������������Ϣ�㲥���д��ڵ�����

*/
//void PACKET_RecvMsg(sPCLIENT_DATA pClient) {
//	char msg[dMAX_SOCK_BUFF];
//	int packetPos;					
//	char packet[dMAX_SOCK_BUFF];	//packet to be sent
//	char tmp[dMAX_SOCK_BUFF];       // content to be sent
//
//
//	//step1. get msg
//	GetString(pClient->m_recvBuff, msg, pClient->m_recvPos);
//
//	//step2. complete content to be sent
//	sprintf(tmp,"%s : %s\r\n",pClient->m_Name,msg);
//
//	//step3.complete packet to be sent
//	packetPos = 2;
//	PutWord(packet, dPACKET_SEND_MSG, packetPos); // tag
//	PutString(packet, tmp, packetPos);			  // data
//	PutSize(packet, packetPos);					  // size
//
//	//step4. send packet to all
//	SendToAll(packet, packetPos);
//}

/*
	���´���������ݰ��������������ݰ��Զ����״̬���иı�
*/

/*
	����������ݰ���IS_READY
*/
void HANDLE_IS_READY(sPCLIENT_DATA pClient) {
	//���ÿͻ���״̬�ó�1����ʾ�Ѿ�׼����ʼ��Ϸ
	pClient->is_Ready = 1;

}

/*
����������ݰ���MAINTANK_POS

���ݰ��ṹ��size->tag->{Maintank.id:byte,x int ,y int,dir int}->
*/

void HANDLE_MAINTANK_POS(sPCLIENT_DATA pClient) {
	//���յ���client����̹�˵���Ϣ
	BYTE id;
	int pos_x, pos_y;
	int dir;

	int pos = 4;

	//getDATA
	id = GetByte(pClient->m_recvBuff, pos);
	pos_x = GetInteger(pClient->m_recvBuff, pos);
	pos_y = GetInteger(pClient->m_recvBuff, pos);

	dir = GetInteger(pClient->m_recvBuff, pos);

	//change state
	pClient->mainTank.SetDir((Dir)dir);
	pClient->mainTank.set_id(id);
	pClient->mainTank.set_pos(pos_x, pos_y);
	
	return;

}

/*
����������ݰ���NEW_BULLET
*/
/*
���ݰ��ṹ��size->tag->{Bullet.id:byte , x int ,y int,dir int}->
*/
void HANDLE_NEW_BULLET(sPCLIENT_DATA pClient,Game& game) {
	//get data 
	//���յ���client����̹�˵���Ϣ
	BYTE id;
	int pos_x, pos_y;
	int dir;

	int pos = 4;
	id = GetByte(pClient->m_recvBuff, pos);
	pos_x = GetInteger(pClient->m_recvBuff, pos);
	pos_y = GetInteger(pClient->m_recvBuff, pos);

	dir = GetInteger(pClient->m_recvBuff, pos);


	//new bullet
	Point point;
	point.Set(pos_x, pos_y);
	Bullet* pBullet = new Bullet(point, (Dir)dir, pClient->mainTank.get_color());
	pBullet->SetStep(20);
	pBullet->set_id(id);
	game.lstMainTankBullets.push_back(pBullet);

}


/*
	���ݴ������������ݰ����в�ͬ�Ĵ���
	������״̬ȷ�ϣ�
	  �޸�nickname��
	  ������Ϣ���ͻ�-������������
	  ������Ϣ��������-���ͻ�����

	  @���ܣ�����ĳ�û����еȴ�����������ݰ�
	  NOTICE:ÿ����һ�����ݰ����û���m_recvBuff�ͻ�ɾ���Ѿ���������ݰ���ͬʱ����m_recvSize
*/
BOOL ReadRecvBuff(sPCLIENT_DATA pClient,Game& game) {
	WORD msgSize;
	WORD tag;

	while (pClient->m_recvSize > 0) {
		pClient->m_recvPos = 0;

		//step1. get packet_size and tag  
		msgSize = GetWord(pClient->m_recvBuff, pClient->m_recvPos);
		tag = GetWord(pClient->m_recvBuff, pClient->m_recvPos);

		/*
			��Ŀǰm_recvBuff�����ݵĳ���С�����ݰ����ܳ���ʱ
		*/
		if (pClient->m_recvSize < msgSize) {
			return 1;							//???
		}

		/*
			���ݰ�����
		*/
		switch (tag)
		{
		case dPACKET_ALIVE_CHECK:
			PACKET_AliveCheck(pClient);
			break;
		case dPACKET_CHANGE_NICK:
			PACKET_ChangeNick(pClient);
			break;
		case dPACKET_IS_READY: 
			HANDLE_IS_READY(pClient);
			break;
		case dPACKET_NEW_BULLET:
			HANDLE_NEW_BULLET(pClient,game);
			break;
		case dPACKET_MAINTANK_POS: 
			HANDLE_MAINTANK_POS(pClient);
			break;
		}


		/*
			��ʾ�Ѿ�������ˣ���˿ռ����Ǩ��
		*/
		memmove(&pClient->m_recvBuff[0],
			&pClient->m_recvBuff[msgSize],
			pClient->m_recvSize - msgSize);
		pClient->m_recvSize -= msgSize;

	}
	return 1;
}