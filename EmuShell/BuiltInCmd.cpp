#include "EmuShell.h"

int emush_cd(std::vector<std::wstring>& args)
{
    if (args.size() == 1)
    {
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

int ConnectServer(std::vector<std::wstring>& args) {
    if (args.size() != 1)
        return 0;
    char url[512] = { 0 };
    WideCharToMultiByte(CP_ACP, WC_ERR_INVALID_CHARS, args[1].data(), -1,
        url, -1, NULL, NULL);
    int Ret = ClientOpen(url, DEFAULT_PORT);
    if (Ret) {
        std::cerr << "INetCore loaded failed.\n";
        return 0;
    }
    return 1;
}