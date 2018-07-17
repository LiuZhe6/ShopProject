// ShopClient.cpp : 定义控制台应用程序的入口点。
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
	//接收发送返回消息
	char buffer[1024];
	int num;
	if ((num = recv(clientSocket, buffer, 1024, 0)) > 0)
	{
		buffer[num] = '\0';
		cout << "sever say:" << buffer << endl;

	}
	return clientSocket;
}

/*获取商品列表*/
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
		
		cout << "共有" << num << "个商品" << endl;
	}
	while (( recv(clientSocket, buf, sizeof(g), 0)) > 0 && --num)
	{
		//cout << "sever say:" << buf << endl;

		/*解析数据*/
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
	//cout << "字符串长度为:" << sizeof(msg) << endl;


	//接收数据
	char revdata[100];
	int num = recv(clientSocket, revdata, sizeof(double), 0);
	if (num > 0) {
		revdata[num] = '\0';
		double d;
		memcpy(&d, revdata, sizeof(double));
		cout<<"余额为" << d << endl;
	}
}
/*disconnect the socket*/
void disconnect(SOCKET &clientSocket)
{
	char msg[50] = "##@";
	//发送退出消息
	send(clientSocket, msg, strlen(msg), 0);
	closesocket(clientSocket);
	WSACleanup();
}
/*show the menu what you will do*/
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

void receive(SOCKET clientSocket)
{
	char msg[100];//存储传送的消息
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
	//建立连接
	connect(clientSocket);
	
	//具体操作
	showMenu(clientSocket);

	//接收
	//receive(clientSocket);

	//关闭连接
	disconnect(clientSocket);
    return 0;
}

