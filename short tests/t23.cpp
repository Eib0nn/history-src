#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WHINTHRESHOLD 0x0A00
#include "mingw.invoke.h"
#include "mingw.thread.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include "mingw.mutex.h"

using namespace mingw_stdthread;

std::mutex console_mutex;
std::atomic<bool> quit_program(true);

void printF()
{
    while (quit_program)
    {
        try
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            int *ptr = new int[10];
            for (int i = 0; i < 10; i++)
            {
                ptr[i] = i;
            }
            for (int i = 0; i < 10; i++)
            {
                ptr[i] = ptr[i] * 1000000000;
            }
        }
        catch (std::exception const &ex)
        {
            std::lock_guard<std::mutex> lock(console_mutex);
        }
    }
}
void printF_2()
{
    while (quit_program)
    {
        try
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            int *ptr = new int[10];
            for (int i = 0; i < 10; i++)
            {
                ptr[i] = i;
            }
            for (int i = 0; i < 10; i++)
            {
                ptr[i] = ptr[i] * 1000000000;
            }
        }
        catch (std::exception const &ex)
        {
            std::lock_guard<std::mutex> lock(console_mutex);
        }
    }
}

int main()
{
    LPCSTR Appname = "teste02-03.exe";
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
    if (!CreateProcessA(Appname, NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)){
        
    }
    try
    {

        thread t1(printF);
        thread t2(printF_2);
        t1.detach();
        t2.detach();

        // Wait for the user to press a key before exiting
        getchar();
        return 0;
    }
    catch (std::exception const &ex)
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        return 1;
    }
}