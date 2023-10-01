#include "server.h"

int main()
{
    setlocale(LC_ALL, "");
    std::wcout << "\tHackEmu Server 0.1a\n";

    char url[] = "127.0.0.1";

    /*if (!ClientOpen(url, DEFAULT_CLIENT_PORT)) {
        std::wcerr << "INetCore loaded failed.\n";
        std::system("pause");
        return -1;
    }*/
    return 0;
}