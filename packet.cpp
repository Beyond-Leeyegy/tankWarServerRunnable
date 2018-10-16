/*
	FILE :packet.cpp
	���ݰ�������

*/

#include "def.h"
#include "packet.h"
#include <cstring>

/*
��pPacket������nPos����BYTE���͵�value��ֵ
*/
void  PutByte(char *pPacket, BYTE value, int & nPos) {
	
	*(BYTE *)(pPacket + nPos) = value;
	nPos += sizeof(BYTE);
}

/*
��pPacket����nPosλ�õ�ֵ��BYTE���ͷ���
*/

BYTE  GetByte(char* pPacket, int& nPos) {
	BYTE value = *(BYTE *)(pPacket + nPos);
	nPos += sizeof(BYTE);
	return value;
}


/*
��pPacket������nPos����WORD���͵�value��ֵ
*/
void PutWord(char *pPacket, WORD value, int & nPos) {
	*(WORD *)(pPacket + nPos) = value;
	nPos += sizeof(WORD);
}

/*
��pPacket����nPosλ�õ�ֵ��WORD���ͷ���
*/

WORD GetWord(char* pPacket, int& nPos) {
	WORD value = *(WORD *)(pPacket + nPos);
	nPos += sizeof(WORD);
	return value;
}

/*
��pPacket������nPos����DWORD���͵�value��ֵ
*/
void PutDword(char *pPacket, DWORD value, int & nPos) {
	*(DWORD *)(pPacket + nPos) = value;
	nPos += sizeof(DWORD);
}

/*
��pPacket����nPosλ�õ�ֵ��DWORD���ͷ���
*/

DWORD GetDword(char* pPacket, int &nPos) {
	DWORD value = *(DWORD *)(pPacket + nPos);
	nPos += sizeof(DWORD);
	return value;
}

/*
��pPacket������nPos����int���͵�value��ֵ
*/
void  PutInteger(char *pPacket, int value, int & nPos) {
	*(int *)(pPacket + nPos) = value;
	nPos += sizeof(int);
}

/*
��pPacket����nPosλ�õ�ֵ��int���ͷ���
*/

int  GetInteger(char* pPacket, int &nPos) {
	int value = *(int*)(pPacket + nPos);
	nPos += sizeof(int );
	return value;
}

/*
��pPacket������nPos����short���͵�value��ֵ
*/
void PutShort(char *pPacket, short value, int & nPos) {
	*(short *)(pPacket + nPos) = value;
	nPos += sizeof(short);
}

/*
��pPacket����nPosλ�õ�ֵ��short���ͷ���
*/

short GetShort(char* pPacket, int& nPos) {
	short value = *(short*)(pPacket + nPos);
	nPos += sizeof(short);
	return value;
}

/*
��pPacket������nPos�����ַ���
*/
void PutString(char *pPacket, char *str, int & nPos) {
	*(WORD*)(pPacket + nPos) = strlen(str);
	nPos += sizeof(WORD);
	memcpy(pPacket + nPos, str, strlen(str));
	nPos += strlen(str);
}

/*
��pPacket����nPosλ�õ��ַ������Ƶ�pBuffer������NULL�Ĵ���
*/

void GetString(char* pPacket, char *pBuffer, int &nPos) {
	WORD length = *(WORD*)(pPacket + nPos);
	nPos += sizeof(WORD);
	memcpy(pBuffer, pPacket + nPos, length);
	*(pBuffer + length) = '\0';
	nPos += length;
}

/*
�������ݰ����ܳ���:���������ݰ���ʼ����
*/
void PutSize(char *pPacket, WORD nPos) {
	*(WORD *)pPacket = nPos;
}
