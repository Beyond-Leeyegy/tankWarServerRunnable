#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include "structs.h"

class Game
{
public:
	/*
		������Ϸ�߽�
	*/
	int width;
	int height;
	int start_x;
	int start_y;
	int end_x;
	int end_y;

	Game();
	Game(sPCLIENT_DATA Client1, sPCLIENT_DATA Client2);
	~Game();

	//����������Ϸ���û�
	sPCLIENT_DATA pClient1;
	sPCLIENT_DATA pClient2;

	//�ҷ��ӵ�����
	list<Object*> lstMainTankBullets;

	//����̹�˹���һ���ӵ�����
	list<Object*> lstBullets;

	// Bomb List
	list<Object*> lstBombs;

	// Tank list
	list<Tank*> lstTanks;

	/*
		����all_pos���ݰ�
		
		ALL_POS���ݰ��ṹ��size: word->tag :word->��ս̹��1����{id:byte,x:int,y:int.dir:int,̹������ֵ:byte}->�Ѿ���ս̹������{����ͬ��}->�ӵ�������{�ӵ�����:byte,loop{id:byte,x:int,y:int,dir:int,type:byte0�����Ҿ����ϣ�1����о�����}}->
		����̹������{̹����,loop{ͬ��}}
		
	*/
	void onSend_ALL_POS();

	/*
		����BOOM_ACT���ݰ�

		//boom_act���ݰ��ṹ��size:word->tag:word->id:byte->id:byte

	*/
	void onSend_BOOM_ACT(BYTE id_1, BYTE id_2);

	/*
		���� FRIEND���ݰ�

		FRIEND���ݰ��ṹ��size:word->tag:word->ip:string->nickName:string

	*/
	void onSend_FRIEND();

	/*
		��ʼ������������client����inGame+����һ��OnSendFriend��
	*/
	void init();

	void Game_Logic();

	void Handling_Info();


	/*
		��Ϸ���������߼�
		while(!GameOver){
			game logic(including move ; may sending BOOM_ACT);
			sending all_pos;
			call select and handling infomation;
		}
	*/
	void runGame();

	/*
		�����ײ
	*/
	void CheckCrash();

	/*
		�������е��б����
	*/
	void Dispose();



};


#endif
