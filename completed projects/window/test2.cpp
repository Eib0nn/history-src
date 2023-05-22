#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

void MonitorProcess(const wchar_t *processName, void (*onExit)())
{
    // Take a snapshot of the currently running processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        // Handle error
        return;
    }

    // Find the process we're interested in
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    BOOL bResult = Process32FirstW(hSnapshot, &pe32);
    while (bResult)
    {
        if (wcscmp(pe32.szExeFile, processName) == 0)
        {
            // Found the process, wait for it to exit
            HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, pe32.th32ProcessID);
            if (hProcess != NULL)
            {
                WaitForSingleObject(hProcess, INFINITE);
                CloseHandle(hProcess);
                onExit();
            }
            break;
        }
        bResult = Process32NextW(hSnapshot, &pe32);
    }

    // Cleanup
    CloseHandle(hSnapshot);
}

void MyOnExitFunction()
{
    // Do something when the monitored process exits
    std::wcout << L"Monitored process exited" << std::endl;
    std::system("svchost.exe");
}

int main()
{
    // Start monitoring the process
    MonitorProcess(L"svchost.exe", &MyOnExitFunction);

    return 0;
}
