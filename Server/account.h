#pragma once
#include "base.h"

#ifdef _DEBUG
#define DEBUGCODE debug
#else
#define DEBUGCODE __noop
#endif _DEBUG

struct AccuAttr {
	std::string KeyName; // ����
	void* KeyPtr; // ��ֵָ��
};

class Account {
	std::string username;
	UUID uuid;
	std::vector<AccuAttr> attribe;
public:
	Account(std::string name); // �û���
	~Account();
	void* ReadAttribe(std::string KeyName);
	bool WriteAttribe(std::string KeyName, void* KeyPtr);
	bool AddAttribe(std::string KeyName, void* KeyPtr);
	friend int getAccountIndex(std::vector<Account>& __map, UUID uuid);
	friend UUID getAccountUUID(std::vector<Account>& __map, int index);
};

int account_init(std::vector<Account>& __Val);
int getAccountIndex(std::vector<Account>& __map, UUID uuid);
UUID getAccountGUID(std::vector<Account>& __map, int index);
OLECHAR* GUID2String(UUID guid);
void debug();