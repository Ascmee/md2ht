#include <map>
#include "console_util.h"
#include "file_util.hpp"

int console_util::_argc = 0;
char **console_util::_argv = nullptr;
string console_util::parameters[18] = {
        "-h", "--help", "-o", "--output",
        "-a", "--author", "-c", "--classify",
        "-in", "--index", "-cs", "--classifications",
        "-pr", "--project", "-ab", "--about",
        "--has", "--has"};
::Status console_util::status;
bool console_util::isFile = false;

void console_util::init(int argc, char **argv) {
    _argc = argc;
    _argv = argv;
}

void console_util::printHelp() {
    cout << "Usage: md2ht [options] file\n"
            "Options:\n"
            "        -h  --help              显示帮助信息\n"
            "        -o  --output            文章的标题，没有输入，就默认文件名\n"
            "        -a  --author            显示的博主名字，名字后面需要跟随自己的github页面或其他网址，第一次使用时需要指定，后续可以不指定\n"
            "        -c  --classify          文章所属的分类，确保对应的分类存在，否则将不会为其分类\n"
            "        -in --index             指定该文章为index.html中所显示的内容，文章内容垂直居中，此时会无视-o参数\n"
            "        -cs --classifications   指定分类中的类型，最少4个，最多9个，最后一个为默认分类，第一次必须指定\n"
            "        -pr --project           指定该文件为项目的文件，在参数后面需要跟随项目的链接\n"
            "        -ab --about             指定该文章为about.html中显示的内容，此时会无视-o参数\n"
            "        --has                   指定已经存在的博客文件夹的路径，若是没有指定，则会新建博客，指定后，后续都会用这个目录\n"
            "\n"
            "该程序必须要在有安装git的环境下使用，以克隆对应的博客模板，从而进行修改\n"
            "产生的页面将会在保存在对应的指定的文件夹中对应的位置，由用户将博客上传到自己的服务器或代码托管平台这是将markdown 转化为此博客模板所需的html文件而做的，由于我们学艺不精，所以可能会有错误，如果可以，请向我们提出\n"
            "声明：本程序开源，博客模板也是开源的，均可以在github上查看到";
}

void console_util::gitClone() {
    system("cls");
    if (system("git -v")) {
        system("cls");
        cerr << "\033[31m" << "请检查git环境" << "\033[0m";
        exit(-1);
    }
    string str = "123";
    str += "5423";
    system("git clone https://github.com/Ascmee/blog_template.git");
    fileInfo.dir_path = "blog_template";
    fileInfo.hasDir = false;
    file_util::writeTo("cache\\has.txt",fileInfo.dir_path);
}

void console_util::matchParameter() {
    if (_argc == 1) {
        printHelp();
        return;
    } else {
        map<int, int> contain_parameters;
        vector<string> classifications;
        int check_result = checkParameter(contain_parameters, classifications);
        if (check_result == status.error)
            exit(-1);
        else if (check_result == status.help) {
            printHelp();
            exit(0);
        } else if (check_result != status.success) {
            // 以防万一，方便后续的修改
            exit(-1);
        }
        // 避免命令的冲突
        if (contain_parameters.count(8)) {
            if (contain_parameters.count(2)) {
                cerr << "\033[31m" << _argv[contain_parameters[8]] << "与" << _argv[contain_parameters[2]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            } else if (contain_parameters.count(6)) {
                cerr << "\033[31m" << _argv[contain_parameters[8]] << "与" << _argv[contain_parameters[6]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            } else if (contain_parameters.count(12)) {
                cerr << "\033[31m" << _argv[contain_parameters[8]] << "与" << _argv[contain_parameters[12]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            } else if (contain_parameters.count(14)) {
                cerr << "\033[31m" << _argv[contain_parameters[8]] << "与" << _argv[contain_parameters[14]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            }
        }
        if (contain_parameters.count(12)) {
            if (contain_parameters.count(14)) {
                cerr << "\033[31m" << _argv[contain_parameters[12]] << "与" << _argv[contain_parameters[14]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            }
        }
        if (contain_parameters.count(14)) {
            if (contain_parameters.count(2)) {
                cerr << "\033[31m" << _argv[contain_parameters[14]] << "与" << _argv[contain_parameters[2]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            } else if (contain_parameters.count(6)) {
                cerr << "\033[31m" << _argv[contain_parameters[14]] << "与" << _argv[contain_parameters[6]] << "不能一起使用"
                     << "\033[0m";
                exit(-1);
            }
        }
        // 进行命令的解析与执行
        console_util::parseParameter(contain_parameters, classifications);
    }
}

void console_util::parseParameter(map<int, int> &contain_parameters, vector<string> &classifications) {
    // has
    if (contain_parameters.count(16)) {
        fileInfo.dir_path = _argv[contain_parameters[16] + 1];
        if(!file_util::hasDir("cache"))
            system("mkdir cache");
        file_util::writeTo("cache\\has.txt",fileInfo.dir_path);
    } else if(file_util::fileExist("cache\\has.txt"))
        fileInfo.dir_path = file_util::readFrom("cache\\has.txt");
    else gitClone();
    // output
    if (contain_parameters.count(2))
        fileInfo.output_name = _argv[contain_parameters[2] + 1];
    // author
    if (contain_parameters.count(4))
        file_util::writeAuthor(_argv[contain_parameters[4] + 1], _argv[contain_parameters[4] + 2]);
    // classification
    if (contain_parameters.count(6))
        fileInfo.classification = _argv[contain_parameters[6] + 1];
    // index
    if (contain_parameters.count(8))
        file_util::writeIndex();
        // classifications
    else if (contain_parameters.count(10))
        file_util::writeClassifications(classifications);
        // projects
    else if (contain_parameters.count(12))
        file_util::writeProject(_argv[contain_parameters[12] + 1]);
        // about
    else if (contain_parameters.count(14))
        file_util::writeAbout();
    else if (isFile)
        file_util::writeArticles();
}

short console_util::checkParameter(map<int, int> &contain_parameters, vector<string> &classifications) {
    int classification_num = 0, parameter_num = 0, author_num = 0;
    bool parameter = false, classification = false, author = false, has = false, project = false;
    string last_parameter;
    for (int i = 1; i < _argc - 1; i++) {
        LOOP:
        if (i >= _argc - 1)
            break;
        for (int j = 0; j < 18; j++) {
            if (_argv[i] == parameters[j]) {
                if (parameter || author || has || project) {
                    if (parameter_num == 0) {
                        cerr << "\033[31m" << "参数" << last_parameter << "没有值" << "\033[0m";
                        return status.error;
                    }

                    if (author && author_num != 2) {
                        cerr << "\033[31m" << "参数" << last_parameter << "要有两个值" << "\033[0m";
                        return status.error;
                    }
                }
                if (classification && classification_num < 4) {
                    cerr << "\033[31m" << "参数" << last_parameter << "值最少为4个" << "\033[0m";
                    return status.error;
                }

                if (contain_parameters.count(j) == 1) {
                    cerr << "\033[31m" << "参数" << last_parameter << "不可重复" << "\033[0m";
                    return status.error;
                }

                classification = false;
                parameter = false;
                parameter_num = 0;
                classification_num = 0;
                last_parameter = parameters[j];
                contain_parameters.insert(pair<int, int>(j, i));

                if (j % 2 == 0)
                    contain_parameters.insert(pair<int, int>(j + 1, i));
                else
                    contain_parameters.insert(pair<int, int>(j - 1, i));

                if (j == 10 || j == 11)
                    classification = true;
                else if (j == 0 || j == 1)
                    return status.help;
                else if (j == 4 || j == 5)
                    author = true;
                else if (j == 16 || j == 17)
                    has = true;
                else if (j == 12 || j == 13)
                    project = true;
                else if (j < 8)
                    parameter = true;

                i++;
                goto LOOP;
            }
        }

        if (parameter) {
            parameter_num++;
            parameter = false;
            if (last_parameter == "-o" || last_parameter == "--output")
                if (!nameCheck(_argv[i], "博客的标题名"))
                    return status.error;
        } else if (classification) {
            classification_num++;
            if (count(classifications.begin(), classifications.end(), _argv[i])) {
                cerr << "\033[31m" << "分类不可以重复" << "\033[0m";
                return status.error;
            }

            classifications.emplace_back(_argv[i]);
            if (classification_num > 9) {
                cerr << "\033[31m" << "分类最多为9个" << "\033[0m";
                return status.error;
            }
        } else if (project) {
            project = false;
            regex r("https://.{5,}");
            if (!regex_match(_argv[i], r)) {
                cerr << "\033[31m" << last_parameter << "的值必须是网址" << "\033[0m";
                return status.error;
            }
        } else if (has) {
            has = false;
            if (!file_util::isTargetDir(_argv[i])) return status.error;
        } else if (author) {
            author_num++;
            if (author_num == 2)
                author = false;
            else if (!nameCheck(_argv[i], "博主的名字")) return status.error;
        } else {
            cerr << "\033[31m" << "未知参数" << _argv[i] << "\033[0m";
            return status.error;
        }
    }

    for (int j = 0; j < 18; j++) {
        if (_argv[_argc - 1] == parameters[j]) {
            if (parameter || author || has) {
                if (parameter_num == 0) {
                    cerr << "\033[31m" << "参数" << last_parameter << "没有值" << "\033[0m";
                    return status.error;
                }
            }
            if (classification && classification_num < 4) {
                cerr << "\033[31m" << "参数" << last_parameter << "值最少为4个" << "\033[0m";
                return status.error;
            }

            if (contain_parameters.count(j) == 1) {
                cerr << "\033[31m" << "参数" << last_parameter << "不可重复" << "\033[0m";
                return status.error;
            }

            last_parameter = _argv[_argc - 1];
            if (j == 0 || j == 1) {
                return status.help;
            } else if (j == 10 || j == 11) {
                cerr << "\033[31m" << "分类最少为4个" << "\033[0m";
                return status.error;
            } else if (j >= 8 && j <= 15) {
                parameter = true;
                break;
            }

            cerr << "\033[31m" << "参数" << last_parameter << "没有值" << "\033[0m";
            return status.error;
        }
    }

    if (project) {
        regex r("https://.{5,}");
        if (!regex_match(_argv[_argc - 1], r)) {
            cerr << "\033[31m" << last_parameter << "的值必须是网址" << "\033[0m";
            return status.error;
        }
    }

    if (has) {
        if (!file_util::isTargetDir(_argv[_argc - 1])) return status.error;
        if (contain_parameters.count(4) || contain_parameters.count(10))
            if (!mustHaveFile(contain_parameters)) return status.success;
        cerr << "\033[31m" << "未指定文件" << "\033[0m";
        return status.error;
    }

    if (author) {
        if (author_num == 0) {
            if (!nameCheck(_argv[_argc - 1], "博主的名字")) return status.error;
            cerr << "\033[31m" << last_parameter << "未指定链接" << "\033[0m";
            return status.error;
        } else if (author_num == 1) {
            regex r("https://.{5,}");
            if (regex_match(_argv[_argc - 1], r)) {
                if (!mustHaveFile(contain_parameters)) return status.success;
                cerr << "\033[31m" << "未指定文件" << "\033[0m";
                return status.error;
            }
            cerr << "\033[31m" << "指定的链接必须以https://开头" << "\033[0m";
            return status.error;
        }
    }

    if (classification) {
        if (classification_num < 3) {
            cerr << "\033[31m" << "参数" << last_parameter << "值最少为4个" << "\033[0m";
            return status.error;
        } else if (classification_num >= 9) {
            cerr << "\033[31m" << "参数" << last_parameter << "值最多为9个" << "\033[0m";
            return status.error;
        } else if (std::count(classifications.begin(), classifications.end(), _argv[_argc - 1])) {
            cerr << "\033[31m" << "分类不可以重复" << "\033[0m";
            return status.error;
        } else {
            classifications.emplace_back(_argv[_argc - 1]);
            if (contain_parameters.count(2)) {
                cerr << "\033[31m" << "指定了" << _argv[contain_parameters[2]] << "参数，需要指定文件" << "\033[0m";
                return status.error;
            } else if (contain_parameters.count(6)) {
                cerr << "\033[31m" << "指定了" << _argv[contain_parameters[6]] << "参数，需要指定文件" << "\033[0m";
                return status.error;
            } else if (contain_parameters.count(8)) {
                cerr << "\033[31m" << "指定了" << _argv[contain_parameters[8]] << "参数，需要指定文件" << "\033[0m";
                return status.error;
            } else if (contain_parameters.count(12)) {
                cerr << "\033[31m" << "指定了" << _argv[contain_parameters[12]] << "参数，需要指定文件" << "\033[0m";
                return status.error;
            } else if (contain_parameters.count(14)) {
                cerr << "\033[31m" << "指定了" << _argv[contain_parameters[14]] << "参数，需要指定文件" << "\033[0m";
                return status.error;
            }
            if (!mustHaveFile(contain_parameters)) return status.success;
            cerr << "\033[31m" << "未指定文件" << "\033[0m";
            return status.error;
        }

    }

    if (parameter && parameter_num == 0) {
        if (!nameCheck(_argv[_argc - 1], "博客的标题名")) return status.error;
        cerr << "\033[31m" << "未指定文件" << "\033[0m";
        return status.error;
    }

    if (file_util::fileExist(_argv[_argc - 1])) {
        string str(_argv[_argc - 1]);
        regex r(".{1,}\\.md");
        if (!regex_match(str, r)) {
            cerr << "\033[31m" << "文件只能是md格式的" << "\033[0m";
            return status.error;
        }
        console_util::isFile = true;
        return status.success;
    }

    cerr << "\033[31m" << _argv[_argc - 1] << "文件不存在" << "\033[0m";
    return status.error;
}

bool console_util::mustHaveFile(map<int, int> &contain_parameters) {
    if (contain_parameters.count(2) || contain_parameters.count(6) || contain_parameters.count(8) ||
        contain_parameters.count(12) || contain_parameters.count(14))
        return true;
    return false;
}

bool console_util::nameCheck(char *arg, const string &name) {
    string str(arg);
    if (arg[0] == '-') {
        cerr << "\033[31m" << name << "不可以-开头" << "\033[0m";
        return false;
    }
    for (const char &c: str) {
        switch (c) {
            case '&':
                cerr << "\033[31m" << name << "中不可包含&" << "\033[0m";
                return false;
            case '?':
                cerr << "\033[31m" << name << "中不可包含?" << "\033[0m";
                return false;
            case '=':
                cerr << "\033[31m" << name << "中不可包含=" << "\033[0m";
                return false;
            case '.':
                cerr << "\033[31m" << name << "中不可包含." << "\033[0m";
                return false;
            case '/':
                cerr << "\033[31m" << name << "中不可包含/" << "\033[0m";
                return false;
            case '"':
                cerr << "\033[31m" << name << "中不可包含\"" << "\033[0m";
                return false;
            case '\'':
                cerr << "\033[31m" << name << "中不可包含\'" << "\033[0m";
                return false;
            case '!':
                cerr << "\033[31m" << name << "中不可包含!" << "\033[0m";
                return false;
            case '$':
                cerr << "\033[31m" << name << "中不可包含$" << "\033[0m";
                return false;
            case '^':
                cerr << "\033[31m" << name << "中不可包含^" << "\033[0m";
                return false;
            case ';':
                cerr << "\033[31m" << name << "中不可包含;" << "\033[0m";
                return false;
            case '[':
                cerr << "\033[31m" << name << "中不可包含[" << "\033[0m";
                return false;
            case ']':
                cerr << "\033[31m" << name << "中不可包含]" << "\033[0m";
                return false;
            case '{':
                cerr << "\033[31m" << name << "中不可包含}" << "\033[0m";
                return false;
        }
    }
    return true;
}
