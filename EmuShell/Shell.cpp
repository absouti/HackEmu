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
#include "EmuShell.h"

#define EMUSH_TOK_DELIM L" \t\r\n"
#define EMUSH_TOK_BUFFER_SIZE 256

#define STRBUFFER 256

int emush_builtin_nums();

const wchar_t* builtin_cmd[] =
{
    L"cd",
    L"help",
    L"exit",
    L"connect"
};

int (*builtin_func[])(std::vector<std::wstring>&) =
{
    &emush_cd,
    &emush_help,
    &emush_exit,
    &ConnectServer
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
        comm += L" ";
        comm += args[i];
    }

    // ��������
    BOOL bRet = CreateProcess(              //����ʧ�ܣ�����0�����óɹ����ط�0��
        NULL,                               //һ�㶼�ǿգ�����һ��������������˲���ָ��"cmd.exe",��һ�������в��� "/c otherBatFile")
        comm.data(),                       //�����в���         
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
        FALSE,                              //_In_        BOOL                  bInheritHandles,
        0,                 //�µĽ��̲�ʹ�� Alooc
        NULL,                               //_In_opt_    LPVOID                lpEnvironment,
        NULL,                               //_In_opt_    LPCTSTR               lpCurrentDirectory,
        &si,                                //_In_        LPSTARTUPINFO         lpStartupInfo,
        &pi);                               //_Out_       LPPROCESS_INFORMATION lpProcessInformation

    // �ж�
    if (bRet) {

        

        //std::wcout << L"process is running...\n";
        //�ȴ��ӽ��̽���
        WaitForSingleObject(pi.hProcess, -1);

        //std::wcout << L"process is finished\n";
        //��ȡ�ӽ��̵ķ���ֵ
        GetExitCodeProcess(pi.hProcess, &returnCode);
        std::wcout << L"retCode:" << returnCode << std::endl;

        // �رվ��
        //CloseHandle(&si);
        //CloseHandle(&pi);

        return SH_SUCCESS;
    }

    //https://learn.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
    //https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes#system-error-codes
	wchar_t e1[] = L"CreateProcess";
    ErrorFunc(e1);
    
    return SH_FAIL;
}

int emush_execute(std::vector<std::wstring>& args)
{
    // �ж����������Ƿ�Ϊ��
    //if (&args == nullptr) return -1;
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
    

    wchar_t* line = new wchar_t[8192]; // �����ԣ�cmd ������8190���ַ�

    // �����ÿ�
    /*for (int i = 0; i <= 256 - 1; i++) {
        line[i] = 0;
    }*/

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

void emush_startup()
{
    setlocale(LC_ALL, ""); //��������

    // Ctrl+C Ctrl+Break ���ж�
    SetConsoleCtrlHandler(&eHandlerRoutine, TRUE);

    std::wcout << L"HackEmu v0.1a\n";

    // ���ӷ�����
    char url[8192];
    readln:
    ZeroMemory(url, 8192);
    std::wcout << L"Input game server:\n";
    std::cin.getline(url, 8192);
    if (ClientOpen(url, DEFAULT_PORT))
    {
        std::cerr << "INetCore loaded failed." << std::endl;
        goto readln;
    }
        

    // ���� BGM
    //HANDLE h_BGM = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&PlayBGM, NULL, 0, NULL);

    return;
}

int main(int argc, char* argv[])
{
    emush_startup();
    emush_loop();
    return -1;
}
