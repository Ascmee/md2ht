#include "data_convert.h"
#include "name_arr.h"
#include "string"
#include <ctime>
#include <sstream>
using namespace std;

string get_time(name_arr &n1)
{
    time_t t1;
    time(&t1);
    auto *p = localtime(&t1);

    string s1;
    stringstream ss1;
    ss1 << p->tm_year + 1900 << "/"
        << ((p->tm_mon + 1 >= 10) ? "" : "0") << p->tm_mon + 1 << "/"
        << ((p->tm_mday >= 10) ? "" : "0") << p->tm_mday << "/"
        << ((p->tm_hour >= 10) ? "" : "0") << p->tm_hour << ":"
        << ((p->tm_min >= 10) ? "" : "0") << p->tm_min << ":"
        << ((p->tm_sec >= 10) ? "" : "0") << p->tm_sec;
    s1 = ss1.str();
    n1.data_edit(s1);
    return s1;
}