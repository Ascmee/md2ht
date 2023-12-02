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
            "        -h  --help              ��ʾ������Ϣ\n"
            "        -o  --output            ���µı��⣬û�����룬��Ĭ���ļ���\n"
            "        -a  --author            ��ʾ�Ĳ������֣���һ��ʹ��ʱ��Ҫָ�����������Բ�ָ��\n"
            "        -c  --classify          ���������ķ��࣬ȷ����Ӧ�ķ�����ڣ����򽫲���Ϊ�����\n"
            "        -in --index             ָ��������Ϊindex.html������ʾ�����ݣ��������ݴ�ֱ���У���ʱ������-o����\n"
            "        -cs --classifications   ָ�������е����ͣ�����4�������9�������һ��ΪĬ�Ϸ��࣬��һ�α���ָ��\n"
            "        -pr --project           ָ�����ļ�Ϊ��Ŀ���ļ����ڲ���������Ҫ������Ŀ������\n"
            "        -ab --about             ָ��������Ϊabout.html����ʾ�����ݣ���ʱ������-o����\n"
            "        -h  --has               ָ���Ѿ����ڵĲ����ļ��е�·��������û��ָ��������½�����\n"
            "\n"
            "�ó������Ҫ���а�װgit�Ļ�����ʹ�ã��Կ�¡��Ӧ�Ĳ���ģ�壬�Ӷ������޸�\n"
            "������ҳ�潫���ڱ����ڶ�Ӧ��ָ�����ļ����ж�Ӧ��λ�ã����û��������ϴ����Լ��ķ�����������й�ƽ̨���ǽ�markdown ת��Ϊ�˲���ģ�������html�ļ������ģ���������ѧ�ղ��������Կ��ܻ��д���������ԣ������������\n"
            "������������Դ������ģ��Ҳ�ǿ�Դ�ģ���������github�ϲ鿴��";
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
            // �Է���һ������������޸�
            exit(-1);
        }
        // ��������Ľ�����ִ��
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
        cerr << "\033[31m" << "�ļ������ڣ�" << _argv[_argc - 1] << "\033[0m";
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
                    cerr << "\033[31m" << "�ظ�������" << _argv[i] << "\033[0m";
                    return status.error;
                }
                if(classification && classification_num < 4){
                    cerr << "\033[31m" << "��������Ϊ4��" << "\033[0m";
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
                    cerr << "\033[31m" << "�������Ϊ9��" << "\033[0m";
                    return status.error;
                }
                break;
            } else if(j == 17){
                cerr << "\033[31m" << "δ֪���" << _argv[i] << "\033[0m";
                return status.error;
            }
        }
        if(contain_parameter)
            contain_parameter = false;
    }
    if(classification && classification_num < 4){
        cerr << "\033[31m" << "��������Ϊ4��" << "\033[0m";
        return status.error;
    }
    return status.success;
}

