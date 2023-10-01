/*

    EmuShell
    Unicode(UTF-16)实现

    Origin Author:
    bilibili：克拉克黎明之前
        https://space.bilibili.com/251474603
    Github Home
        https://github.com/mxkt0322

    Origin Repo:
        https://github.com/mxkt0322/BiliBili

    原作者未注明许可证
*/

//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NON_CONFORMING_WCSTOK


#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef int     ssize_t;
#endif

#include "EmuShell.h"

#define EMUSH_TOK_DELIM L" \t\r\n"
#define EMUSH_TOK_BUFFER_SIZE 256

#define STRBUFFER 256

int emush_builtin_nums();

const wchar_t* builtin_cmd[] =
{
    L"cd",
    L"help",
    L"exit"
};

int (*builtin_func[])(std::vector<std::wstring>&) =
{
    &emush_cd,
    &emush_help,
    &emush_exit
};

int emush_builtin_nums()
{
    return sizeof(builtin_cmd) / sizeof(builtin_cmd[0]);
}

void emush_read_line(wchar_t* line)
{
    //Only GNU/Linux
    //getline(&line, &bufsize, stdin);

    std::wcin.getline(line, 8192);
}

// 字符分割
// https://zhuanlan.zhihu.com/p/426939341
std::vector<std::wstring> stringSplit(const std::wstring& str, wchar_t delim) {
    std::size_t previous = 0;
    std::size_t current = str.find(delim);
    std::vector<std::wstring> elems;
    while (current != std::wstring::npos) {
        if (current > previous) {
            elems.push_back(str.substr(previous, current - previous));
        }
        previous = current + 1;
        current = str.find(delim, previous);
    }
    if (previous != str.size()) {
        elems.push_back(str.substr(previous));
    }
    return elems;
}

std::vector<std::wstring> emush_split_line(wchar_t* line)
{
    std::wstring Str(line);
    wchar_t wc[] = EMUSH_TOK_DELIM;
    return (stringSplit(Str, *wc));
}

int emush_launch(std::vector<std::wstring>& args) {

    // https://blog.csdn.net/u012442719/article/details/55261051
    STARTUPINFO si = { sizeof(STARTUPINFO) };//在产生子进程时，子进程的窗口相关信息
    PROCESS_INFORMATION pi;                  //子进程的ID/线程相关信息
    DWORD returnCode;//用于保存子程进的返回值;

    // 拼接字符串
    std::wstring comm(args[0]);
    for (int i = 1; i < args.size(); i++) {
        comm += L" ";
        comm += args[i];
    }

    // 创建进程
    BOOL bRet = CreateProcess(              //调用失败，返回0；调用成功返回非0；
        NULL,                               //一般都是空；（另一种批处理情况：此参数指定"cmd.exe",下一个命令行参数 "/c otherBatFile")
        comm.data(),                       //命令行参数         
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
        FALSE,                              //_In_        BOOL                  bInheritHandles,
        0,                 //新的进程不使用 Alooc
        NULL,                               //_In_opt_    LPVOID                lpEnvironment,
        NULL,                               //_In_opt_    LPCTSTR               lpCurrentDirectory,
        &si,                                //_In_        LPSTARTUPINFO         lpStartupInfo,
        &pi);                               //_Out_       LPPROCESS_INFORMATION lpProcessInformation

    // 判断
    if (bRet) {

        

        //std::wcout << L"process is running...\n";
        //等待子进程结束
        WaitForSingleObject(pi.hProcess, -1);

        //std::wcout << L"process is finished\n";
        //获取子进程的返回值
        GetExitCodeProcess(pi.hProcess, &returnCode);
        std::wcout << L"retCode:" << returnCode << std::endl;

        // 关闭句柄
        //CloseHandle(&si);
        //CloseHandle(&pi);

        return SH_SUCCESS;
    }

    //https://learn.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
    //https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes#system-error-codes
    ErrorFunc(std::wstring(L"CreateProcess").data());
    
    return SH_FAIL;
}

int emush_execute(std::vector<std::wstring>& args)
{
    // 判断输入内容是否为空
    if (&args == nullptr) return -1;
    if (args.size() == 0) return -1;

    for (int i = 0; i < emush_builtin_nums(); i++)
    {
        if (args[0].compare(builtin_cmd[i]) == 0)
            return (*builtin_func[i])(args);
    }
    return emush_launch(args);
}

void emush_loop()
{
    // 设置环境变量
    if (!(_wchdir(LR"(.\mnt\bin)") == -1 || _wchdir(LR"(..\mnt\bin)") == -1)) {
        wchar_t path[MAX_PATH];
        _wgetcwd(path, MAX_PATH);
        std::wcerr << "cannot find bin directory" << std::endl;
        std::system("pause>nul");
        std::exit(-3);
    }
    wchar_t path_cur[MAX_PATH];
    _wgetcwd(path_cur, MAX_PATH);
    if (SetEnvironmentVariable(L"Path", path_cur) == 0) {
        std::wcerr << "current directory are so long" << std::endl;
        std::system("pause>nul");
        std::exit(-4);
    }

    // 符号链接
    if (!CreateSymbolicLink(LR"(.\nano.exe)", LR"(.\nano\nano.exe)",
        SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE))
    {
        ErrorFunc(std::wstring(L"CreateSymbolicLink").data());
        std::wcerr << L"This means that you cannot directly execute commands such as nano" << std::endl;
        std::wcerr << L"You can try run as administrator or enable Developed Mode" << std::endl;
    }

    wchar_t* line = new wchar_t[8192]; // 经测试，cmd 能输入8190个字符

    // 数组置空
    /*for (int i = 0; i <= 256 - 1; i++) {
        line[i] = 0;
    }*/

    // 命令行数组
    std::vector<std::wstring> args;

    // 状态码
    int status=0;

    do
    {
        wchar_t path[MAX_PATH];
        _wgetcwd(path, MAX_PATH);

        std::wcout << L"[emush " << path << L" ] $";
        

        emush_read_line(line);
        args = emush_split_line(line);
        status = emush_execute(args);

    } while (status);

}

void emush_startup()
{
    setlocale(LC_ALL, ""); //设置区域

    // Ctrl+C Ctrl+Break 不中断
    SetConsoleCtrlHandler(&eHandlerRoutine, TRUE);

    std::wcout << L"HackEmu v0.1a\n";

    // 连接服务器
    char url[8192];
    readln:
    ZeroMemory(url, 8192);
    std::wcout << L"Input game server:\n";
    std::cin.getline(url, 8192);
    if (!ConnectServer(url))
        goto readln;

    // 播放 BGM
    //HANDLE h_BGM = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&PlayBGM, NULL, 0, NULL);

    return;
}

int wmain(int argc, wchar_t* argv[])
{
    emush_startup();
    emush_loop();
    return -1;
}