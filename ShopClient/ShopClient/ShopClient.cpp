// ShopClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

typedef struct {
	unsigned int ID;
	char name[30];
	double price;
	char brief[30];
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
	sock_in.sin_port = htons(3590);

	
	//sock_in.sin_addr.S_un.S_addr = inet_addr("172.16.5.253");
	sock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in)) == SOCKET_ERROR) {
		cout << "Connect error" << endl;
		exit (-1);
	}
	//���շ��ͷ�����Ϣ
	char buffer[1024];
	int num;
	if ((num = recv(clientSocket, buffer, 1024, 0)) > 0)
	{
		buffer[num] = '\0';
		cout << "sever say:" << buffer << endl;

	}
	return clientSocket;
}

/*��ȡ��Ʒ�б�*/
void getGoodsList(SOCKET &clientSocket)
{
	char msg[50] = "##get##";
	//printf("%s\n", msg);
	//send the command of geting good lists
	send(clientSocket, msg, strlen(msg)+1, 0);

	//receive data
	char buf[1024];
	int num;
	GOODS g;
	if ((recv(clientSocket, buf, sizeof(int), 0)) > 0)
	{
		//sscanf(buf, "%d", &num);
		memcpy(&num, buf, sizeof(int));
		
		cout << "����" << num << "����Ʒ" << endl;
	}
	while (( recv(clientSocket, buf, sizeof(g), 0)) > 0 && --num)
	{
		//cout << "sever say:" << buf << endl;

		/*��������*/
		int len;
		
		/*memcpy(&len, buf, sizeof(unsigned int));
		memcpy(&g.ID, buf + sizeof(unsigned int), sizeof(unsigned int));
		unsigned int nameLen;
		memcpy(&nameLen, buf + 2 * sizeof(unsigned int), sizeof(unsigned int));
		memcpy(g.name, buf + 3 * sizeof(unsigned int), nameLen * sizeof(char));
		memcpy(&g.price, buf + 3 * sizeof(unsigned int) + strlen(g.name) * sizeof(char), sizeof(double));
		unsigned int briefLen;
		memcpy(&briefLen, buf + 3 * sizeof(unsigned int) + strlen(g.name) * sizeof(char) + sizeof(double), sizeof(unsigned int));
		memcpy(g.brief, buf + 3 * sizeof(unsigned int) + strlen(g.name) * sizeof(char) + sizeof(double) + sizeof(unsigned int), briefLen * sizeof(char));
*/
		/*memcpy(&g.ID, buf, sizeof(unsigned int));
		memcpy(g.name, buf + sizeof(unsigned int), 30 * sizeof(char));
		memcpy(&g.price, buf + sizeof(unsigned int) + 30 * sizeof(char), sizeof(unsigned int));
		memcpy(g.brief, buf + sizeof(unsigned int) + 30 * sizeof(char) + sizeof(unsigned int), 30 * sizeof(char));*/
		

		memcpy(&g, buf, sizeof(g));
		//cout << g.ID << '\t' << g.name << '\t' << g.price << '\t' << g.brief << endl;
		printf("%d\t%s\t%.2f\t%s\n", g.ID, g.name, g.price, g.brief);
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
	//cout << "�ַ�������Ϊ:" << sizeof(msg) << endl;


	//��������
	char revdata[100];
	int num = recv(clientSocket, revdata, sizeof(double), 0);
	if (num > 0) {
		revdata[num] = '\0';
		double d;
		memcpy(&d, revdata, sizeof(double));
		cout<<"���Ϊ" << d << endl;
	}
}
/*disconnect the socket*/
void disconnect(SOCKET &clientSocket)
{
	char msg[50] = "##@";
	//�����˳���Ϣ
	send(clientSocket, msg, strlen(msg), 0);
	closesocket(clientSocket);
	WSACleanup();
}
/*show the menu what you will do*/
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

void receive(SOCKET clientSocket)
{
	char msg[100];//�洢���͵���Ϣ
	int num;
	GOODS g;
	while ((num = recv(clientSocket, msg, sizeof(msg), 0))>0)
	{
		msg[num] = '\0';
		cout << "Server say: " << msg << endl;
		memcpy(&g, msg, sizeof(msg));
		cout << "ID=" << g.ID << endl;
		cout << "name=" << g.name << endl;
		cout << "brief" << g.brief << endl;
		
	}
}
int main()
{
	SOCKET clientSocket;
	//��������
	connect(clientSocket);
	
	//�������
	showMenu(clientSocket);

	//����
	//receive(clientSocket);

	//�ر�����
	disconnect(clientSocket);
    return 0;
}

