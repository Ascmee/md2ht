#include <iostream>
#include <string>
#include "console_util.h"
using namespace std;

int main(int argc, char *argv[]) {
    
    system("chcp 65001");
    system("cls");
    console_util::init(argc,argv);
    console_util::matchParameter();
    return 0;
}