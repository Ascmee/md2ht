
#include "name_arr.h"
name_arr n1;
int name_arr::data_edit(const std::string &data_str)
{
    this->data = data_str;
    return 0;
}
int name_arr::title_edit(const std::string &title_str)
{
    this->title = title_str;
    return 0;
}
int name_arr::classification_edit(const std::string &classification_str)
{
    this->classification = classification_str;
    return 0;
}




