#pragma once
/*
	FILE:utils.h

	����ͨ�ú���
*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include "def.h"

/*
	��־�Ļ���������ļ���¼����

*/
void log(char *buf, ...);

/*
	win32 timeGetTime �����ķ�Ч��emulation��
*/
#ifndef  WIN32
DWORD timeGetTime();
#endif // ! WIN32

#endif // !__UTILS_H__
