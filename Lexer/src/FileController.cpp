//
// Created by 芝士海豹 on 2023/4/4.
//

#include "FileController.h"
using namespace std;
using namespace std::filesystem;


/**
 * Read files from a directory.
 * @param filePath
 * @return
 */
int FileController::ReadFile(const string& filePath) {
    path p(filePath);
    if(!exists(p))
    {
        cerr<<"File not found.\n";
        return -1;
    }
    if(is_directory(p))
    {
        for(auto &x:recursive_directory_iterator(p))
            if(is_regular_file(x))
                fileContent.push_back(x.path().string());
    }
    else
    {
        fileContent.push_back(filePath);
    }
    fileIterator=fileContent.begin();
    return 0;
}

/**
 * Get file from fileContent.
 * @return
 */
string FileController::GetFile() {
    if(fileIterator!=fileContent.end())
        return *fileIterator++;
    else
        return "";
}

void FileController::SetOutputPath(const string& pa) {
    path p(pa);
    if(!exists(p))
    {
        create_directories(p);
        cout<<"Output path created.\n";
    }
    else if(!is_directory(p))
    {
        cerr<<"Output path is not a directory.\n";
        return;
    }
}
