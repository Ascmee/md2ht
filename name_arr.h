
#ifndef CL_MDTOHTML_NAME_ARR_H
#define CL_MDTOHTML_NAME_ARR_H

#include <string>
using namespace std;
class name_arr
{
public:
    //name_arr();
    int data_edit(const string &data_str);
    int title_edit(const string &title_str);
    int classification_edit(const string &classification_str);
private:
    string data;
    string title;
    string classification;

};
extern int a;

#endif //CL_MDTOHTML_NAME_ARR_H
