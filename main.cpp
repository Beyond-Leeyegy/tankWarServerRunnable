/*
	FILE: main.cpp

	Main
*/

#include "def.h"
#include "structs.h"
#include "linked_list.h"
#include "network.h"
#include "utils.h"
#include "message.h"
#include "structs.h"
#include  "game.h"
#include <pthread.h>



struct arg {
	sPCLIENT_DATA client1;
	sPCLIENT_DATA client2;
};
 
typedef struct arg * arg_thread;

/*
	ȫ�ֱ���
*/
int g_ProcessLife = TRUE;

/*
	�������Ļ����׽���
*/
SOCKET g_SOCK = INVALID_SOCKET;

/*
	�ͻ���������
*/
sPCLIENT_DATA g_ClientList = NULL;

/*
	����������
*/
int g_TotalClient = 0;

/*
	ȫ�ֺ���
*/
void ProcessLoop();


void* start_game_thread(void * arg) {
	arg_thread args = (arg_thread)arg;
	Game game(args->client1, args->client2);
	game.init();
	game.runGame();
}


/*
	Win32��Main����
*/

#ifdef WIN32
#else
	/*
		LINUX
	*/
	
/*
	���������������������Signalʱ����øú���
*/
void DestroySignal(int sigNum) {
//	log("sth bad happen");
//	printf("you sees see");
	g_ProcessLife = FALSE;
}

/*
	Signal�����趨
*/
void InitSignal() {

	/*
		����SIGPIPE
		ԭ��SIGPIPE��ǿ�ƹرս��̶��������κ���ʾ����������Ա�޴�ȷ����Ϣ��Ϊ�˱��������������Ҫ��������
		����SIGPIPE�ĳ��������糢�����Ѿ��رյ��׽�����д����
	*/
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags &= ~SA_RESETHAND;
	sigaction(SIGPIPE, &act, NULL);

	/*
		������ǿ�ƽ������������е�Signalʱ�򣬵��������������������к�����Signal������
	*/
	signal(SIGINT,DestroySignal);
	signal(SIGKILL,DestroySignal);
	signal(SIGQUIT, DestroySignal);
	signal(SIGTERM, DestroySignal);  //�����ƺ���linux���ڵĶ��岻����ͬ��
}

/*
	LINUX /FressBSD�� Main����
*/
int 
main() {
	InitSignal();
	log("IAM HERE\r\n");
	ProcessLoop();
	return TRUE;
}

#endif

/*
	�������������ݳ�ʼ������
*/
void InitServerData() {

}


/*
	��������������ɾ������
*/

void DestroyServerData() {
	
}



/*
	WIN32��LINUX��ͬ����ѭ��
*/

void ProcessLoop() {
#ifdef WIN32
	//omitting
#endif

	/*
		�������׽��ֳ�ʼ��
	*/
	g_SOCK = InitServerSock(dSERVER_PORT, dMAX_LISTEN);

	if (g_SOCK == INVALID_SOCKET)
	{
		log("INVALID SOCKET\r\n");
		printf("INVALID SOCKET\n\n\n\n");
		return;
	}
	
	log("VALID DATA\r\n");


	/*
	
		���������ݳ�ʼ��
	*/

	InitServerData();

	fd_set read_set;
	fd_set write_set;
	fd_set exc_set;
	struct timeval tv;
	SOCKET nfds;



	/*
		����δ��⵽�źž�������ת����һ���׽��ֵ��źŵļ��
	*/
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	sPCLIENT_DATA client, next_client;

	while ((g_ProcessLife))
	{
#ifdef WIN32
		//OMITTING
#endif

			/*
				fd_set��ʼ��
			*/
			FD_ZERO(&read_set);
			FD_ZERO(&write_set);
			FD_ZERO(&exc_set);

			/*
				�����׽��ּӵ�read_set�������µ���������
			*/
			FD_SET(g_SOCK,&read_set);
			nfds = g_SOCK;


			/*
				���׽��ֵ����ֵ
			*/
			LIST_WHILE(g_ClientList,client,next_client,m_next);
			if (client->m_scok > nfds)
				nfds = client->m_scok;

			/*
				��Ҫ�������׽�����fd_Set֮��İ�
				ֻ����������Ϸ״̬�е��û�
			*/
			if (!client->in_Game) {
				FD_SET(client->m_scok, &read_set);
				FD_SET(client->m_scok, &write_set);
				FD_SET(client->m_scok, &exc_set);
			}

			LIST_WHILEEND(g_ClientList, client, next_client);
			
			/*
				call select
			*/
				if (select(nfds + 1, &read_set, &write_set, &exc_set, &tv) < -1) {
					log("select Error!\r\n");
					continue;
			}
			/*
				����������Ĵ���
			*/
			if (FD_ISSET(g_SOCK, &read_set))
				AcceptNewClient(g_SOCK);

			/*
				��������������recv�����գ�
			*/
			LIST_WHILE(g_ClientList, client, next_client, m_next);

			/*
				ֻ��������Ϸ״̬���û�
			*/
			if (client->in_Game)
				continue;
			/*
				error handling
			*/
			if (FD_ISSET(client->m_scok, &exc_set)) {
				DisconnectClient(client);
				LIST_SKIP(client, next_client);
			}

			/*
				����1����û�з�Ӧ���Ͽ�����

			*/
			if (client->m_lastRecvTime + 60000 <= timeGetTime()) {
				DisconnectClient(client);
				LIST_SKIP(client, next_client);
			}

			/*
				���Խ������ݣ��ͻ���-����������
				������׽�����ͼ��������������ݣ���ô�ͳ��Խ�����Щ����,�洢����������
			*/
			if (FD_ISSET(client->m_scok, &read_set)) {

				if (!RecvFromClient(client)) {
					DisconnectClient(client);
					LIST_SKIP(client, next_client);
				}
			}

			/*
				����������ĳ�ͻ����͹��������������
				����ͻ�-���������������������ݣ���ô�ʹ�����Щ����
			*/

			/*
			
				@readrecvBuff:message.h 
				here just handle is_ready
			*/
			Game tmp;
			if (client->m_recvSize) { 
				if (!ReadRecvBuff(client,tmp))
				{
					DisconnectClient(client);
					LIST_SKIP(client, next_client);
				}


			}

			LIST_WHILEEND(g_ClientList, client, next_client);

			/*
				���Է������ݣ�������-���ͻ��ˣ�
				����������׽�����ͼ��ͻ���д���ݵ�ʱ���� ������-���ͻ��������������ݣ���ô�ͳ��Է�����Щ����
			*/
			LIST_WHILE(g_ClientList, client, next_client, m_next);
			/*
			ֻ��������Ϸ״̬���û�
			*/
			if (client->in_Game)
				continue;

			if (client->m_sendSize && FD_ISSET(client->m_scok, &write_set)) {
				//???��ʵ�����Ϊʲô�����isset����write_set����Ϊ�����������Щɶ���ݣ��ȵ���sendDataд�����������Զ���ģ���Ȼ��ŵ���flushSendBuff����ȥ
				if (FlushSendBuff(client) < 0) {
					DisconnectClient(client);
					LIST_SKIP(client, next_client);
				}
			}

			LIST_WHILEEND(g_ClientList, client, next_client);


			/*
				!in_game && is_ready
			*/
			int count = 0;
			sPCLIENT_DATA tmp1, tmp2;

			LIST_WHILE(g_ClientList, client, next_client, m_next);
			if (!client->in_Game && client->is_Ready) {
				count = (count + 1) % 2;
				if (count == 0) {
					tmp1 = client;
				}
				else if (count == 1) {
					tmp2 = client;
					arg_thread arg;
					arg->client1 = tmp1;
					arg->client2 = tmp2;

					pthread_t t1;
					pthread_create(&t1, NULL, start_game_thread, (void*)arg);
					//start_game_thread;
				}
			}
			LIST_WHILEEND(g_ClientList, client, next_client);

}

	/*
		���������ݵ�ɾ��
	
	*/
	DestroyServerData();

#ifdef WIN32
//omitting data here

#endif




}
