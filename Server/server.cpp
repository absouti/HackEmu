#include "server.h"
#include "account.h"
#include "vfs.h"

int vfs_init() {
    VFileSystem vfs;
    char* exe1 = new char[] {"abcdefg"};
    
    return 0;
}

int main()
{

    setlocale(LC_ALL, "");
    std::cout << "HackEmu Server 0.1a\n";

    Timer *timer = new Timer;

    std::vector<Account> AccuManager;
    account_init(AccuManager);
    //vfs_init();
    return -124;

    if (ServerOpen(DEFAULT_PORT)) {
        std::wcerr << "INetCore loaded failed.\n";
        std::system("pause");
        return 0;
    }
    delete timer;
    std::wcout << L"Done!\n";
    return -1;
}