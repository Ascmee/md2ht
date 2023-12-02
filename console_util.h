#include <iostream>
#include <set>
#include "file_util.h"
using namespace std;

#ifndef CL_MDTOHTML_CONSOLE_UTIL_H
#define CL_MDTOHTML_CONSOLE_UTIL_H

struct Status {
    short int error = 1;
    short int help = 1 << 1;
    short int success = 1 << 2;
};

class console_util{
public:
    console_util() = delete;
    static void init(int argc,char** argv);
    static void matchParameter();
    static void printHelp();
    static short int checkParameter();
    static void parseParameter(set<char*>& classifications);


private:
    static int _argc;
    static char** _argv;
    static string parameters[18];
    static Status status;
};


#endif //CL_MDTOHTML_CONSOLE_UTIL_H
