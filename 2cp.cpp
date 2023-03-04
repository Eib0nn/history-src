#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WHINTHRESHOLD 0x0A00
#include "mingw.invoke.h"
#include "mingw.thread.h"
#include "mingw.mutex.h"
#include <iostream>
#include <windows.h>
using namespace mingw_stdthread;

std::mutex console;

void test_thread(){
    while(true){
        int a;
        try{
            std::lock_guard<std::mutex> lock(console);
            printf("WorkingPT2\n");
            
        }
        catch(std::exception const &ex){
            std::lock_guard<std::mutex> lock(console);
            printf("Exception: %s\n", ex.what());
        }
    }
}
int main(){
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
    try{
        thread t2 (test_thread);
        t2.detach();
        printf("Press any key to exit...\n");
        getchar();
        return 0;
    }
    catch(std::exception const &ex){
        std::lock_guard<std::mutex> lock(console);
        printf("Exception caught: %s\n", ex.what());
        return 1;
    }
}