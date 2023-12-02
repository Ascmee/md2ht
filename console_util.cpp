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
            "\t-h --help\t\t显示帮助信息\n"
            "\t-o --output\t\t文章的标题，没有输入，就默认文件名\n"
            "\t-a --author\t\t显示的博主名字，第一次使用时需要指定，后续可以不指定\n"
            "\t-c --classify\t\t文章所属的分类，确保对应的分类存在，否则将不会为其分类\n"
            "\t-i --index\t\t指定该文章为index.html中所显示的内容，若是不会自己修改，可以指定，每次都会覆盖原文件，此时会无视-o参数\n"
            "\t-ab --about\t\t指定该文章为about.html中显示的内容，若是自己不会修改，可以指定，每次都会覆盖原文件，此时会无视-o参数\n"
            "\t-cs --classifications\t\t指定分类中的类型，最多可以添加12个\n"
            "\t-h --has\t\t指定已经存在的博客文件夹的路径，若是没有指定，则会新建博客\n"
            "\n"
            "该程序必须要在有安装git的环境下使用，以克隆对应的博客模板，从而进行修改\n"
            "产生的页面将会在保存在对应的指定的文件夹中对应的位置，由用户将博客上传到自己的服务器或代码托管平台"
            "这是将markdown转化为此博客模板所需的html文件而做的，由于我们学艺不精，所以可能会有错误，可以提出\n"
            "声明：本程序开源，博客模板也是开源的，均可以在github上查看到";
}