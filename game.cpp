#include "game.h"
#include "message.h"
#include "packet.h"
#include "Bullet.h"
#include "utils.h"
#include "Shape.h"
#include "Setting.h"
#include "EnemyTank.h"
#include "network.h"
#include "def.h"
/*
����all_pos���ݰ�

Game::Game(sPCLIENT_DATA Client1 , sPCLIENT_DATA Client2)
{
        pClient1 = Client1;
        pClient2 = Client2;
}

Game::~Game()
{
}


ALL_POS���ݰ��ṹ��size word->tag word->��ս̹��1����{id:byte,x:int,y:int.dir:int,̹������ֵ:byte}
					->�Ѿ���ս̹������{����ͬ��}
					->�ӵ�������{�ӵ�����:byte,loop{id:byte,x:int,y:int,dir:int,type:byte0�����Ҿ����ϣ�1����о�����}}
					->����̹������{̹����:byte,loop{id:byte,x:int,y:int,dir:int}}

Ҫ���� ��������ʱд�����ݵĻ���������
*/
Game::Game(sPCLIENT_DATA Client1 , sPCLIENT_DATA Client2)
{
        pClient1 = Client1;
        pClient2 = Client2;
}

Game::~Game()
{
}

Game::Game(){}
void Game::onSend_ALL_POS() {
	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];
	
	//��װ�û�1��ͷ��

	int pos_1 = 2;
	PutWord(content_to_1, (WORD)dPACKET_ALL_POS, pos_1);
	/*
		MYSELF
	*/
	//id
	PutByte(content_to_1, pClient1->mainTank.get_id(), pos_1);

	//x:int
	PutInteger(content_to_1, pClient1->mainTank.get_pos().GetX(), pos_1);

	//y:int
	PutInteger(content_to_1, pClient1->mainTank.get_pos().GetY(), pos_1);

	//dir:int
	PutInteger(content_to_1, pClient1->mainTank.GetDir(), pos_1);

	//life:byte
	PutByte(content_to_1, pClient1->mainTank.GetLife(), pos_1);

	/*
		FRIEND
	*/
	//id
	PutByte(content_to_1, pClient2->mainTank.get_id(), pos_1);

	//x:int
	PutInteger(content_to_1, pClient2->mainTank.get_pos().GetX(), pos_1);

	//y:int
	PutInteger(content_to_1, pClient2->mainTank.get_pos().GetY(), pos_1);

	//dir:int
	PutInteger(content_to_1, pClient2->mainTank.GetDir(), pos_1);

	//life:byte
	PutByte(content_to_1, pClient2->mainTank.GetLife(), pos_1);



	//��װ�û�2��ͷ��
	int pos_2 = 2;
	PutWord(content_to_2, (WORD)dPACKET_ALL_POS, pos_2);

	/*
	MYSELF
	*/
	//id
	PutByte(content_to_2, pClient2->mainTank.get_id(), pos_2);

	//x:int
	PutInteger(content_to_2, pClient2->mainTank.get_pos().GetX(), pos_2);

	//y:int
	PutInteger(content_to_2, pClient2->mainTank.get_pos().GetY(), pos_2);

	//dir:int
	PutInteger(content_to_2, pClient2->mainTank.GetDir(), pos_2);

	//life:byte
	PutByte(content_to_2, pClient2->mainTank.GetLife(), pos_2);

	/*
		FRIEND
	*/
	//id
	PutByte(content_to_2, pClient1->mainTank.get_id(), pos_2);

	//x:int
	PutInteger(content_to_2, pClient1->mainTank.get_pos().GetX(), pos_2);

	//y:int
	PutInteger(content_to_2, pClient1->mainTank.get_pos().GetY(), pos_2);

	//dir:int
	PutInteger(content_to_2, pClient1->mainTank.GetDir(), pos_2);

	//life:byte
	PutByte(content_to_2, pClient1->mainTank.GetLife(), pos_2);


	BYTE num;
	BYTE id;
	int x;
	int y;
	int dir;


	// bullet
	PutByte(content_to_1, lstBullets.size() + lstMainTankBullets.size(), pos_1);
	PutByte(content_to_2, lstBullets.size() + lstMainTankBullets.size(), pos_2);

	//enemy bullet
	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
	{
		//id:BYTE
		PutByte(content_to_1, (*it)->get_id(),pos_1);
		PutByte(content_to_2, (*it)->get_id(), pos_2);


		//x:INT
		PutInteger(content_to_1, (*it)->get_pos().GetX(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetX(), pos_2);

		//y:INT
		PutInteger(content_to_1, (*it)->get_pos().GetY(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetY(), pos_2);

		//dir:INT
		PutInteger(content_to_1, (*it)->GetDir(), pos_1);
		PutInteger(content_to_2, (*it)->GetDir(), pos_2);

		//type:BYTE
		PutByte(content_to_1, 1, pos_1);
		PutByte(content_to_2, 1, pos_2);
	}

	//our bullet
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end(); it++)
	{
		//id:BYTE
		PutByte(content_to_1, (*it)->get_id(), pos_1);
		PutByte(content_to_2, (*it)->get_id(), pos_2);


		//x:INT
		PutInteger(content_to_1, (*it)->get_pos().GetX(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetX(), pos_2);

		//y:INT
		PutInteger(content_to_1, (*it)->get_pos().GetY(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetY(), pos_2);

		//dir:INT
		PutInteger(content_to_1, (*it)->GetDir(), pos_1);
		PutInteger(content_to_2, (*it)->GetDir(), pos_2);

		//type:BYTE
		PutByte(content_to_1, 0, pos_1);
		PutByte(content_to_2, 0, pos_2);
	}

	// enemyTank{̹����:byte,loop{id:byte,x:int,y:int,dir:int}
	//num
	PutByte(content_to_1, lstTanks.size(), pos_1);
	PutByte(content_to_2, lstTanks.size(), pos_2);



	//enemyTank
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end(); it++)
	{
		//id:BYTE
		PutByte(content_to_1, (*it)->get_id(), pos_1);
		PutByte(content_to_2, (*it)->get_id(), pos_2);


		//x:INT
		PutInteger(content_to_1, (*it)->get_pos().GetX(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetX(), pos_2);

		//y:INT
		PutInteger(content_to_1, (*it)->get_pos().GetY(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetY(), pos_2);

		//dir:INT
		PutInteger(content_to_1, (*it)->GetDir(), pos_1);
		PutInteger(content_to_2, (*it)->GetDir(), pos_2);

	}
	
	//putsize
	PutSize(content_to_1, (WORD)pos_1);
	PutSize(content_to_2, (WORD)pos_2);


	if (pClient1 != NULL&& pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient1->m_scok);
	}
	else if(pClient1 != NULL){
		memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
		pClient1->m_sendSize += pos_1;

	}


	if (pClient2 != NULL && pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient2->m_scok);
	}
	else if(pClient2 != NULL)
	{
		memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
		pClient2->m_sendSize += pos_2;
	}
}


/*
����BOOM_ACT���ݰ�

//boom_act���ݰ��ṹ��size:word->tag:word->id:byte->id:byte

*/
void Game::onSend_BOOM_ACT(BYTE id_1,BYTE id_2) {
	int pos_1= 2;
	int pos_2 = 2;

	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];

	//tag:word
	PutWord(content_to_1, (WORD)dPACKET_BOOM_ACT, pos_1);
	PutWord(content_to_2, (WORD)dPACKET_BOOM_ACT, pos_2);


	//id1:byte
	PutByte(content_to_1, id_1, pos_1);
	PutByte(content_to_2, id_1, pos_2);

	//id2:byte
	PutByte(content_to_1, id_2, pos_1);
	PutByte(content_to_2, id_2, pos_2);

	//putsize
	PutSize(content_to_1, (WORD)pos_1);
	PutSize(content_to_2, (WORD)pos_2);

	if (pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient1->m_scok);
	}
	else {
		memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
		pClient1->m_sendSize += pos_1;

	}


	if (pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient2->m_scok);
	}
	else
	{
		memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
		pClient2->m_sendSize += pos_2;
	}

}

/*
���� FRIEND���ݰ�

FRIEND���ݰ��ṹ��size:word->tag:word->ip:string->nickName:string
�ֱ�Ϊclient1����client2�����ݰ���Ϊclient2����client1�����ݰ�
*/
void Game::onSend_FRIEND() {
	//client
	int pos_1 = 2;
	int pos_2 = 2;

	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];

	/*
		client1
	*/	
	//tag:word
	PutWord(content_to_1, dPACKET_FRIEND, pos_1);

	//ip:string
	PutString(content_to_1, pClient2->m_IP, pos_1);

	//nickName:string
	PutString(content_to_1, pClient2->m_Name, pos_1);

	//put size
	PutSize(content_to_1, (WORD)pos_1);

	/*
		client2
	*/
	//tag:word
	PutWord(content_to_2, dPACKET_FRIEND, pos_2);

	//ip:string
	PutString(content_to_2, pClient1->m_IP, pos_2);

	//nickName:string
	PutString(content_to_2, pClient1->m_Name, pos_2);

	//put size
	PutSize(content_to_2, (WORD)pos_2);

	/*
		cache
	*/

	if (pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient1->m_scok);
	}
	else {
		memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
		pClient1->m_sendSize += pos_1;

	}


	if (pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient2->m_scok);
	}
	else
	{
		memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
		pClient2->m_sendSize += pos_2;
	}

}


/*
��ʼ������������client����inGame+����һ��OnSendFriend��
*/
void Game::init() {
	pClient1->in_Game = 1;
	pClient2->in_Game = 1;

	//sending onSendFriend
	onSend_FRIEND();
}


void Game::Game_Logic() {
	/*
	�����ײ���������ײ����disappear
	*/
	CheckCrash();

	// New Game Level
	if (Setting::m_bNewLevel)
	{
		Setting::m_bNewLevel = false;

		Setting::NewGameLevel();


		//should add a new ���ݰ���������Ϸ����Ϣ
		//Graphic::ShowGameLevel(Setting::GetGameLevel());

		for (int i = 0; i < Setting::GetTankNum(); i++)
		{
			EnemyTank* p = new EnemyTank(width, height);
			lstTanks.push_back(p);
		}

		// ������ͣ����Enter��ʼ
		//skip = true;
		//continue;
		return;
	}

	/*
	�û�1��Ϸ����
	//�������ĸ��û�������Ϸ������������Ͽ����ӣ������������뱣����һ��������Ϸ�е��û�����Ϸ����

	//
	//*/
	//if (pClient1 == NULL||pClient1->mainTank.IsDisappear())
	//{
	//	//skip = true;
	//	//bGameOver = true;

	//	//Graphic::ShowGameOver();

	//	//continue;
	//}

	///*
	//�û�2��Ϸ����
	//omitting
	//*/

	//if (pClient2->mainTank.IsDisappear()) {

	//	continue;
	//}

	////���յ�������߼��������ս̹����ʧ�ˣ���ô����Ĵ���Ͳ�Ӧ��ִ����


	//move one step
	/*
		û����ʧ && ������������
	*/
	if (pClient1 != NULL && !pClient1->mainTank.IsDisappear()) {
		pClient1->mainTank.Move(start_x,end_x,start_y,end_y);
	}

	if (pClient2 != NULL && !pClient2->mainTank.IsDisappear()) {
		pClient2->mainTank.Move(start_x, end_x, start_y, end_y);
	}


	//mainTank.Display();

	/* Move Tanks and Send BOOM_ACT */
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end();)
	{
		(*it)->Move(start_x, end_x, start_y, end_y);

		if ((*it)->IsDisappear())
		{
			/*
				�����ڱ�����ʣ���̹�������ܹ����÷֡����ٵ�̹��������
				�������е�̹��������Ƿ���Ҫ�����µ�һ��
			*/
			Setting::TankDamaged();

			// Add a bomb
			//(*it)->Boom(lstBombs);

			// Delete the tank
			delete *it;
			it = lstTanks.erase(it);
			continue;
		}

		//(*it)->Display();

		//����о�̹������ӵ�
		if ((*it)->NeedShoot())
		{
			EnemyTank* p = (EnemyTank*)*it;
			p->Shoot(lstBullets);
		}
		it++;
	}

	/* Draw Bullets */
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end();)
	{
		(*it)->Move(start_x, end_x, start_y, end_y);

		if ((*it)->IsDisappear())
		{
			// Add a bomb
			//(*it)->Boom(lstBombs);

			// Delete the bullet
			delete *it;
			it = lstMainTankBullets.erase(it);
			continue;
		}

		//(*it)->Display();
		it++;
	}

	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end();)
	{
		(*it)->Move(start_x, end_x, start_y, end_y);

		if ((*it)->IsDisappear())
		{
			// Add a bomb
			//(*it)->Boom(lstBombs);

			// Delete the bullet
			delete *it;
			it = lstBullets.erase(it);
			continue;
		}

		//(*it)->Display();
		it++;
	}

	///* Draw Bombs */
	//for (list<Object*>::iterator it = lstBombs.begin(); it != lstBombs.end();)
	//{
	//	(*it)->Move();

	//	if ((*it)->IsDisappear())
	//	{
	//		delete *it;
	//		it = lstBombs.erase(it);
	//		continue;
	//	}

	//	(*it)->Display();
	//	it++;
	//}
}


void Game::Handling_Info() {
	fd_set read_set;
	fd_set write_set;
	fd_set exc_set;


	
	/*
		fd_set ��ʼ��
	*/

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_ZERO(&exc_set);

	/*
		binding
	*/
	if (pClient1 != NULL) {
		FD_SET(pClient1->m_scok, &read_set);
		FD_SET(pClient1->m_scok, &write_set);
		FD_SET(pClient1->m_scok, &exc_set);
	}

	if (pClient2 != NULL) {
		FD_SET(pClient2->m_scok, &read_set);
		FD_SET(pClient2->m_scok, &write_set);
		FD_SET(pClient2->m_scok, &exc_set);
	}


	/*
		CHECK
	*/
	if (pClient1 != NULL && (FD_ISSET(pClient1->m_scok, &exc_set)|| FD_ISSET(pClient2->m_scok, &exc_set))) {
		//OMITTING

		DisconnectClient(pClient1);
		//LIST_SKIP(client, next_client);
	}

	/*
	����1����û�з�Ӧ��
	OMITTING

	*/
	if (pClient1 != NULL && (pClient1->m_lastRecvTime + 60000 <= timeGetTime()|| pClient2->m_lastRecvTime + 60000 <= timeGetTime())) {
		DisconnectClient(pClient1);
		//LIST_SKIP(client, next_client);
	}

	/*
		try to recv
	*/


	//client1
	if (pClient1 != NULL && FD_ISSET(pClient1->m_scok, &read_set)) {

		if (!RecvFromClient(pClient1)) {
			DisconnectClient(pClient1);
			//LIST_SKIP(client, next_client);
		}
	}

	//client2
	if (pClient2 != NULL && FD_ISSET(pClient2->m_scok, &read_set)) {

		if (!RecvFromClient(pClient2)) {
			DisconnectClient(pClient2);
			//LIST_SKIP(client, next_client);
		}
	}

	/*
		handle all recv
	*/
	//client1
	if (pClient1 != NULL && pClient1->m_recvSize) {
		if (!ReadRecvBuff(pClient1,*this))
		{
			DisconnectClient(pClient1);
			//LIST_SKIP(client, next_client);
		}
	}

	//client2
	if (pClient2 != NULL && pClient2->m_recvSize) {
		if (!ReadRecvBuff(pClient2 ,*this))
		{
			DisconnectClient(pClient2);
			//LIST_SKIP(client, next_client);
		}
	}

	/*
		send
	*/
	//client1
	if (pClient1 != NULL && pClient1->m_sendSize && FD_ISSET(pClient1->m_scok, &write_set)) {
		//???��ʵ�����Ϊʲô�����isset����write_set����Ϊ�����������Щɶ���ݣ��ȵ���sendDataд�����������Զ���ģ���Ȼ��ŵ���flushSendBuff����ȥ
		if (FlushSendBuff(pClient1) < 0) {
			DisconnectClient(pClient1);
			//LIST_SKIP(client, next_client);
		}
	}
	//client2
	if (pClient2 != NULL && pClient2->m_sendSize && FD_ISSET(pClient2->m_scok, &write_set)) {
		//???��ʵ�����Ϊʲô�����isset����write_set����Ϊ�����������Щɶ���ݣ��ȵ���sendDataд�����������Զ���ģ���Ȼ��ŵ���flushSendBuff����ȥ
		if (FlushSendBuff(pClient2) < 0) {
			DisconnectClient(pClient2);
			//LIST_SKIP(client, next_client);
		}
	}
}


/*
��Ϸ���������߼�
while(!GameOver){
game logic(including move ; may sending BOOM_ACT);
sending all_pos;
call select and handling infomation;
}
*/

/*
	����һ��clientΪNULL��ʱ��1.�����쳣�Ͽ������� 2.һ����սʧ�ܶ������Ͽ������ӣ�����һ�����ܼ�����Ϸ
	����˫����ΪNULL��ʱ����Ϸ�������˳�
*/
void Game::runGame() {
	while (true) {
		if (pClient1 == NULL && pClient2 == NULL) {
			return;
		}
		
		//game logic
		Game_Logic();

		//sending all_pos
		onSend_ALL_POS();

		//handling_info
		Handling_Info();

		/*
			������ս̹�˶���ʧ��ʱ����Ϸ����
			�ͻ��ˣ���ʾ��Ϸ�����Ļ��棬�Ͽ��������֮������ӣ��ٴ�׼��֮��
		*/

		if (pClient1 != NULL && pClient2!=NULL && pClient1->mainTank.IsDisappear() && pClient2->mainTank.IsDisappear()) {
			pClient1->is_Ready = 0;
			pClient1->in_Game = 0;
			pClient1->is_Ready = 0;
			pClient1->in_Game = 0;

			/*
				�Ͽ������׽������ӣ������˴�������ɾ��������Ӧ�Ŀͻ��Ĳ���
			*/
			DisconnectClient(pClient1);
			DisconnectClient(pClient2);

			break;
		}
	}
}



void Game::CheckCrash()
{
	// Check enermy tank damage
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end(); it++)
	{
		for (list<Tank*>::iterator itt = lstTanks.begin(); itt != lstTanks.end(); itt++)
		{
			if (Shape::CheckIntersect((*it)->GetSphere(), (*itt)->GetSphere()))
			{
				
				(*itt)->SetDisappear();
				(*it)->SetDisappear();
				onSend_BOOM_ACT((*itt)->get_id(), (*it)->get_id());
			}
		}
	}

	// Check main tank damage
	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
	{
		if (Shape::CheckIntersect((*it)->GetSphere(), pClient1->mainTank.GetSphere()))
		{
			pClient1->mainTank.SetLife(pClient1->mainTank.GetLife() - 1);

			if (Setting::GetLife() > 0)
			{
				(*it)->SetDisappear();
				onSend_BOOM_ACT((*it)->get_id(), (*it)->get_id());
			}
			else
			{
				pClient1->mainTank.SetDisappear();
				onSend_BOOM_ACT(pClient1->mainTank.get_id(), pClient1->mainTank.get_id());
			}
		}

		if (Shape::CheckIntersect((*it)->GetSphere(), pClient2->mainTank.GetSphere()))
		{

			pClient2->mainTank.SetLife(pClient2->mainTank.GetLife() - 1);

			if (Setting::GetLife() > 0)
			{
				(*it)->SetDisappear();
				onSend_BOOM_ACT((*it)->get_id(), (*it)->get_id());
			}
			else
			{
				pClient2->mainTank.SetDisappear();
				onSend_BOOM_ACT(pClient2->mainTank.get_id(), pClient2->mainTank.get_id());

			}
		}
	}
}


void Game::Dispose()
{
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end(); it++)
	{
		delete *it;
	}
	lstTanks.clear();

	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end(); it++)
	{
		delete *it;
	}

	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
	{
		delete *it;
	}
	lstBullets.clear();

	for (list<Object*>::iterator it = lstBombs.begin(); it != lstBombs.end(); it++)
	{
		delete *it;
	}
	lstBombs.clear();

	//Graphic::Destroy();
}
