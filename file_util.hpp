#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <dirent.h>
#include <io.h>

using namespace std;

#ifndef CL_MDTOHTML_FILE_UTIL_H
#define CL_MDTOHTML_FILE_UTIL_H

struct File_info {
    string dir_path;
    string output_name;
    string classification;
    bool hasDir = true;
} fileInfo;

class file_util {
public:
    static bool fileExist(const string &path) {
        fstream ifs(path, ios::in);
        if (ifs.is_open()) {
            ifs.close();
            return true;
        }
        return false;
    }

    static bool hasDir(const string &path) {
        DIR *pDIR;
        if (!(pDIR = opendir(path.c_str()))) {
            closedir(pDIR);
            return false;
        }
        closedir(pDIR);
        return true;
    }

    static bool isDir(DIR *&pDIR, char *path) {
        if (!(pDIR = opendir(path))) {
            cerr << "\033[31m" << "--has的值不是文件夹" << "\033[0m";
            return false;
        }
        return true;
    }

    static bool isTargetDir(char *path) {
        DIR *pDIR;
        if (!isDir(pDIR, path)) {
            closedir(pDIR);
            return false;
        }
        // 
        string files[] = {"about.html", "articles\\Template.html", "articles.html", "classification.html",
                          "css\\about.css", "css\\articles.css", "css\\classification.css",
                          "css\\fonts\\LXGWWenKai-Bold.ttf", "css\\fonts\\LXGWWenKai-Regular.ttf",
                          "css\\fonts\\LXGWWenKaiMono-Bold.ttf", "css\\fonts\\LXGWWenKaiMono-Regular.ttf",
                          "css\\general.css", "css\\index.css", "css\\page.css", "css\\projects.css",
                          "highlight\\DIGESTS.md", "highlight\\es\\core.js", "highlight\\es\\core.min.js",
                          "highlight\\es\\highlight.js", "highlight\\es\\highlight.min.js",
                          "highlight\\es\\package.json", "highlight\\highlight.js", "highlight\\highlight.min.js",
                          "highlight\\languages\\armasm.min.js", "highlight\\languages\\bash.min.js",
                          "highlight\\languages\\c.min.js", "highlight\\languages\\cpp.min.js",
                          "highlight\\languages\\csharp.min.js", "highlight\\languages\\css.min.js",
                          "highlight\\languages\\go.min.js", "highlight\\languages\\http.min.js",
                          "highlight\\languages\\java.min.js", "highlight\\languages\\javascript.min.js",
                          "highlight\\languages\\json.min.js", "highlight\\languages\\kotlin.min.js",
                          "highlight\\languages\\lua.min.js", "highlight\\languages\\markdown.min.js",
                          "highlight\\languages\\nginx.min.js", "highlight\\languages\\objectivec.min.js",
                          "highlight\\languages\\php.min.js", "highlight\\languages\\python.min.js",
                          "highlight\\languages\\rust.min.js", "highlight\\languages\\shell.min.js",
                          "highlight\\languages\\sql.min.js", "highlight\\languages\\x86asm.min.js",
                          "highlight\\languages\\xml.min.js", "highlight\\package.json",
                          "highlight\\styles\\atom-one-dark-reasonable.min.css", "image\\about.png",
                          "image\\about_over.png", "image\\articles.png", "image\\articles_over.png",
                          "image\\classification.png", "image\\classification_over.png",
                          "image\\more.png", "image\\more_over.png", "image\\projects.png",
                          "image\\projects_over.png", "image\\search.png", "image\\search_over.png",
                          "image\\top.png", "image\\top_over.png", "index.html", "js\\about.js",
                          "js\\articles.js", "js\\classification.js", "js\\file.js", "js\\general.js",
                          "js\\html2canvas.js", "js\\index.js", "js\\info.js", "js\\page.js",
                          "js\\projects.js", "js\\project_to.js", "projects.html",
                          "template\\about.html", "template\\index.html", "template\\projects.html",
                          "template\\template.html"
        };
        string path_str(path);
        if (*(path_str.end() - 1) != '\\') {
            path_str.push_back('\\');
        }

        for (string &file: files) {
            if (!fileExist(path_str + file)) {
                cerr << "\033[31m" << path_str << "中没有" << file << "\033[0m";
                return false;
            }
        }

        closedir(pDIR);
        return true;
    }

    static void writeAuthor(const string &author, const string &url) {
        // var author = "Acc";
        // var home_url = "https://github.com/Ascmee";
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\info.js";

        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();

        ofstream ofs(project_path, ios::out);
        for (const string &line: lines) {
            if (line.find("var author = \"") != -1) {
                ofs << "var author = \"" << author << "\";" << endl;
            } else if (line.find("var home_url = \"") != -1) {
                ofs << "var home_url = \"" << url << "\";" << endl;
            } else
                ofs << line << endl;
        }
        ofs.close();
    }

    static void writeIndex() {

    }

    static void writeClassifications(vector<string> &classifications) {
        // var classification_arr = ["Java", "C/C++", "Asm", "Python", "Web", "Data", "C#", "Linux", "Other"];
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\file.js";

        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();
        ofstream ofs(project_path, ios::out);
        int i = 0;
        for (const string &line: lines) {
            if (line.find("var classification_arr = [") != -1) {
                ofs << "var classification_arr = [";
                for (const string &classification: classifications) {
                    i++;
                    ofs << "\"" << classification << "\"";
                    if (i != classifications.size())
                        ofs << ", ";
                    else
                        ofs << "];";
                }
            } else
                ofs << line << endl;
        }
        if (i == 0) {
            ofs << "var classification_arr = [";
            for (const string &classification: classifications) {
                i++;
                ofs << "\"" << classification << "\"";
                if (i != classifications.size())
                    ofs << ", ";
                else
                    ofs << "];";
            }
        }
        ofs.close();
    }

    static void writeProject(const string &url) {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\project_to.js";

        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();

        ofstream ofs(project_path, ios::out);
        for (const string &line: lines) {
            if (line.find("];") != -1) {

            }
            ofs << line << endl;
        }
        ofs.close();
    }

    static void writeAbout() {}

    static void writeArticles() {}

    static void readLines(ifstream &ifs, vector<string> &lines) {
        string str;
        char c;
        while ((c = (char) ifs.get()) != -1) {
            if (c == '\n') {
                lines.push_back(str);
                str.clear();
                continue;
            }
            str.push_back(c);
        }
        lines.push_back(str);
    }

    static void writeTo(const string &path, const string &str) {
        ofstream ofs(path, ios::out);
        ofs << str;
        ofs.close();
    }

    static string readFrom(const string &path) {
        vector<string> lines;
        ifstream ifs(path, ios::in);
        readLines(ifs, lines);
        string result;
        result = lines[0];
        if (lines.size() > 1) {
            for (int i = 1; i < lines.size(); i++) {
                result += " " + lines[i];
            }
        }
        ifs.close();
        return result;
    }
};


#endif //CL_MDTOHTML_FILE_UTIL_H