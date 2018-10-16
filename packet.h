#pragma once
/*
	FILE:packet.h
	
	���ݰ���������ͷ�ļ�

*/


/*
	packetһ��ṹ���䳤����
	packet_size(WORD)-packet_tag(WORD)-data(�䳤)

	���ڴ����data�Ǵ�����������ݣ���ôdata���Խ�һ���з�
	packet_size(WORD)-packet_tag(WORD)-str_length(WORD)-str(�䳤)

*/


#ifndef  __PACKET_H__
#define  __PACKET_H__

#include "def.h"
/*
	��pPacket������nPos����BYTE���͵�value��ֵ
*/
void  PutByte(char *pPacket, BYTE value, int & nPos);

/*
	��pPacket����nPosλ�õ�ֵ��BYTE���ͷ���
*/

BYTE  GetByte(char* pPacket, int& nPos);


/*
��pPacket������nPos����WORD���͵�value��ֵ
*/
void PutWord(char *pPacket, WORD value, int & nPos);

/*
��pPacket����nPosλ�õ�ֵ��WORD���ͷ���
*/

WORD GetWord(char* pPacket, int& nPos);

/*
��pPacket������nPos����DWORD���͵�value��ֵ
*/
void PutDword(char *pPacket, DWORD value, int & nPos);

/*
��pPacket����nPosλ�õ�ֵ��DWORD���ͷ���
*/

DWORD GetDword(char* pPacket, int& nPos);

/*
��pPacket������nPos����int���͵�value��ֵ
*/
void  PutInteger(char *pPacket, int value, int & nPos);

/*
��pPacket����nPosλ�õ�ֵ��int���ͷ���
*/

int  GetInteger(char* pPacket, int& nPos);

/*
��pPacket������nPos����short���͵�value��ֵ
*/
void PutShort(char *pPacket, short value, int & nPos);

/*
��pPacket����nPosλ�õ�ֵ��short���ͷ���
*/

short GetShort(char* pPacket, int &nPos);

/*
��pPacket������nPos�����ַ���
*/
void PutString(char *pPacket, char *str , int & nPos);

/*
��pPacket����nPosλ�õ��ַ������Ƶ�pBuffer������NULL�Ĵ���
*/

void GetString(char* pPacket, char *pBuffer, int& nPos);

/*
	�������ݰ����ܳ���
*/
void PutSize(char *pPacket, WORD nPos);

#endif // ! __PACKET_H__

