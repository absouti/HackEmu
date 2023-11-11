#include "server.h"
#include "account.h"
#include "vfs.h"

int vfs_init() {
    VFileSystem vfs;
    vfs.AddFile("home", vfile_type::directory, "/", nullptr, 0);
    
    return 0;
}

int main()
{

    setlocale(LC_ALL, "");
    std::cout << "HackEmu Server 0.1a\n";

    Timer *timer = new Timer;

    std::list<Account> Ac;
    account_init(Ac);
    vfs_init();

    if (ServerOpen(DEFAULT_PORT)) {
        std::wcerr << "INetCore loaded failed.\n";
        std::system("pause");
        return 0;
    }
    delete timer;
    std::wcout << L"Done!\n";
    return -1;
}