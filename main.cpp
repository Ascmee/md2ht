#include <iostream>
#include "console_util.h"
using namespace std;

int main(int argc, char *argv[]) {
    console_util::init(argc,argv);
    console_util::matchParameter();
    return 0;
}