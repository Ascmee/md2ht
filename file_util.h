#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#ifndef CL_MDTOHTML_FILE_UTIL_H
#define CL_MDTOHTML_FILE_UTIL_H


class file_util {
public:
    static bool fileExist(string path);
};


#endif //CL_MDTOHTML_FILE_UTIL_H
