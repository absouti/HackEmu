/*

    EmuShell
    Unicode(UTF-16)ʵ��

    Origin Author:
    bilibili������������֮ǰ
        https://space.bilibili.com/251474603
    Github Home
        https://github.com/mxkt0322

    Origin Repo:
        https://github.com/mxkt0322/BiliBili

    ԭ����δע�����֤
*/

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_WCSTOK


#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef int     ssize_t;
#endif

#include "EmuShell.h"


//Windows API Ĭ�����·����������
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

int (*builtin_func[])(wchar_t**) =
{
    &emush_cd,
    &emush_help,
    &emush_exit
};

int emush_builtin_nums()
{
    return sizeof(builtin_cmd) / sizeof(builtin_cmd[0]);
}

void emush_read_line(wchar_t* Str)
{
    std::streamsize bufsize = 256;

    //Only GNU/Linux
    //getline(&line, &bufsize, stdin);

    std::wcin.getline(Str, bufsize);
}

//parsing 


wchar_t** emush_split_line(wchar_t* line)
{
    int buffer_size = EMUSH_TOK_BUFFER_SIZE, position = 0;
    wchar_t** tokens = (wchar_t**)malloc(buffer_size * sizeof(wchar_t*));
    wchar_t* token;
    // "cd /desk/main"
    //token = strtok(line, EMUSH_TOK_DELIM);
    token = wcstok(line, EMUSH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position++] = token;
        token = wcstok(NULL, EMUSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int emush_launch(wchar_t** args) {

    // https://blog.csdn.net/u012442719/article/details/55261051
    STARTUPINFO si = { sizeof(STARTUPINFO) };//�ڲ����ӽ���ʱ���ӽ��̵Ĵ��������Ϣ
    PROCESS_INFORMATION pi;                  //�ӽ��̵�ID/�߳������Ϣ
    DWORD returnCode;//���ڱ����ӳ̽��ķ���ֵ;

    wchar_t commandLine1[512] = {0};
    wcscpy(commandLine1, args[0]);  //���������в���һ

    BOOL bRet = CreateProcessW(              //����ʧ�ܣ�����0�����óɹ����ط�0��
        NULL,                               //һ�㶼�ǿգ�����һ��������������˲���ָ��"cmd.exe",��һ�������в��� "/c otherBatFile")
        commandLine1,                       //�����в���         
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
        FALSE,                              //_In_        BOOL                  bInheritHandles,
        CREATE_NEW_CONSOLE,                 //�µĽ���ʹ���µĴ��ڡ�
        NULL,                               //_In_opt_    LPVOID                lpEnvironment,
        NULL,                               //_In_opt_    LPCTSTR               lpCurrentDirectory,
        &si,                                //_In_        LPSTARTUPINFO         lpStartupInfo,
        &pi);                               //_Out_       LPPROCESS_INFORMATION lpProcessInformation

    if (bRet)
    {
        std::wcout << L"process is running...\n";
        //�ȴ��ӽ��̽���
        WaitForSingleObject(pi.hProcess, -1);
        std::wcout << L"process is finished\n";
        //��ȡ�ӽ��̵ķ���ֵ
        GetExitCodeProcess(pi.hProcess, &returnCode);
        std::wcout << L"process return code:\n" << returnCode << L'\n';
    }
    else
    {
        _wperror(L"Create Process error!\n");
        //return 0;
    }

    return 1;
}

int emush_execute(wchar_t** args)
{
    if (args[0] == NULL) return 1;

    for (int i = 0; i < emush_builtin_nums(); i++)
        if (wcscmp(args[0], builtin_cmd[i]) == 0)
            return (*builtin_func[i])(args);
    return emush_launch(args);
}

void emush_loop()
{
    wchar_t* line =  new wchar_t[256];

    for (int i = 0; i <= 256 - 1; i++) {
        line[i] = 0;
    }

    // "cd /desk/main"
    // "cd" "/desk/main"
    wchar_t** args;
    int status=0;

    do
    {
        wchar_t path[MAX_PATH];
        _wgetcwd(path, MAX_PATH);

        std::wstring path_cpp = std::wstring(path);

        std::wcout << L"[emush " << path_cpp << L" ]$ ";
        

        emush_read_line(line);
        args = emush_split_line(line);
        status = emush_execute(args);

        free(args);
    } while (status);

}


//int main(int argc, char* argv[])
// Only Microsoft Can Do!!!
// �ض��� Microsoft ����չ
int wmain(int argc, wchar_t* argv[])
{
    _wsetlocale(LC_ALL, L"zh-CN"); //��������
    emush_loop();
    return 0;
}