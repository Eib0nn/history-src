#define _WIN32_WINNT 0xA00
#include <iostream>
#include <windows.h>
#include <cstdlib>

using namespace std;
typedef int (*original_function)(int);

int Hook(int arg)
{
    original_function originalFn = reinterpret_cast<original_function>(originalFnADD);
    int result = originalFn(arg);
    return result;
}

uintptr_t originalFnADD = 0;

int main()
{
    // {...}
    DWORD desiredAccess = PROCESS_ALL_ACCESS;
    BOOL inheritHandle = FALSE;
    DWORD processID = 268;
    HANDLE process = OpenProcess(desiredAccess, inheritHandle, processID);
    if (process != NULL)
    {
        cout << "[+] Successfully opened process" << endl;
        LPVOID remoteMem = VirtualAllocEx(process, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (remoteMem != NULL)
        {
            cout << "[*] Memory allocated in the process at address: " << remoteMem << endl;

            // address
            const char *addressStr = "00007FFADC35F1F0";
            originalFnADD = std::stoull(addressStr, nullptr, 16);

            // trampoline (original function to hook)
            BYTE trampolineBytes[5];
            ReadProcessMemory(process, reinterpret_cast<LPCVOID>(originalFnADD), trampolineBytes, sizeof(trampolineBytes), nullptr);

            // patch original function with a (assembly) jmp to hook function
            int32_t relativeOffset = reinterpret_cast<int32_t>(&Hook) - (originalFnADD + 5);
            BYTE patchBytes[] = {0xE9, static_cast<BYTE>(relativeOffset & 0xFF), static_cast<BYTE>((relativeOffset >> 8) & 0xFF),
                                 static_cast<BYTE>((relativeOffset >> 16) & 0xFF), static_cast<BYTE>((relativeOffset >> 24) & 0xFF)};
            DWORD oldProtect;
            VirtualProtectEx(process, reinterpret_cast<LPVOID>(originalFnADD), sizeof(patchBytes), PAGE_EXECUTE_READWRITE, &oldProtect);
            WriteProcessMemory(process, reinterpret_cast<LPVOID>(originalFnADD), patchBytes, sizeof(patchBytes), nullptr);
            VirtualProtectEx(process, reinterpret_cast<LPVOID>(originalFnADD), sizeof(patchBytes), oldProtect, &oldProtect);

            // call the original function to redirect to the hook
            int result = reinterpret_cast<original_function>(originalFnADD)(42);

            // restore the original bytes of the modified function
            VirtualProtectEx(process, reinterpret_cast<LPVOID>(originalFnADD), sizeof(trampolineBytes), PAGE_EXECUTE_READWRITE, &oldProtect);
            WriteProcessMemory(process, reinterpret_cast<LPVOID>(originalFnADD), trampolineBytes, sizeof(trampolineBytes), nullptr);
            VirtualProtectEx(process, reinterpret_cast<LPVOID>(originalFnADD), sizeof(trampolineBytes), oldProtect, &oldProtect);

            VirtualFreeEx(process, remoteMem, 0, MEM_RELEASE);
        }
        else
        {
            cerr << "[-] Failed to allocate virtual memory; error code: " << GetLastError() << endl;
        }
        CloseHandle(process);
    }
    else
    {
        cerr << "[-] Failed to open process; error code: " << GetLastError() << endl;
    }
    return 0;
}