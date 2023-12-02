#include "file_util.h"

bool file_util::fileExist(std::string path) {
    fstream ifs(path,ios::in);
    if(ifs.is_open()){
        ifs.close();
        return true;
    }
    return false;
}