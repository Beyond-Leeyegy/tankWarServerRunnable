#pragma once
/*
	FILE: def.h

	Ϊ��ƽ̨��ͷ�ļ��ۺ�

*/

#ifndef __DEF_H__
#define __DEF_H__

#define dSERVER_PORT  7000
#define dMAX_LISTEN	  200

/*
	��ʹ��selectʱ��Ϊ����������FD_SETSIZE���ƣ������е�define��
		����linux����FreeBSDʱ��Ĭ��ֵ��1024��
			��win32��Ĭ��ֵ��64
*/
#define FD_SETSIZE 6000


/*
	��ƽ̨ for win32
*/
#if defined(WIN32)
#include <windows.h>
#include <winsock.h>
#include <mmsystem.h>
#include <stdio.h>
#include <time.h>

//#include "resource.h" //?????????????????


#else


/*
	��ƽ̨�� for FreeBSD / Linux ��ͷ�ļ�
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

/*
	��UNIXϵ�в���ϵͳ����ʹ��close�����ر��׽���
*/
#define closesocket(sock) close(sock)

#define INVALID_SOCKET (-1)
#define TRUE 1
#define FALSE 0

typedef int SOCKET;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef int COLORREF;



#endif // end of if defined (Win32)

#endif // !__DEF_H__
