#include <iostream>
#include <dirent.h> 
#include <cstring>
#include <io.h>

/*So now on this is a prototype of my old idea, but now im more prepared to do it (i think), the initial idea is the
iterate function that will put all the directory paths inside a vector, and then process_directory will make anything
that i want with all of them, this can be kinda abstract to me so will see what happens, but the idea looks good*/

/*void process_directory(const std::string& path){
    {...}
}*/

void iterate_subdirs(const std::string &dir_path /*, std::vector<std::string>& dirs*/)
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
            iterate_subdirs(subdir_path);
        }
    } while (_findnext(handle, &file_info) == 0);

    _findclose(handle);
}

int main(){
    std::string str = "C:\\";
    iterate_subdirs(str);
}