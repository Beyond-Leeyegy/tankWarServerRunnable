#pragma once
/*
	FILE: message.h



*/

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "structs.h"
#include "game.h"

#define dPACKET_ALIVE_CHECK		0	//��������״̬��ȷ��
#define dPACKET_CHANGE_NICK		1	//�ºŵ��޸�
#define dPACKET_MAINTANK_POS	2   //�ͻ�-��������  ���������Լ���λ��
#define dPACKET_IS_READY		3	//�ͻ�-�������� ��ʾ�Լ�׼����ʼ��Ϸ
#define dPACKET_NEW_BULLET		4	//�ͻ�-�������� �µ��ӵ�������ʱ��ͻᴫ������ݰ�(id,x,y,dir)
#define dPACKET_ALL_POS			5	//������-���ͻ�  ����Ԫ�ص�λ����Ϣ��each item:id,x,y,dir��(tank / bullet / )
#define dPACKET_BOOM_ACT		6   //������-���ͻ� ִ�б�ը��Ԫ�أ�̹�� & ը������id,boom_x,boom_Y��
#define dPACKET_FRIEND			7	//������-���ͻ� �������ҵ���һ����֮��ͬ��ս�Ķ��ѣ���Ϣ:�Է���nickName��ip��ַ



//new added

/*
ά������
*/
void PACKET_AliveCheck(sPCLIENT_DATA pClient);

/*
�޸Ĵº�
*/
void PACKET_ChangeNick(sPCLIENT_DATA pClient);

/*
������Ϣ�����ͻ���-����������
@param: sPCLIENT_DATA
@���ܣ����ͻ��˷�������������Ϣ�㲥���д��ڵ�����

*/
//void PACKET_RecvMsg(sPCLIENT_DATA pClient);



/*
	����������ݰ���IS_READY
*/

void HANDLE_IS_READY(sPCLIENT_DATA pClient);

/*
	����������ݰ���MAINTANK_POS
*/
void HANDLE_MAINTANK_POS(sPCLIENT_DATA pClient);

/*
	����������ݰ���NEW_BULLET
*/
void HANDLE_NEW_BULLET(sPCLIENT_DATA pClient, Game& game);

/*
���ݴ������������ݰ����в�ͬ�Ĵ���
������״̬ȷ�ϣ�
�޸�nickname��
������Ϣ���ͻ�-������������
������Ϣ��������-���ͻ�����
*/
BOOL ReadRecvBuff(sPCLIENT_DATA pClient,Game& game);


#endif // !__MESSAGE_H__
