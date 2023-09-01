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

// �ַ��ָ�
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
    STARTUPINFO si = { sizeof(STARTUPINFO) };//�ڲ����ӽ���ʱ���ӽ��̵Ĵ��������Ϣ
    PROCESS_INFORMATION pi;                  //�ӽ��̵�ID/�߳������Ϣ
    DWORD returnCode;//���ڱ����ӳ̽��ķ���ֵ;

    // ƴ���ַ���
    std::wstring comm(args[0]);
    for (int i = 1; i < args.size(); i++) {
        comm = comm + L" ";
        comm = comm + args[i];
    }
    wchar_t commandLine[8192] = {0};
    wcscpy(commandLine, args[0].c_str());

    // ��������
    BOOL bRet = CreateProcessW(              //����ʧ�ܣ�����0�����óɹ����ط�0��
        NULL,                               //һ�㶼�ǿգ�����һ��������������˲���ָ��"cmd.exe",��һ�������в��� "/c otherBatFile")
        commandLine,                       //�����в���         
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
        FALSE,                              //_In_        BOOL                  bInheritHandles,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,                 //�µĽ��̲�ʹ���µĴ��ڡ�
        NULL,                               //_In_opt_    LPVOID                lpEnvironment,
        NULL,                               //_In_opt_    LPCTSTR               lpCurrentDirectory,
        &si,                                //_In_        LPSTARTUPINFO         lpStartupInfo,
        &pi);                               //_Out_       LPPROCESS_INFORMATION lpProcessInformation

    // �ж�
    if (bRet) {
        std::wcout << L"process is running...\n";
        //�ȴ��ӽ��̽���
        WaitForSingleObject(pi.hProcess, -1);
        std::wcout << L"process is finished\n";
        //��ȡ�ӽ��̵ķ���ֵ
        GetExitCodeProcess(pi.hProcess, &returnCode);
        std::wcout << L"process return code:\n" << returnCode << L'\n';
        return 1;
    }
    std::wcerr << "error at create process" << std::endl;
    return 2;


}

int emush_execute(std::vector<std::wstring>& args)
{
    // �ж����������Ƿ�Ϊ��
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
    wchar_t* line =  new wchar_t[8192]; // �����ԣ�cmd ������8190���ַ�

    for (int i = 0; i <= 256 - 1; i++) {
        line[i] = 0;
    }

    // ����������
    std::vector<std::wstring> args;

    // ״̬��
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

// �ض��� Microsoft ����չ
int wmain(int argc, wchar_t* argv[])
{
    //_wsetlocale(LC_ALL, L"zh-CN"); //��������
    emush_loop();
    return -1;
}