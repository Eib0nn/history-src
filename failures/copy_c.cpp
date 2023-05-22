#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <ctime>
#include <iomanip>

std::string get_unique_id()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d-%H-%M-%S");
    return ss.str();
}

void Copy_A()
{
    char buf[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    std::string exe_path = buf;
    std::string dir_path = exe_path.substr(0, exe_path.find_last_of("\\/"));
    std::ostringstream oss;
    std::string unique_id = get_unique_id();
    std::ofstream file;
    int i = 1;
    while (i <= 10)
    {
        oss << unique_id << "_" << i << ".txt";
        file.open(oss.str());
        if (!file.is_open())
        {
            std::cerr << "Failed to create file: " << oss.str() << std::endl;
        }
        else
        {
            printf("Files created!\n");
        }
        oss.str("");
        file.close();
        i++;
    }
    i = 0;
}

int main()
{
    std::thread t1 (Copy_A);
    t1.detach();
    while(true){}
}