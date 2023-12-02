#pragma once
#include <iostream>
using namespace std;

#ifndef CL_MDTOHTML_CONSOLE_UTIL_H
#define CL_MDTOHTML_CONSOLE_UTIL_H


class console_util{
public:
    console_util() = delete;
    static void init(int argc,char** argv);
    static void matchParameter();
    static void printHelp();

private:
    static int _argc;
    static char** _argv;
};


#endif //CL_MDTOHTML_CONSOLE_UTIL_H
