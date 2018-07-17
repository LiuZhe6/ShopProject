#pragma once
//account.h
#ifndef _ACCOUNT_H__
#define _ACCOUNT_H__

#include "date.h"
#include "accumulator.h"
#include <string>

class Account {
private:
	std::string id;
	double balance;
	static double total;
protected:
	Account(const Date &date, const std::string &id);
	void record(const Date &date, double amount, const std::string &desc);
	void error(const std::string &msg)const;
public:
	const std::string &getId() const { return id; }
	double getBalance() const { return balance; }
	virtual void withdraw(const Date &date, double amount, const std::string &desc) = 0;
	static double getTotal() { return total; }
	virtual void deposit(const Date &date, double amount, const std::string &desc) = 0;
	virtual void settle(const Date&date) = 0;
	virtual void show() const;
	virtual bool pay(double amount) = 0;
	void sub(double amount);
};


class SavingsAccount : public Account {
private:
	Accumulator acc;
	double rate;
public:
	SavingsAccount(const Date&date, const std::string &id, double rate);
	double getRate() const { return rate; }
	//����
	void deposit(const Date &date, double amount, const std::string &desc);
	//ȡ��
	void withdraw(const Date &date, double amount, const std::string &desc);
	void settle(const Date &date); //���������ʣ�ÿ��1.1����
	bool pay(double amount);
};

class CreditAccount : public Account {
private:
	Accumulator acc;  //��Ϣ����
	double credit;  //���ö��
	double rate;   //Ƿ��������
	double fee;		//���ÿ����
	double getDebt() const {
		double balance = getBalance();
		return (balance < 0 ? balance : 0);
	}
public:
	CreditAccount(const Date&date, const std::string &id, double credit, double rate, double fee);
	double getCredit() const { return credit; }
	double getRate() const { return rate; }
	double getFee() const { return fee; }
	double getAvailableCredit() const {
		if (getBalance()<0)
			return credit + getBalance();
		else return credit;
	}
	//����
	void deposit(const Date &date, double amount, const std::string &desc);
	//ȡ��
	void withdraw(const Date &date, double amount, const std::string &desc);
	void settle(const Date &date);
	void show() const;
};
#endif