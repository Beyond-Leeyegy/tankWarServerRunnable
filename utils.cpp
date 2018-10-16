/*
	FILE:utils.cpp
	
	����ͨ�õĺ���
*/

#include "def.h"
#include <stdarg.h>

/*
	����va_list & sprintf
*/
void log(char*buf, ...) {
	va_list args;
	char tmp_main[1024];
	char tmp_time[1024];

	FILE * fp = fopen("log.txt", "a");//a����׷��д��ֻд�������ھʹ������ļ�������ֻд��ģʽ���ļ������ھͻᴴ���������硰w����������д��

	//step1. get multiple arguments
	va_start(args, buf);//��һ��������va_list���ͣ��ڶ��������Ǻ�������Ի��Ĳ��������ڱ������о���buf
	vsprintf(tmp_main, buf, args);
	va_end(args);	    //start��end����ʹ�þ��õ�������ʽ�Ĳ���

	//step2. get now time��for linux��
	//��������
		//1.����time���������ش�UTCʱ�䡪������ͳһʱ�䣨1970��1��1��0ʱ0��0�룩������Ϊֹ��UTCʱ���ȥ����������time_t���� 
		//2.����localtime������time�ķ���ֵת���ɵ�ǰϵͳ����ʱ����������+8ʱ����Ӧ��ʱ�䣬����ֵ��tm�ṹ��ָ�����ͣ���Ա���ꡢ�¡��ա�ʱ���֡���
	struct tm *current_time;
	time_t time_secs;

	time(&time_secs);
	current_time = localtime(&time_secs);

	if (!fp)
		return;

	// file open successfully
	// step3. complete the tmp_main

	//������Ϊ���Ǵ�1900�꿪ʼ���أ�����������Ӧ����1970�𣿣���
	sprintf(tmp_time, "[%d/%02d/%02d %02d:%02d:%02d]:%s", current_time->tm_year + 1900,
		current_time->tm_mon,
		current_time->tm_mday,
		current_time->tm_hour,
		current_time->tm_min,
		current_time->tm_sec,
		tmp_main);
	strcpy(tmp_main, tmp_time);
	fprintf(fp, tmp_main);
	
#ifndef  WIN32
	printf(tmp_main);
#endif // ! WIN32

	
	fclose(fp);

}

/*
	LINUX/FREEBSD:û���Ժ���Ϊ��λ��ȡCPUʱ���
	win32��timeGetTime��������ʵ��
	�����linux�������Լ�ʵ��һ��timeGetTime

*/
#ifndef WIN32
DWORD timeGetTime() {
	struct timeval tv;//tv_sec tv_usec
	gettimeofday(&tv, NULL);
	DWORD value = (((tv.tv_sec - 1000000000) * 1000) + (tv.tv_usec / 1000));
	return value;

}
#endif // !WIN32
