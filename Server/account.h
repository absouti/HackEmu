#pragma once
#include "server.h"

struct AccuAttr {
	std::string KeyName; // 键名
	void* KeyPtr; // 键值指针
};

class Account {
	std::string username;
	GUID guid;
	std::vector<AccuAttr> attribe;
public:
	Account(std::string name); // 用户名
	~Account();
	void* ReadAttribe(std::string KeyName);
	bool WriteAttribe(std::string KeyName, void* KeyPtr);
	bool AddAttribe(std::string KeyName, void* KeyPtr);
	friend int getAccountIndex(std::vector<Account>& __map, GUID guid);
	friend GUID getAccountGUID(std::vector<Account>& __map, int index);
};

int account_init(std::vector<Account>& __Val);
int getAccountIndex(std::vector<Account>& __map, GUID guid);
GUID getAccountGUID(std::vector<Account>& __map, int index);
OLECHAR* GUID2String(GUID guid);