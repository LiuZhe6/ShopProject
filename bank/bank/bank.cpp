// bank.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "account.h"
#include "array.h"
#include <iostream>
#include<WinSock2.h>
#include <cstdio>
#include<string>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main() {
	//��ʼ��WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(20000);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	printf("�ȴ�����...\n");
	Date date(2008, 11, 1);
	SavingsAccount sa2(date, "ɵ�����˻�", 0.015);
	sa2.deposit(date,20001,"aaaa");              //��100000��
	/*cout<<sa2.getTotal()<<endl;
	sa2.deposit(date, 100000, "aaaa");
	cout << sa2.getTotal() << endl;
	bool flag=sa2.pay(100000870);
	if (flag)
		cout << sa2.getTotal() << endl;
	else
		cout << "����" << endl;*/
	while (true)
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("connect success\n");
		//printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������  
		int ret = recv(sClient, revData, sizeof(double), 0);
		if (ret > 0)
		{
			
			//printf(revData);
			//revData[8] = '\0';
			double amount;
			memcpy(&amount, revData, sizeof(double));
			printf("%lf\n", amount);
			double total = sa2.getTotal();
			char flag='0';
			double balance = total;
			balance = total - amount;
			if (total >= amount)
			{
				sa2.pay(amount);
				flag = '1';
			}
			//��������  
			printf("balance=%lf\n", balance);
			char *sendData = (char*)malloc(sizeof(double));
			memcpy(sendData, &balance, sizeof(double));
			//sprintf(sendData,"%lf",balance);
			send(sClient, sendData, sizeof(double), 0);
		}
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
















	//Date date(2008, 11, 1);
	////���������˻�
	//// SavingsAccount sa1(date,"�������˻�",0.015);
	//// SavingsAccount sa2(date,"ɵ�����˻�",0.015);
	//// CreditAccount ca(date,"ɵ�����˻�",10000,0.0005,50);
	//Array<Account*> accounts(0);
	////const int n=sizeof(accounts)/sizeof(Account *); //�˻�����
	//cout << "(a)add account (d) deposit (w) withdraw (s) show (c) change day (n) next month (e) exit" << endl;
	//char cmd;
	//do {
	//	date.show();
	//	cout << "\tTotal: " << Account::getTotal() << "\tcommand>";
	//	char type;
	//	int index, day;
	//	double amount, credit, rate, fee;
	//	Account *account;
	//	string id, desc;
	//	cin >> cmd;
	//	switch (cmd) {
	//	case 'a':
	//		cin >> type >> id;
	//		if (type == 's') {
	//			cin >> rate;
	//			account = new SavingsAccount(date, id, rate);
	//		}
	//		else {
	//			cin >> credit >> rate >> fee;
	//			account = new CreditAccount(date, id, credit, rate, fee);
	//		}
	//		accounts.resize(accounts.getSize() + 1);
	//		accounts[accounts.getSize() - 1] = account;
	//		break;
	//	case 'd':
	//		cin >> index >> amount;
	//		getline(cin, desc);
	//		accounts[index]->deposit(date, amount, desc);
	//		break;
	//	case 'w':
	//		cin >> index >> amount;
	//		getline(cin, desc);
	//		accounts[index]->withdraw(date, amount, desc);
	//		break;
	//	case 's':
	//		for (int i = 0; i<accounts.getSize(); i++) {
	//			cout << "[" << i << "]";
	//			accounts[i]->show();
	//			cout << endl;
	//		}
	//		break;
	//	case 'c':
	//		cin >> day;
	//		if (day<date.getDay())
	//			cout << "You cannot specify a previous day";
	//		else if (day>date.getMaxDay())
	//			cout << "Invalid day";
	//		else
	//			date = Date(date.getYear(), date.getMonth(), day);
	//		break;
	//	case 'n':
	//		if (date.getMonth() == 12)
	//			date = Date(date.getYear() + 1, 1, 1);
	//		else
	//			date = Date(date.getYear(), date.getMonth() + 1, 1);
	//		for (int i = 0; i<accounts.getSize(); i++)
	//			accounts[i]->settle(date);
	//		break;
	//	}
	//} while (cmd != 'e');

	//// //11�·���Ŀ
	//// accounts[0].deposit(Date(2008,11,5),5000,"salary");
	//// accounts[1].deposit(Date(2008,11,25),10000,"sell stock 0323");
	//// ca.withdraw(Date(2008,11,15),2000,"buy a cell");
	//// //�������ÿ�
	//// ca.settle(Date(2008,12,1));
	//// //12�·���Ŀ
	//// accounts[0].deposit(Date(2008,12,5),5500,"salary");
	//// ca.deposit(Date(2008,12,1),2016,"pay the credit");
	//// //���㲢���
	//// cout<<endl;
	//// for(int i=0;i<n;i++){
	//// 	accounts[i].settle(Date(2009,1,1));
	//// 	accounts[i].show();
	//// 	cout<<endl;
	//// }
	//// ca.settle(Date(2009,1,1));
	//// ca.show(); cout<<endl;
	//for (int i = 0; i<accounts.getSize(); i++)
	//	delete accounts[i];
	return 0;
}

