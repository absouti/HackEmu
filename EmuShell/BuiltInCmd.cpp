#include "EmuShell.h"

int emush_cd(std::vector<std::wstring>& args)
{
    if (args.size() == 1)
    {
        //_wperror(L"Emush error at cd, lack of args\n");
        std::wcerr << L"Emush error at cd, lack of args" << std::endl;
        return -1;
    }
    // cd 
    if (_wchdir(args[1].c_str()) != 0)
    {
        _wperror(L"");
        return -1;
    }

    return 1;
}

int emush_help(std::vector<std::wstring>& args)
{
    _putws(L"This is Emulated Shell");
    _putws(L"You can't run any programs end with .bat or .com");
    _putws(L"Here are some built in cmd:");
    for (int i = 0; i < emush_builtin_nums(); i++)
        wprintf(L"%ws\n", builtin_cmd[i]);
    return 1;
}

int emush_exit(std::vector<std::wstring>& args)
{
    exit(0);
}

int ConnectServer(char* url) {
    if (ClientOpen(url, DEFAULT_SERVER_PORT)) {
        std::wcerr << "INetCore loaded failed.\n";
        return 0;
    }
    
    return 1;
}