#include "account.h"

int main()
{

    setlocale(LC_ALL, "");
    std::wcout << "HackEmu Server 0.1a\n";

    Timer *timer = new Timer;

    std::vector<Account> AccuManager;
    account_init(AccuManager);

    if (ServerOpen(DEFAULT_PORT)) {
        std::wcerr << "INetCore loaded failed.\n";
        std::system("pause");
        return 0;
    }
    delete timer;
    std::wcout << L"Done!\n";
    return -1;
}