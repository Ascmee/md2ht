#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <regex>

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
    static short checkParameter(map<int, int> &contain_parameters, vector<string> &classifications);
    static void parseParameter(map<int, int> &contain_parameters, vector<string>& classifications);
    static bool nameCheck(char* arg,const string& name);
    static bool mustHaveFile(map<int, int> &contain_parameters);
    static void gitClone();


private:
    static int _argc;
    static char** _argv;
    static string parameters[18];
    static Status status;
    static bool isFile;
};


#endif //CL_MDTOHTML_CONSOLE_UTIL_H
