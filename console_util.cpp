#include "console_util.h"

int console_util::_argc = 0;
char** console_util::_argv = nullptr;
string console_util::parameters[18] = {
        "-h","--help","-o","--output",
        "-a","--author","-c","--classify",
        "-in","--index","-cs","--classifications",
        "-pr","--project","-ab","--about",
        "-h","--has"};
::Status console_util::status;

void console_util::init(int argc, char** argv){
    _argc = argc;
    _argv = argv;
}

void console_util::printHelp(){
    cout << "Usage: md2hm [options] file\n"
            "Options:\n"
            "        -h  --help              显示帮助信息\n"
            "        -o  --output            文章的标题，没有输入，就默认文件名\n"
            "        -a  --author            显示的博主名字，第一次使用时需要指定，后续可以不指定\n"
            "        -c  --classify          文章所属的分类，确保对应的分类存在，否则将不会为其分类\n"
            "        -in --index             指定该文章为index.html中所显示的内容，文章内容垂直居中，此时会无视-o参数\n"
            "        -cs --classifications   指定分类中的类型，最少4个，最多9个，最后一个为默认分类，第一次必须指定\n"
            "        -pr --project           指定该文件为项目的文件，在参数后面需要跟随项目的链接\n"
            "        -ab --about             指定该文章为about.html中显示的内容，此时会无视-o参数\n"
            "        -h  --has               指定已经存在的博客文件夹的路径，若是没有指定，则会新建博客\n"
            "\n"
            "该程序必须要在有安装git的环境下使用，以克隆对应的博客模板，从而进行修改\n"
            "产生的页面将会在保存在对应的指定的文件夹中对应的位置，由用户将博客上传到自己的服务器或代码托管平台这是将markdown 转化为此博客模板所需的html文件而做的，由于我们学艺不精，所以可能会有错误，如果可以，请向我们提出\n"
            "声明：本程序开源，博客模板也是开源的，均可以在github上查看到";
}

void console_util::matchParameter(){
    if(_argc == 1){
        printHelp();
        return;
    }else {
        int check_result = checkParameter();
        if(check_result == status.error)
            exit(-1);
        else if(check_result == status.help){
            printHelp();
            exit(0);
        }else if(check_result != status.success){
            // 以防万一，方便后续的修改
            exit(-1);
        }
        // 进行命令的解析与执行
        set<char*> classifications;
        console_util::parseParameter(classifications);
    }
}

void console_util::parseParameter(set<char*>& classifications) {
    set<int> contain_parameters;
    bool classification = false;
    bool contain_parameter = false;
    for(int i = 1;i < _argc - 1;i++){
        LOOP:
        for(int j = 0;j < 18;j++){
            if(_argv[i] == parameters[j]){
                contain_parameters.insert(j);
                if(j % 2 == 0)
                    contain_parameters.insert(j + 1);
                else
                    contain_parameters.insert(j - 1);
                contain_parameter = true;
                classification = false;
                if(j == 6 || j == 7)
                    classification = true;
                i++;
                goto LOOP;
            }else if(classification){
                classifications.insert(_argv[i]);
                break;
            }
        }
        if(contain_parameter)
            contain_parameter = false;
    }
    if(!file_util::fileExist(string(_argv[_argc - 1]))){
        cerr << "\033[31m" << "文件不存在：" << _argv[_argc - 1] << "\033[0m";
        exit(-1);
    }
}

short int console_util::checkParameter() {
    set<int> contain_parameters;
    bool classification = false;
    short int classification_num = 0;
    bool contain_parameter = false;
    for(int i = 1;i < _argc;i++){
        LOOP:
        for(int j = 0;j < 18;j++){
            if(_argv[i] == parameters[j]){
                if(contain_parameters.find(j) != contain_parameters.end()) {
                    cerr << "\033[31m" << "重复参数：" << _argv[i] << "\033[0m";
                    return status.error;
                }
                if(classification && classification_num < 4){
                    cerr << "\033[31m" << "分类最少为4个" << "\033[0m";
                    return status.error;
                }
                contain_parameters.insert(j);
                if(j % 2 == 0)
                    contain_parameters.insert(j + 1);
                else
                    contain_parameters.insert(j - 1);
                contain_parameter = true;
                classification = false;
                if(j == 6 || j == 7){
                    classification = true;
                }else if(j == 0 || j == 1){
                    return status.help;
                }
                i++;
                goto LOOP;
            }else if(classification){
                classification_num++;
                if(classification_num > 9){
                    cerr << "\033[31m" << "分类最多为9个" << "\033[0m";
                    return status.error;
                }
                break;
            } else if(j == 17){
                cerr << "\033[31m" << "未知命令：" << _argv[i] << "\033[0m";
                return status.error;
            }
        }
        if(contain_parameter)
            contain_parameter = false;
    }
    if(classification && classification_num < 4){
        cerr << "\033[31m" << "分类最少为4个" << "\033[0m";
        return status.error;
    }
    return status.success;
}

