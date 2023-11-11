#include "account.h"

/*
    Account::Account(std::string name)
    name -> 用户名

*/
Account::Account(std::string name) {
    this->username = name;

    //CoCreateGuid(&this->uuid); // 生成UUID

    //OLECHAR* guid_str = GUID2String(this->uuid);
    //std::wstring guidstr(guid_str);
    //guidstr = L"./account/" + guidstr;
    //
    //fs::create_directory(guidstr);
    //std::wstring accconfig = guidstr + L"/config.ini";
    //std::ofstream fout;
    //fout.open(accconfig, std::ios::out | std::ios::trunc);
    //if (!fout.is_open()) {
    //    return;
    //}
    //fout << "Name:" + name << "\r\n"
    //    << "GUID:";
    //fout.close();
    //std::wofstream wfout;
    //wfout.open(accconfig, std::ios::out);
    //if (!wfout.is_open()) {
    //    return;
    //}
    //wfout << guid_str << "\r\n";
    //wfout.close();
    //delete[] guid_str;
}
Account::~Account() {
    //for (int i = 0; i < this->attribe.size(); i++) {
    //    if (attribe[i].KeyPtr == nullptr) {
    //        delete this->attribe[i].KeyPtr; // 容易异常
    //    }
    //}
}
void* Account::ReadAttribe(std::string KeyName) {
    /*if (this->attribe.size() == 0)
        return nullptr;
    for (int i = 0; i < this->attribe.size(); i++) {
        if (this->attribe[i].KeyName.compare(KeyName) == 0) {
            return this->attribe[i].KeyPtr;
        }
    }*/
    return nullptr;
}
bool Account::WriteAttribe(std::string KeyName, void* KeyPtr) {
    /*if (this->attribe.size() == 0)
        return false;
    for (int i = 0; i < this->attribe.size(); i++) {
        if (this->attribe[i].KeyName.compare(KeyName) == 0) {
            this->attribe[i].KeyPtr = KeyPtr;
            return true;
        }
    }*/
    return false;
}
bool Account::AddAttribe(std::string KeyName, void* KeyPtr) {
    for (auto ite = this->attribe.begin(); ite != this->attribe.end(); ite++) {
        if (ite->KeyName.compare(KeyName) == 0) {
            return false;
        }
    }
    this->attribe.push_back(AccuAttr(KeyName, KeyPtr));
    return true;
}

// Something was wrong
int getAccountIndex(std::list<Account>& __map, UUID uuid) {
    int index = 0;
    for (auto ite = __map.begin(); ite != __map.end(); ite++) {
        index++;
        if (ite->uuid == uuid)
            return index;
    }
    return -1;
}
UUID getAccountUUID(std::list<Account>& __map, int index) {
    if (__map.size() < index) {
        return UUID();
    }
    return __map.begin()->uuid; // 问题同上
}
OLECHAR* GUID2String(UUID uuid)
{
    OLECHAR* guid_str = new OLECHAR();
    StringFromGUID2(uuid, guid_str, 39);
    return guid_str;
}
bool TravelFolder(const fs::path& pathpath)
{
    if (!fs::exists(pathpath))
    {
        return false;
    }
    if (!fs::is_directory(pathpath))
    {
        return false;
    }
    fs::directory_entry direntry(pathpath);
    if (direntry.status().type() == fs::file_type::directory)
    {
        //TODO something....
    }
    fs::directory_iterator dirite(pathpath);
    for (auto& ite : dirite)
    {
        if (ite.status().type() == fs::file_type::directory) //is folder
        {
            std::string strsubdir = ite.path().string();
            TravelFolder(strsubdir);
        }
        else if (ite.status().type() == fs::file_type::regular) //is file
        {
            //TODO other....
            std::string strfile = ite.path().filename().string();
        }
    }
    return true;
}

int account_init(std::list<Account>& __Val)
{
    if (!TravelFolder("account"))
        fs::create_directory("account");
    
    __Val.emplace_back(Account("admin"));

    DEBUGCODE();
	return 0;
}

void debug() {

}