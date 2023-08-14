#include "EmuShell.h"

int emush_cd(wchar_t** args)
{
    if (args[1] == NULL)
    {
        _wperror(L"Emush error at cd, lack of args\n");
    }
    // cd 
    else
    {
        if (_wchdir(args[1]) != 0)
            _wperror(L"Emush error at chdir\n");
    }
    return 1;
}

int emush_help(wchar_t** args)
{
    _putws(L"This is Emulated Shell");
    _putws(L"You can't run any programs end with .bat or .com");
    _putws(L"Here are some built in cmd:");
    for (int i = 0; i < emush_builtin_nums(); i++)
        wprintf(L"%ws\n", builtin_cmd[i]);
    return 1;
}

int emush_exit(wchar_t** args)
{
    return 0;
}