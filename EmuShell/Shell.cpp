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

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_WCSTOK


#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef int     ssize_t;
#endif

#include "EmuShell.h"


//Windows API 默认最大路径长度限制
// https://learn.microsoft.com/zh-cn/windows/win32/fileio/maximum-file-path-limitation
#define MAX_PATH 260 

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
    //std::streamsize bufsize = 256;

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
    //int buffer_size = EMUSH_TOK_BUFFER_SIZE, position = 0;
    ////wchar_t* tokens = new wchar_t[buffer_size];
    ////std::array<wchar_t*, EMUSH_TOK_BUFFER_SIZE> tokens();
    //wchar_t* token;
    //
    ////token = strtok(line, EMUSH_TOK_DELIM);
    //token = wcstok(line, EMUSH_TOK_DELIM);
    //while (token != nullptr)
    //{
    //    tokens[position++] = token;
    //    token = wcstok(nullptr, EMUSH_TOK_DELIM);
    //}
    //tokens[position] = NULL;
    //return tokens;

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
        comm = comm + L" ";
        comm = comm + args[i];
    }
    wchar_t commandLine[8192] = {0};
    wcscpy(commandLine, args[0].c_str());

    // 创建进程
    BOOL bRet = CreateProcessW(              //调用失败，返回0；调用成功返回非0；
        NULL,                               //一般都是空；（另一种批处理情况：此参数指定"cmd.exe",下一个命令行参数 "/c otherBatFile")
        commandLine,                       //命令行参数         
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
        FALSE,                              //_In_        BOOL                  bInheritHandles,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,                 //新的进程不使用新的窗口。
        NULL,                               //_In_opt_    LPVOID                lpEnvironment,
        NULL,                               //_In_opt_    LPCTSTR               lpCurrentDirectory,
        &si,                                //_In_        LPSTARTUPINFO         lpStartupInfo,
        &pi);                               //_Out_       LPPROCESS_INFORMATION lpProcessInformation

    // 判断
    if (bRet) {
        std::wcout << L"process is running...\n";
        //等待子进程结束
        WaitForSingleObject(pi.hProcess, -1);
        std::wcout << L"process is finished\n";
        //获取子进程的返回值
        GetExitCodeProcess(pi.hProcess, &returnCode);
        std::wcout << L"process return code:\n" << returnCode << L'\n';
        return 1;
    }
    std::wcerr << "error at create process" << std::endl;
    return 2;


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
    wchar_t* line =  new wchar_t[8192]; // 经测试，cmd 能输入8190个字符

    for (int i = 0; i <= 256 - 1; i++) {
        line[i] = 0;
    }

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

// 特定于 Microsoft 的扩展
int wmain(int argc, wchar_t* argv[])
{
    //_wsetlocale(LC_ALL, L"zh-CN"); //设置区域
    emush_loop();
    return -1;
}