// ShopClient.cpp : 定义控制台应用程序的入口点。
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

/*获取商品列表*/
void getGoodsList(SOCKET &clientSocket)
{
	char msg[50] = "##get##";
	//发送
	send(clientSocket, msg, strlen(msg), 0);

	//接收数据
	char revdata[100];
	int num = recv(clientSocket, revdata, 100, 0);
	if (num > 0) {
		revdata[num] = '\0';
		cout << "Sever say:" << revdata << endl;
	}
}

/*购买商品*/
void purchaseGoods(SOCKET &clientSocket)
{
	char id[5];
	cout << "请输入想要购买的商品ID" << endl;
	cin >> id;
	char msg[50]="#";
	strcat(msg, id);
	strcat(msg, "##");
	send(clientSocket, msg, strlen(msg), 0);
	cout << "字符串长度为:" << sizeof(msg) << endl;

	//接收结果
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
		cout << "输入下列序号" << endl;
		cout << "1.获取商品列表" << endl;
		cout << "2.购买商品" << endl;
		cout << "0.退出" << endl;
		cout << "**********************" << endl;
		cout << "请输入序号:";
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
	//建立连接
	connect(clientSocket);

	//具体操作
	showMenu(clientSocket);

	//关闭连接
	disconnect(clientSocket);
    return 0;
}

