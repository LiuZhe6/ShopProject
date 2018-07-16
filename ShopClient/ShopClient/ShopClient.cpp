// ShopClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

typedef struct {
	unsigned int ID;
	char* name;
	double price;
	char* brief;
}GOODS;

SOCKET connect(SOCKET &clientSocket)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		exit;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Socket error" << endl;
		exit(-1);
	}
	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(8888);
	sock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in)) == SOCKET_ERROR) {
		cout << "Connect error" << endl;
		exit (-1);
	}
	return clientSocket;
}

/*��ȡ��Ʒ�б�*/
void getGoodsList(SOCKET &clientSocket)
{
	char msg[50] = "##get##";
	//����
	send(clientSocket, msg, strlen(msg), 0);

	//��������
	char revdata[100];
	int num = recv(clientSocket, revdata, 100, 0);
	if (num > 0) {
		revdata[num] = '\0';
		cout << "Sever say:" << revdata << endl;
	}
}

/*������Ʒ*/
void purchaseGoods(SOCKET &clientSocket)
{
	char id[5];
	cout << "��������Ҫ�������ƷID" << endl;
	cin >> id;
	char msg[50]="#";
	strcat(msg, id);
	strcat(msg, "##");
	send(clientSocket, msg, strlen(msg), 0);
	cout << "�ַ�������Ϊ:" << sizeof(msg) << endl;

	//���ս��
	char revdata[100];
	int num = recv(clientSocket, revdata, 100, 0);
	if (num > 0) {
		revdata[num] = '\0';
		cout << "Sever say:" << revdata << endl;
	}
}
void showMenu(SOCKET &clientSocket)
{
	while (true) {
		cout << "**********************" << endl;
		cout << "�����������" << endl;
		cout << "1.��ȡ��Ʒ�б�" << endl;
		cout << "2.������Ʒ" << endl;
		cout << "0.�˳�" << endl;
		cout << "**********************" << endl;
		cout << "���������:";
		int n;
		cin >> n;
		switch (n)
		{
		case 1:
			getGoodsList(clientSocket);
			break;
		case 2:
			purchaseGoods(clientSocket);
			break;
		default:
			break;
		}
		if (n == 0)
			return;
		
	}
	
}

void disconnect(SOCKET &clientSocket)
{
	closesocket(clientSocket);
	WSACleanup();
}
int main()
{
	SOCKET clientSocket;
	//��������
	connect(clientSocket);

	//�������
	showMenu(clientSocket);

	//�ر�����
	disconnect(clientSocket);
    return 0;
}

