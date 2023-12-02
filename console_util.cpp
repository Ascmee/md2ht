#include "console_util.h"

void console_util::init(int argc, char** argv){
    _argc = argc;
    _argv = argv;
}

void console_util::matchParameter(){
    if(_argc == 1){
        printHelp();
        return;
    }else {

    }
}

void console_util::printHelp(){
    cout << "Usage: md2hm [options] file\n"
            "Options:\n"
            "\t-h --help\t\t��ʾ������Ϣ\n"
            "\t-o --output\t\t���µı��⣬û�����룬��Ĭ���ļ���\n"
            "\t-a --author\t\t��ʾ�Ĳ������֣���һ��ʹ��ʱ��Ҫָ�����������Բ�ָ��\n"
            "\t-c --classify\t\t���������ķ��࣬ȷ����Ӧ�ķ�����ڣ����򽫲���Ϊ�����\n"
            "\t-i --index\t\tָ��������Ϊindex.html������ʾ�����ݣ����ǲ����Լ��޸ģ�����ָ����ÿ�ζ��Ḳ��ԭ�ļ�����ʱ������-o����\n"
            "\t-ab --about\t\tָ��������Ϊabout.html����ʾ�����ݣ������Լ������޸ģ�����ָ����ÿ�ζ��Ḳ��ԭ�ļ�����ʱ������-o����\n"
            "\t-cs --classifications\t\tָ�������е����ͣ����������12��\n"
            "\t-h --has\t\tָ���Ѿ����ڵĲ����ļ��е�·��������û��ָ��������½�����\n"
            "\n"
            "�ó������Ҫ���а�װgit�Ļ�����ʹ�ã��Կ�¡��Ӧ�Ĳ���ģ�壬�Ӷ������޸�\n"
            "������ҳ�潫���ڱ����ڶ�Ӧ��ָ�����ļ����ж�Ӧ��λ�ã����û��������ϴ����Լ��ķ�����������й�ƽ̨"
            "���ǽ�markdownת��Ϊ�˲���ģ�������html�ļ������ģ���������ѧ�ղ��������Կ��ܻ��д��󣬿������\n"
            "������������Դ������ģ��Ҳ�ǿ�Դ�ģ���������github�ϲ鿴��";
}