#include <iostream>
#include <string>
#include <vector>
#include "name_arr.h"
#include "data_convert.h"
#include "console_util.h"
using namespace std;



int main(int argc,char* argv[])
{
    system("chcp 65001");
    system("cls");
    vector<name_arr> v1;
    string ori_data,ori_title,ori_classification;
    string user_input;
    console_util::init(argc,argv);
    console_util::matchParameter();
    return 0;
}