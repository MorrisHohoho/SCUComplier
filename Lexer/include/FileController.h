//
// Created by 芝士海豹 on 2023/4/4.
//

#ifndef CMINUS_COMPLIER_FILECONTROLLER_H
#define CMINUS_COMPLIER_FILECONTROLLER_H

#include<iostream>
#include<filesystem>
#include<vector>
#include<string>


//FileController is used to read files from a directory.
class FileController {
private:
    std::vector<std::string> fileContent;
    std::vector<std::string>::iterator  fileIterator=fileContent.begin();

public:
    FileController()=default;
    ~FileController()=default;

    int ReadFile(const std::string& path);
    std::string GetFile();
    void SetOutputPath(const std::string& p);
};


#endif //CMINUS_COMPLIER_FILECONTROLLER_H
