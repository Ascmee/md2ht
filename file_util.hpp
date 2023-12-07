#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <dirent.h>
#include <regex>
#include <io.h>
#include <windows.h>
#include "data_convert.h"
#include "name_arr.h"

using namespace std;

#ifndef CL_MDTOHTML_FILE_UTIL_H
#define CL_MDTOHTML_FILE_UTIL_H

struct File_info {
    string dir_path;
    string output_name;
    string classification;
    string md_path;
    bool hasDir = true;
} fileInfo;

string nowH1 = "-";
string nowH2 = "--";
string nowH3 = "---";
string nowH4 = "----";
string nowH5 = "-----";

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
        string files[] = {"css\\about.css", "css\\articles.css", "css\\classification.css",
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
                          "image\\top.png", "image\\top_over.png", "js\\about.js",
                          "js\\articles.js", "js\\classification.js", "js\\file.js", "js\\general.js",
                          "js\\html2canvas.js", "js\\index.js", "js\\info.js", "js\\page.js",
                          "js\\projects.js", "js\\project_to.js", "template\\about.html",
                          "template\\index.html", "template\\projects.html", "template\\template.html"
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
                ofs << "var author = \"" << changeToUTF8(author) << "\";" << endl;
            } else if (line.find("var home_url = \"") != -1) {
                ofs << "var home_url = \"" << url << "\";" << endl;
            } else
                ofs << line << endl;
        }
        ofs.close();
    }

    static void writeIndex() {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "index.html";
        string project_read_path = "template\\index.html";
        parseMd(project_path, project_read_path, true);
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
                    ofs << "\"" << changeToUTF8(classification) << "\"";
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
                ofs << "\"" << changeToUTF8(classification) << "\"";
                if (i != classifications.size())
                    ofs << ", ";
                else
                    ofs << "];";
            }
        }
        ofs.close();
    }

    static void writeProject(const string &url) {
        // { date: "2023/09/13/09:31:33", title: "贪吃蛇——Java实现", href: "https://www.github.com" },
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\project_to.js";

        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();
        ofstream ofs(project_path, ios::out);
        name_arr na;
        string n;
        regex r("\\s{0,}var\\s{0,}project_name_arr\\s{0,}=\\s{0,}\\[.{0,}\\];\\s{0,}");
        bool written = false;
        if (lines[0].find("var project_name_arr = [];") != -1) {
            ofs << "var project_name_arr = [" << endl << "  { date: \"" << get_time(na) << "\", title: \""
                << changeToUTF8(fileInfo.output_name)
                << "\", href: \"" << url << "\" }," << endl << "];";
        } else if (!regex_match(lines[0], r)) {
            for (const string &line: lines) {
                if (line.find("title: \"") != -1) {
                    n = line.substr(line.find("title: \"") + 8);
                    n = n.substr(0, n.find('\"'));
                } else n = line;
                if (n == changeToUTF8(fileInfo.output_name)) {
                    written = true;
                    ofs << "  { date: \"" << get_time(na) << "\", title: \"" << changeToUTF8(fileInfo.output_name)
                        << "\", href: \"" << url << "\" }," << endl;
                    continue;
                }
                if (!written && line.find("];") != -1)
                    ofs << "  { date: \"" << get_time(na) << "\", title: \"" << changeToUTF8(fileInfo.output_name)
                        << "\", href: \"" << url << "\" }," << endl << "];";
                else
                    ofs << line << endl;
            }
        } else {
            int begin = lines[0].find('[');
            ofs << "var project_name_arr = [" << endl << "  "
                << lines[0].substr(begin + 1, lines[0].find_last_of('}') - begin)
                << "," << endl << "  { date: \"" << get_time(na) << "\", title: \""
                << changeToUTF8(fileInfo.output_name)
                << "\", href: \"" << url << "\" }," << endl << "];";
        }
        ofs.close();
        writeArticles();
    }

    static void writeAbout() {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "about.html";
        string project_read_path = "template\\about.html";
        parseMd(project_path, project_read_path, true);
    }

    static void writeArticles() {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string command = "mkdir " + fileInfo.dir_path + "articles";
        if (!hasDir(fileInfo.dir_path + "articles"))
            system(command.c_str());
        string project_path = fileInfo.dir_path + "articles\\" + fileInfo.output_name + ".html";
        modifiedArticles();
        parseMd(project_path);
    }

    static void removeFile() {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\file.js";
        string n;
        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();
        ofstream ofs(project_path, ios::out);
        for (const string &line: lines) {
            if (line.find("title: \"") != -1) {
                n = line.substr(line.find("title: \"") + 8);
                n = n.substr(0, n.find('\"'));
            } else n = line;
            if (n != changeToUTF8(fileInfo.output_name))
                ofs << line << endl;
        }
        ofs.close();
        project_path = fileInfo.dir_path + "js\\project_to.js";

        lines.clear();
        ifstream iifs(project_path, ios::in);
        readLines(iifs, lines);
        iifs.close();
        ofstream oofs(project_path, ios::out);
        for (const string &line: lines) {
            if (line.find("title: \"") != -1) {
                n = line.substr(line.find("title: \"") + 8);
                n = n.substr(0, n.find('\"'));
            } else n = line;
            if (n != changeToUTF8(fileInfo.output_name))
                oofs << line << endl;
        }
        oofs.close();
        string fileP = fileInfo.dir_path + "articles\\" + fileInfo.output_name + ".html";
        remove(fileP.c_str());
        
        string dir_picture = fileInfo.dir_path + "image\\" + fileInfo.output_name;
        if(hasDir(dir_picture)) {
            string p = "del /Q /F " + dir_picture;
            system(p.c_str());
            p = "rmdir " + dir_picture;
            system(p.c_str());
        }
        cout << "文件已删除" << endl;
    }

    static void modifiedArticles() {
        // 获取默认分类
        //  { date: "2022/01/09/09:31:33", title: "C++的学习之路2", classification: "C/C++" },
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\file.js";

        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();
        ofstream ofs(project_path, ios::out);
        name_arr na;
        regex r("\\s{0,}var\\s{0,}name_arr\\s{0,}=\\s{0,}\\[.{0,}\\];\\s{0,}");
        string n;
        bool written = false;
        if (lines[0].find("var name_arr = [];") != -1) {
            ofs << "var name_arr = [" << endl << "  { date: \"" << get_time(na) << "\", title: \""
                << changeToUTF8(fileInfo.output_name) << "\", classification: \""
                << changeToUTF8(fileInfo.classification) << "\" }," << endl
                << "];" << endl;
            for (int i = 1; i < lines.size(); i++) {
                ofs << lines[i] << endl;
            }
        } else if (!regex_match(lines[0], r)) {
            for (const string &line: lines) {
                if (line.find("title: \"") != -1) {
                    n = line.substr(line.find("title: \"") + 8);
                    n = n.substr(0, n.find('\"'));
                } else n = line;
                if (n == changeToUTF8(fileInfo.output_name)) {
                    written = true;
                    ofs << "  { date: \"" << get_time(na) << "\", title: \""
                        << changeToUTF8(fileInfo.output_name) << "\", classification: \""
                        << changeToUTF8(fileInfo.classification) << "\" },"
                        << endl;
                    continue;
                }
                if (!written && line.find("];") != -1 && line.find("var classification_arr = [") == -1)
                    ofs << "  { date: \"" << get_time(na) << "\", title: \""
                        << changeToUTF8(fileInfo.output_name) << "\", classification: \""
                        << changeToUTF8(fileInfo.classification) << "\" },"
                        << endl << "];" << endl;

                else if (line == *(lines.end() - 1))
                    ofs << line;
                else
                    ofs << line << endl;
            }
        } else {
            int begin = lines[0].find('[');
            ofs << "var name_arr = [" << endl << "  " << lines[0].substr(begin + 1, lines[0].find_last_of('}') - begin)
                << "," << endl << "  { date: \"" << get_time(na) << "\", title: \""
                << changeToUTF8(fileInfo.output_name) << "\", classification: \""
                << changeToUTF8(fileInfo.classification) << "\" }," << endl
                << "];" << endl;
            for (int i = 1; i < lines.size(); i++) {
                ofs << lines[i] << endl;
            }
        }
        ofs.close();
    }

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
        ifs.close();
        return lines[0];
    }

    static void
    parseMd(const string &filepath, const string &readPath = "template\\template.html", bool root_is = false) {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        ifstream ifs(fileInfo.dir_path + readPath);
        vector<string> htmllines;
        regex startHtml("\\s{0,}<!--\\scontent\\sstart\\s-->\\s{0,}");
        regex startTime("\\s{0,}<!--\\scontent\\sdate\\sstart\\s-->\\s{0,}");
        regex startTitle("\\s{0,}<!--\\scontent\\stitle\\sstart\\s-->\\s{0,}");
        readLines(ifs, htmllines);
        ifs.close();
        ofstream ofs(filepath);
        ifstream iifs(fileInfo.md_path);
        vector<string> lines;
        readLines(iifs, lines);
        iifs.close();
        for (const string &line: htmllines) {
            ofs << line << endl;
            if (regex_match(line, startHtml)) {
                writeLines(lines, ofs, root_is);
            } else if (regex_match(line, startTime)) {
                name_arr na;
                string t = get_time(na);
                ofs << t.substr(0, t.find_last_of('/')) << endl;
            } else if (regex_match(line, startTitle)) {
                ofs << changeToUTF8(fileInfo.output_name) << endl;
            }
        }
        ofs.close();
    }

    static string getDefaultClassification() {
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string project_path = fileInfo.dir_path + "js\\file.js";

        vector<string> lines;
        ifstream ifs(project_path, ios::in);
        readLines(ifs, lines);
        ifs.close();
        for (string &line: lines) {
            if (line.find("var classification_arr = [") != -1) {
                if (line.find("var classification_arr = [];") != -1) {
                    cerr << "\033[31m" << "请设置分类" << "\033[0m";
                    exit(-1);
                }
                line = line.substr(line.find_last_of(',') + 3, line.size() - line.find_last_of(',') - 6);
                return line;
            }
        }
        cerr << "\033[31m" << "请设置分类" << "\033[0m";
        exit(-1);
    }

    static bool isEscapesUse(const char &c) {
        switch (c) {
            case '<':
            case '>':
            case '.':
            case '\'':
            case '\"':
            case ':':
            case '|':
            case '[':
            case ']':
            case '{':
            case '}':
            case '-':
            case '_':
            case '+':
            case '=':
            case '(':
            case ')':
            case '*':
            case '&':
            case '^':
            case '$':
            case '#':
            case '!':
            case '~':
            case '`':
                return true;
            default:
                return false;
        }
        return false;
    }

    static string deleteEscapes(const string &line, vector<char> &pop_parameter) {
        string s = line;
        for (int i = 0; i < s.size() - 1; i++) {
            if (s[i] == '\\') {
                pop_parameter.push_back(s[i + 1]);
                s = s.substr(0, i + 1) + s.substr(i + 2);
            }
        }
        return s;
    }

    static string recoverEscapes(const string &line, vector<char> &pop_parameter, int index = 0, int begin = -1,
                                 bool delete_parameter = false, bool delete_is = true) {
        string result = line;
        int nownum = 0;
        if (begin == -1) {
            begin = line.size() - 1;
        }
        vector<char>::iterator it = pop_parameter.end() - 1;

        for (int i = line.size() - 1; i >= index; i--) {
            if (line[i] == '\\') {
                if (i <= begin) {
                    if (delete_is && isEscapesUse(*it)) result[i] = *it;
                    else result.insert(result.begin() + i + 1, *it);
                    if (delete_parameter) {
                        pop_parameter.erase(it);
                        it = pop_parameter.end() - nownum - 1;
                    } else it--;
                } else {
                    nownum++;
                }
            }
        }
        return result;
    }

    static bool findRegex(int pos, const string &line, const string &reg) {
        if (line.substr(pos, reg.size()) != reg) return false;
        return true;
    }

    static void
    copyImage(string &line, map<int, string>::iterator &mkit, vector<char> &pop_parameter, bool root_is) {
        string temp = line;
        int i3 = mkit->first, i2 = (--mkit)->first, i1 = (--mkit)->first, num = 0;
        for (int i = i1; i <= i3; i++) {
            if (line[i] == '\\') num++;
        }
        temp = recoverEscapes(temp, pop_parameter, i1, i3, true);
        string alt = temp.substr(i1 + 2, i2 - i1 - 2);
        string src = temp.substr(i2 + 2, i3 - i2 - 2 + num);

        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        string t = src.substr(src.find_last_of('\\') + 1);
        string st = src;
        if (src.find(':') == -1) {
            string path_img = fileInfo.md_path;
            st = path_img.substr(0, path_img.find_last_of('\\') + 1) + st;
            src = path_img.substr(0, path_img.find_last_of('\\') + 1) + changeToGBK(src);
            for (char &c: src) {
                if (c == '/') {
                    c = '\\';
                }
            }
            for (char &c: st) {
                if (c == '/') {
                    c = '\\';
                }
            }
        }
        ifstream ifs(src, ios::in | ios::binary);
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        for (char &c: t) {
            if (c == '/') {
                c = '\\';
            }
        }
        string lp = "mkdir " + fileInfo.dir_path + "image\\" + fileInfo.output_name;
        if (!hasDir(fileInfo.dir_path + "image\\" + fileInfo.output_name))
            system(lp.c_str());
        string des = fileInfo.dir_path + "image\\" + fileInfo.output_name + "\\" +
                     changeToGBK(t.substr(t.find_last_of('\\') + 1));
        ofstream ofs(des, ios::out | ios::binary);
        int i;
        if (ifs.is_open()) {
            while ((i = ifs.get()) != -1) {
                ofs.put(i);
            }
        } else {
            cout << "waring: " << src << "不存在，图片无法加载" << endl;
        }
        ifs.close();
        ofs.close();
        if (root_is)
            des = "image/" + changeToUTF8(fileInfo.output_name) + "/" + st.substr(st.find_last_of('\\') + 1);
        else
            des = "../image/" + changeToUTF8(fileInfo.output_name) + "/" + st.substr(st.find_last_of('\\') + 1);
        line = line.substr(0, i1) + "</p><img src=\"" + des + "\" alt=\"" + alt + "\" /><p>" + line.substr(i3 + 1);
        mkit++;
        mkit++;
    }

    static void
    copyImageAsImg(string &line, map<int, string>::iterator &mkit, vector<char> &pop_parameter, bool root_is) {
        string temp = line;
        bool is_begin = false;
        int i2 = mkit->first, i1 = (--mkit)->first, num = 0, beginN = -1, endN = -1;
        for (int i = i1; i < i2; i++) {
            if (!is_begin && temp.substr(i, 5) == "src=\"") {
                beginN = i + 5;
                i += 5;
                is_begin = true;
            } else if (temp[i] == '\"') {
                endN = i - 1;
                break;
            }
        }

        for (int i = beginN; i <= endN; i++) {
            if (line[i] == '\\') num++;
        }
        temp = recoverEscapes(temp, pop_parameter, beginN, endN, true);

        string src = temp.substr(beginN, endN - beginN + num + 1);

        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');

        string t = src.substr(src.find_last_of('\\') + 1);
        string st = src;
        if (src.find(':') == -1) {
            string path_img = fileInfo.md_path;
            st = path_img.substr(0, path_img.find_last_of('\\') + 1) + st;
            src = path_img.substr(0, path_img.find_last_of('\\') + 1) + changeToGBK(src);
            for (char &c: src) {
                if (c == '/') {
                    c = '\\';
                }
            }
            for (char &c: st) {
                if (c == '/') {
                    c = '\\';
                }
            }
        }
        ifstream ifs(src, ios::in | ios::binary);
        if (*(fileInfo.dir_path.end() - 1) != '\\')
            fileInfo.dir_path.push_back('\\');
        for (char &c: t) {
            if (c == '/') {
                c = '\\';
            }
        }
        string lp = "mkdir " + fileInfo.dir_path + "image\\" + fileInfo.output_name;
        if (!hasDir(fileInfo.dir_path + "image\\" + fileInfo.output_name))
            system(lp.c_str());
        string des = fileInfo.dir_path + "image\\" + fileInfo.output_name + "\\" +
                     changeToGBK(t.substr(t.find_last_of('\\') + 1));
        ofstream ofs(des, ios::out | ios::binary);
        int i;
        if (ifs.is_open()) {
            while ((i = ifs.get()) != -1) {
                ofs.put(i);
            }
        } else {
            cout << "waring: " << src << "不存在，图片无法加载" << endl;
        }
        ifs.close();
        ofs.close();
        if (root_is)
            des = "image/" + changeToUTF8(fileInfo.output_name) + "/" + st.substr(st.find_last_of('\\') + 1);
        else
            des = "../image/" + changeToUTF8(fileInfo.output_name) + "/" +
                  st.substr(st.find_last_of('\\') + 1);// "1 2"3 4/5>6
        line = line.substr(0, i1) + "</p>" + line.substr(i1, beginN - i1) + des +
               line.substr(endN + 1, i2 - endN + 1) + "<p>" + line.substr(i2 + 2);
        mkit++;
        mkit++;
    }

    static void writeLink(string &line, map<int, string>::iterator &mkit) {
        int i3 = mkit->first, i2 = (--mkit)->first, i1 = (--mkit)->first;
        string alt = line.substr(i1 + 1, i2 - i1 - 1);
        string src = line.substr(i2 + 2, i3 - i2 - 2);
        line = line.substr(0, i1) + "<a href=\"" + src + "\">" + alt + "</a>" + line.substr(i3 + 1);
        mkit++;
        mkit++;
    }

    static void
    getMarkdownKeywords(const string &line, map<int, string> &markdown_keywords, vector<int> &l, map<int, int> &r) {
        int len = line.size();
        map<string, int> markdown_contains;
        char c;
        int em_b = -1;
        int strong_b = -1;
        int strong_em_b = -1;
        int mark_b = -1;
        int math_b = -1;
        int sup_b = -1;
        int sub_b = -1;
        int del_b = -1;
        int u_b = -1;
        int code_b = -1;
        int center_b = -1;
        regex img("<img\\ssrc=\".{0,}\"\\salt=\".{0,}\"\\s.{0,}\\s{0,}/>");
        for (int i = 0; i < len; i++) {
            c = line[i];
            if (findRegex(i, line, "***")) {
                if (strong_em_b == -1) {
                    strong_em_b = i;
                } else {
                    markdown_keywords[strong_em_b] = "***";
                    markdown_keywords[i] = "e***";
                    strong_em_b = -1;
                }
                i += 2;
            } else if (findRegex(i, line, "**")) {
                if (strong_b == -1) {
                    strong_b = i;
                } else {
                    markdown_keywords[strong_b] = "**";
                    markdown_keywords[i] = "e**";
                    strong_b = -1;
                }
                i += 1;
            } else if (findRegex(i, line, "*")) {
                if (em_b == -1) {
                    em_b = i;
                } else {
                    markdown_keywords[em_b] = "*";
                    markdown_keywords[i] = "e*";
                    em_b = -1;
                }
            } else if (findRegex(i, line, "==")) {
                if (mark_b == -1) {
                    mark_b = i;
                } else {
                    markdown_keywords[mark_b] = "==";
                    markdown_keywords[i] = "e==";
                    mark_b = -1;
                }
                i += 1;
            } else if (findRegex(i, line, "~~")) {
                if (del_b == -1) {
                    del_b = i;
                } else {
                    markdown_keywords[del_b] = "~~";
                    markdown_keywords[i] = "e~~";
                    del_b = -1;
                }
                i += 1;
            } else if (i < len - 5 && c == '!' && line[i + 1] == '[') {
                for (int j = i + 1; j < len - 3; j++)
                    if (line[j] == ']' && line[j + 1] == '(')
                        for (int m = j + 1; m < len; m++)
                            if (line[m] == ')') {
                                markdown_keywords[i] = "![";
                                markdown_keywords[j] = "](";
                                markdown_keywords[m] = ")";
                                j = m = len;
                            }
            } else if (findRegex(i, line, "^")) {
                if (sup_b == -1) {
                    sup_b = i;
                } else {
                    markdown_keywords[sup_b] = "^";
                    markdown_keywords[i] = "e^";
                    sup_b = -1;
                }
            } else if (findRegex(i, line, "~")) {
                if (sub_b == -1) {
                    sub_b = i;
                } else {
                    markdown_keywords[sub_b] = "~";
                    markdown_keywords[i] = "e~";
                    sub_b = -1;
                }
            } else if (findRegex(i, line, "`")) {
                if (code_b == -1) {
                    code_b = i;
                } else {
                    markdown_keywords[code_b] = "`";
                    markdown_keywords[i] = "e`";
                    code_b = -1;
                }
            } else if ((i == 0 && c == '[') || (i < len - 3 && c == '[' && line[i - 1] != '!')) {
                for (int j = i + 1; j < len - 3; j++)
                    if (line[j] == ']' && line[j + 1] == '(')
                        for (int m = j + 1; m < len; m++)
                            if (line[m] == ')') {
                                markdown_keywords[i] = "[";
                                markdown_keywords[j] = "](";
                                markdown_keywords[m] = ")";
                                j = m = len;
                            }
            } else if (findRegex(i, line, "<u>")) {
                u_b = i;
                i += 2;
            } else if (findRegex(i, line, "</u>")) {
                if (u_b != -1) {
                    markdown_keywords[u_b] = "<u>";
                    l.push_back(u_b);
                    r[u_b + 2] = 2;
                    markdown_keywords[i] = "</u>";
                    l.push_back(i);
                    r[i + 3] = 3;
                    u_b = -1;
                }
                i += 3;
            } else if (findRegex(i, line, "<center>")) {
                center_b = i;
                i += 7;
            } else if (findRegex(i, line, "</center>")) {
                if (center_b != -1) {
                    markdown_keywords[center_b] = "<center>";
                    l.push_back(center_b);
                    r[center_b + 7] = 7;
                    markdown_keywords[i] = "</center>";
                    l.push_back(i);
                    r[i + 8] = 8;
                    center_b = -1;
                }
                i += 8;
            } else if (line.substr(i, 5) == "<font") {
                for (int j = i + 1; j < len - 7; j++)
                    if (line[j] == '>')
                        for (int m = j + 1; m < len - 6; m++)
                            if (line.substr(m, 7) == "</font>") {
                                markdown_keywords[i] = "<font";
                                markdown_keywords[j] = ">";
                                markdown_keywords[m] = "</font>";
                                l.push_back(i);
                                r[j] = 0;
                                l.push_back(m);
                                r[m + 6] = 6;
                                j = m = len;
                            }
            } else if (line.substr(i, 4) == "<img") {
                for (int j = i + 1; j < len - 1; j++)
                    if (line[j] == '/' && line[j + 1] == '>') {
                        if (regex_match(line.substr(i, j - i + 2), img)) {
                            markdown_keywords[i] = "<img";
                            markdown_keywords[j] = "/>";
                            l.push_back(i);
                            r[j + 1] = 1;
                        }
                        j = len;
                    }
            }
        }

    }

    static void
    selectMarkdown(string &line, int pos, map<int, string> &markdown_keywords, vector<int> &l, map<int, int> &r,
                   vector<int>::iterator &lit,
                   map<int, int>::iterator &rit,
                   map<int, string>::iterator &mkit, vector<char> &pop_parameter, bool root_is) {
        for (int i = pos; i >= 0; i--) {
            if (!l.empty() && *lit <= mkit->first && lit != l.begin()) {
                lit--;
                rit--;
            }
            if (!markdown_keywords.empty() && i == mkit->first) {
                if (mkit->second == "*") {
                    line = line.substr(0, i) + "<em>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e*") {
                    line = line.substr(0, i) + "</em>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "**") {
                    line = line.substr(0, i) + "<strong>" + line.substr(i + 2);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e**") {
                    line = line.substr(0, i) + "</strong>" + line.substr(i + 2);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "***") {
                    line = line.substr(0, i) + "<strong><em>" + line.substr(i + 3);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e***") {
                    line = line.substr(0, i) + "</em></strong>" + line.substr(i + 3);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "==") {
                    line = line.substr(0, i) + "<mark>" + line.substr(i + 2);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e==") {
                    line = line.substr(0, i) + "</mark>" + line.substr(i + 2);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "~~") {
                    line = line.substr(0, i) + "<del>" + line.substr(i + 2);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e~~") {
                    line = line.substr(0, i) + "</del>" + line.substr(i + 2);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "`") {
                    line = line.substr(0, i) + "<code class=\"content-inline-code-class\">" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e`") {
                    line = line.substr(0, i) + "</code>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "^") {
                    line = line.substr(0, i) + "<sup>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e^") {
                    line = line.substr(0, i) + "</sup>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "~") {
                    line = line.substr(0, i) + "<sub>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "e~") {
                    line = line.substr(0, i) + "</sub>" + line.substr(i + 1);
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "<center>" || mkit->second == "</center>" || mkit->second == "<u>" ||
                           mkit->second == "</u>") {
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "[") {
                    mkit++;
                    if (mkit->second == "](") {
                        mkit++;
                        if (mkit->second == ")")
                            writeLink(line, mkit);
                        mkit--;
                    }
                    mkit--;
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "![") {
                    mkit++;
                    if (mkit->second == "](") {
                        mkit++;
                        if (mkit->second == ")")
                            copyImage(line, mkit, pop_parameter, root_is);
                        mkit--;
                    }
                    mkit--;
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else if (mkit->second == "<img") {
                    mkit++;
                    if (mkit->second == "/>") {
                        copyImageAsImg(line, mkit, pop_parameter, root_is);
                    }
                    mkit--;
                    mkit--;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                } else {
                    --mkit;
                    selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                }
                if (mkit == markdown_keywords.begin())
                    return;
                break;
            } else if (line[i] == '<' && (l.empty() || (markdown_keywords[i] != "<center>" &&
                                                        markdown_keywords[i] != "</center>" &&
                                                        markdown_keywords[i] != "<u>" &&
                                                        markdown_keywords[i] != "</u>" &&
                                                        markdown_keywords[i] != "<font" &&
                                                        markdown_keywords[i] != "<img" &&
                                                        markdown_keywords[i] != "</font>"))) {
                line = line.substr(0, i) + "&lt;" + line.substr(i + 1);
                selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                break;
            } else if (line[i] == '>' && (r.empty() || (markdown_keywords[i - r[i]] != "<center>" &&
                                                        markdown_keywords[i - r[i]] != "</center>" &&
                                                        markdown_keywords[i - r[i]] != "<u>" &&
                                                        markdown_keywords[i - r[i]] != "</u>" &&
                                                        markdown_keywords[i - r[i]] != ">" &&
                                                        markdown_keywords[i - r[i]] != "/>" &&
                                                        markdown_keywords[i - r[i]] != "</font>"))) {
                line = line.substr(0, i) + "&gt;" + line.substr(i + 1);
                selectMarkdown(line, i - 1, markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
                break;
            }
        }
    }

    static string writeLine(string &line, vector<char> &pop_parameter, bool root_is) {
        map<int, string> markdown_keywords;
        vector<int> l;
        map<int, int> r;
        getMarkdownKeywords(line, markdown_keywords, l, r);
        vector<int>::iterator lit = l.end() - 1;
        map<int, int>::iterator rit = r.end();
        if (!r.empty()) {
            rit--;
        }
        map<int, string>::iterator mkit = markdown_keywords.end();
        if (!markdown_keywords.empty()) {
            mkit--;
        }

        selectMarkdown(line, line.size(), markdown_keywords, l, r, lit, rit, mkit, pop_parameter, root_is);
        line = recoverEscapes(line, pop_parameter);
        return line;
    }

    static int getLineNum(const string &line) {
        int num = 0;
        for (const char &c: line) {
            if (c == '|') {
                num++;
            }
        }
        return num;
    }

    static void writeLines(vector<string> &lines, ofstream &ofs, bool root_is) {
        regex tableH("\\s{0,}(\\|.{1,}){1,}\\|\\s{0,}");
        regex tableN("\\s{0,}(\\|\\s{0,}-{1,}\\s{0,}){1,}\\|\\s{0,}");
        regex tableB("\\s{0,}(\\|.{1,}){1,}\\|\\s{0,}");
        regex codelinesStart("\\s{0,}`{3,}.{1,}");
        regex codelinesEnd("\\s{0,}`{3,}\\s{0,}");
        regex lineSelect("(\\*|\\+|-)(\\1){2,}");
        regex header("#{1,6}\\s.{1,}");
        regex quoteblock("\\s{0,}>\\s.{0,}");
        regex selectbox("\\s{0,}\\-\\s\\[[\\sxX]\\]\\s.{0,}");
        regex content("\\s{0,}\\[TOC\\]\\s{0,}");
        regex olistSelect("\\s{0,}\\d{1,}\\.\\s.{0,}");
        regex ulistSelect("\\s{0,}(\\*|\\+|-)\\s.{0,}");
        regex blankline("\\s{0,}");
        int row_num = 0;
        string result;
        map<int, vector<char>> pops;
        int continue_blank = 0;
        string TOC = setTOC(lines, root_is);
        // 删除转义符转义后的字符
        for (int i = 0; i < lines.size(); i++) {
            if (!regex_match(lines[i], blankline)) {
                lines[i] = deleteEscapes(lines[i], pops[i]);
            }
        }
        // 转化markdown
        for (int i = 0; i < lines.size(); i++) {
            if (!regex_match(lines[i], blankline)) {
                continue_blank = 0;
            }

            if (regex_match(lines[i], tableH)) {
                vector<string> contents;
                int header_num = getLineNum(lines[i]);
                contents.push_back(lines[i]);
                if (regex_match(lines[i + 1], tableN) && header_num == getLineNum(lines[i + 1])) {
                    row_num++;
                    contents.push_back(lines[i + 1]);
                    for (int j = i + 2; j < lines.size(); j++) {
                        if (regex_match(lines[j], tableB) && header_num == getLineNum(lines[j])) {
                            row_num++;
                            contents.push_back(lines[j]);
                        } else break;
                    }
                    tableProcess(ofs, contents, i, pops, root_is);
                    i += row_num;
                    row_num = 0;
                } else {
                    ofs << "<p>" << writeLine(lines[i], pops[i], root_is) << "</p>" << endl;
                }
            } else if (regex_match(lines[i], codelinesStart)) {
                vector<string> codes;
                int blank_num = getNumofCharfromPos(lines[i], ' ', 0);
                int code_num = getNumofCharfromPos(lines[i], '`', blank_num);
                for (int j = i + 1; j < lines.size(); j++) {
                    if (regex_match(lines[j], codelinesEnd) &&
                        code_num == getNumofCharfromPos(lines[j], '`', blank_num))
                        break;
                    row_num++;
                    lines[j] = recoverEscapes(lines[j], pops[j]);
                    lines[j] = lines[j].substr(blank_num);

                    for (int m = lines[j].size() - 1; m >= 0; m--) {
                        if (lines[j][m] == '<') {
                            lines[j] = lines[j].substr(0, m) + "&lt;" + lines[j].substr(m + 1);
                        } else if (lines[j][m] == '>') {
                            lines[j] = lines[j].substr(0, m) + "&gt;" + lines[j].substr(m + 1);
                        } else if (lines[j][m] == '&') {
                            lines[j] = lines[j].substr(0, m) + "&amp;" + lines[j].substr(m + 1);
                        }
                    }
                    codes.push_back(lines[j]);
                }
                ofs << writeCode(codes, lines[i].substr(code_num)) << endl;
                i += row_num + 1;
                row_num = 0;
            } else if (regex_match(lines[i], lineSelect)) {
                ofs << "<hr />" << endl;
            } else if (regex_match(lines[i], header)) {
                ofs << writeHeader(ofs, lines[i], pops[i], root_is) << endl;
            } else if (regex_match(lines[i], quoteblock)) {
                ofs << writeBlockquote(lines[i], pops[i], root_is) << endl;
            } else if (regex_match(lines[i], selectbox)) {
                ofs << writeSelectInput(lines[i], pops[i], root_is) << endl;
            } else if (regex_match(lines[i], content)) {
                ofs << TOC << endl;
            } else if (regex_match(lines[i], olistSelect)) {
                vector<string> ol;
                ol.push_back(lines[i]);
                for (int j = i + 1; j < lines.size(); j++) {
                    if (regex_match(lines[j], olistSelect)) {
                        row_num++;
                        ol.push_back(lines[j]);
                    } else break;
                }
                ofs << orderlyList(ol, "o", i, pops, root_is) << endl;
                i += row_num;
                row_num = 0;
            } else if (regex_match(lines[i], ulistSelect)) {
                vector<string> ul;
                ul.push_back(lines[i]);
                for (int j = i + 1; j < lines.size(); j++) {
                    if (regex_match(lines[j], ulistSelect)) {
                        row_num++;
                        ul.push_back(lines[j]);
                    } else break;
                }
                ofs << orderlyList(ul, "u", i, pops, root_is) << endl;
                i += row_num;
                row_num = 0;
            } else if (regex_match(lines[i], blankline)) {
                continue_blank++;
                if (continue_blank % 2 == 0) ofs << "<br />" << endl;
            } else {
                ofs << "<p>" << writeLine(lines[i], pops[i], root_is) << "</p>" << endl;
            }
        }
    }

    static void tableProcess(ofstream &ofs, vector<string> v1, int start, map<int, vector<char>> &pops, bool root_is) {
        vector<int> pos_124;
        vector<string>::iterator it = v1.begin();
        int row = v1.size(), now = -1, escape_num = 0, lastnum = 0;
        map<int, vector<char>> pop_table;
        ofs << "<div class=\"content-table-class\">" << endl
            << "<table>" << endl;


        ofs << "<thead>" << endl
            << "<tr>";

        for (int j = 0; j < v1.at(0).length(); ++j) {
            if (v1[0][j] == '\\') {
                pop_table[now].push_back(pops[start][escape_num++]);
            }
            if (v1.at(0).at(j) == '|') {
                pos_124.push_back(j);
                now++;

            }
        }

        for (int j = 0; j < pos_124.size() - 1; ++j) {
            string sub_str = v1[0].substr(pos_124[j] + 1, (pos_124[j + 1] - pos_124[j] - 1));
            ofs << "<th>" << writeLine(sub_str, pop_table[j], root_is) << "</th>" << endl;
        }

        ofs << "</tr>" << endl
            << "</thead>";
        escape_num = 0;
        pos_124.clear();
        ofs << "<tbody>" << endl;
        for (int i = 2; i < row; ++i) {
            lastnum = now;
            now--;
            for (int j = 0; j < v1.at(i).length(); ++j) {
                if (v1[i][j] == '\\') {
                    pop_table[now].push_back(pops[start + i][escape_num++]);
                }
                if (v1.at(i).at(j) == '|') {
                    pos_124.push_back(j);
                    now++;
                }
            }
            escape_num = 0;
            ofs << "<tr>" << endl;
            for (int j = 0; j < pos_124.size() - 1; ++j) {
                string sub_str = v1[i].substr(pos_124[j] + 1, (pos_124[j + 1] - pos_124[j] - 1));
                ofs << "<td>" << writeLine(sub_str, pop_table[lastnum + j], root_is) << "</td>" << endl;
            }
            pos_124.clear();
        }

        ofs << "</tbody>" << endl;
        ofs << "</table>" << endl
            << "</div>" << endl;
    }

    static string writeHeader(ofstream &ofs, const string &line, vector<char> &pop_parameter, bool root_is) {
        regex h1("#\\s.{1,}");
        regex h2("##\\s.{1,}");
        regex h3("###\\s.{1,}");
        regex h4("####\\s.{1,}");
        regex h5("#####\\s.{1,}");
        regex h6("######\\s.{1,}");
        string text, result;
        //<h5><a id="文本1-文本2-文本3-文本4-文本5" href="toc-文本1-文本2-文本3-文本4-文本5">文本5</a></h5>
        if (regex_match(line, h1)) {
            text = line.substr(2);
            nowH1 = text + "-";
            nowH2 = nowH1 + "--";
            nowH3 = nowH1 + "---";
            nowH4 = nowH1 + "----";
            nowH5 = nowH1 + "-----";
            result = "<h1><a id=\"" + line.substr(2) + "\" href=\"#toc-" + line.substr(2) + "\">" +
                     writeLine(text, pop_parameter, root_is) + "</a></h1>";
        } else if (regex_match(line, h2)) {
            text = line.substr(3);
            nowH2 = nowH1 + text + "-";
            nowH3 = nowH2 + "--";
            nowH4 = nowH2 + "---";
            nowH5 = nowH2 + "----";
            result = "<h2><a id=\"" + nowH1 + line.substr(3) + "\" href=\"#toc-" + nowH1 + line.substr(3) + "\">" +
                     writeLine(text, pop_parameter, root_is) + "</a></h2>";
        } else if (regex_match(line, h3)) {
            text = line.substr(4);
            nowH3 = nowH2 + text + "-";
            nowH4 = nowH3 + "--";
            nowH5 = nowH3 + "---";
            result = "<h3><a id=\"" + nowH2 + line.substr(4) + "\" href=\"#toc-" + nowH2 + line.substr(4) + "\">" +
                     writeLine(text, pop_parameter, root_is) + "</a></h3>";
        } else if (regex_match(line, h4)) {
            text = line.substr(5);
            nowH4 = nowH3 + text + "-";
            nowH5 = nowH4 + "--";
            result = "<h4><a id=\"" + nowH3 + line.substr(5) + "\" href=\"#toc-" + nowH3 + line.substr(5) + "\">" +
                     writeLine(text, pop_parameter, root_is) + "</a></h4>";
        } else if (regex_match(line, h5)) {
            text = line.substr(6);
            nowH5 = nowH4 + text + "-";
            result = "<h5><a id=\"" + nowH4 + line.substr(6) + "\" href=\"#toc-" + nowH4 + line.substr(6) + "\">" +
                     writeLine(text, pop_parameter, root_is) + "</a></h5>";
        } else if (regex_match(line, h6)) {
            text = line.substr(7);
            result = "<h6><a id=\"" + nowH5 + line.substr(7) + "\" href=\"#toc-" + nowH5 + line.substr(7) + "\">" +
                     writeLine(text, pop_parameter, root_is) + "</a></h6>";
        }
        return result;
    }

    static int getNumofCharfromPos(const string &line, const char &c, int pos) {
        int len = 0;
        for (int i = pos; i < line.size(); i++) {
            if (line[i] == c) len++;
            else return len;
        }
        return len;
    }

    static string
    orderlyList(vector<string> &lines, const string &c, int start, map<int, vector<char>> &pops, bool root_is) {
        string split = ". ";
        if (c == "u") {
            regex st("\\s{0,}\\*\\s.{0,}");
            regex ad("\\s{0,}\\+\\s.{0,}");
            regex de("\\s{0,}-\\s.{0,}");
            if (regex_match(lines[0], st)) split = "* ";
            else if (regex_match(lines[0], ad)) split = "+ ";
            else if (regex_match(lines[0], de)) split = "- ";
        }
        string result;
        int lastLineNum = getNumofCharfromPos(lines[0], ' ', 0), i = -1, nowLineNum = 0;
        string temp = lines[0].substr(lines[0].find(split) + 2);
        if (lastLineNum == 3) {
            result = "<" + c + "l>";
        }
        result += "<" + c + "l><li>" + writeLine(temp, pops[start], root_is) + "</li>\n";
        for (string &line: lines) {
            i++;
            if (i == 0) {
                continue;
            }
            nowLineNum = getNumofCharfromPos(line, ' ', 0);
            if (line.find(split) != -1) {
                line = line.substr(line.find(split) + 2);
                if (nowLineNum - lastLineNum < 3 && nowLineNum - lastLineNum >= 0) {
                    result += "<li>" + writeLine(line, pops[start + i], root_is) + "</li>\n";
                    lastLineNum = nowLineNum;
                } else if (nowLineNum - lastLineNum >= 3 && nowLineNum - lastLineNum < 4) {
                    result += "<" + c + "l><li>" + writeLine(line, pops[start + i], root_is) + "</li>\n";
                    lastLineNum = nowLineNum;
                } else if (nowLineNum < lastLineNum) {
                    for (i = 1; i <= lastLineNum - nowLineNum / 3; i++) {
                        result += "</" + c + "l>";
                    }
                    result += "<li>" + writeLine(line, pops[start + i], root_is) + "</li>";
                    lastLineNum = nowLineNum;
                }
            } else {
                result += writeLine(line, pops[start + i], root_is);
            }
        }
        if (lastLineNum != 0) {
            for (i = 1; i <= lastLineNum / 3; i++) {
                result += "</" + c + "l>\n";
            }
        }
        result += "</" + c + "l>";
        return result;
    }

    static string writeCode(vector<string> &lines, const string &code) {
        string result;
        result += "          <div class=\"content-page-div-code-class\">\n"
                  "            <div class=\"content-page-code-button-class\">\n"
                  "              <div class=\"content-page-code-button-copy-class\" title=\"复制代码\"></div>\n"
                  "              <div class=\"content-page-code-button-toimage-class\" title=\"生成代码图片\"></div>\n"
                  "              <div class=\"content-page-code-button-number-class\" title=\"显示/隐藏行号\"></div>\n"
                  "            </div>\n"
                  "            <table class=\"content-page-code-table-class\">\n"
                  "              <td class=\"content-page-code-number-box-class\">\n"
                  "                <div>\n"
                  "                  <pre class=\"content-page-code-number-class\"><code class=\"content-code-number\"></code></pre>\n"
                  "                </div>\n"
                  "              </td>\n"
                  "              <td>\n";
        result += "<pre class=\"content-page-code-class\"><code class=\"" + code + "\">";
        for (const string &line: lines) {
            result += line + "\n";
        }
        result += "</code></pre>\n"
                  "              </td>\n"
                  "            </table>\n"
                  "          </div>";
        return result;
    }

    static string writeSelectInput(string &line, vector<char> &pop_parameter, bool root_is) {
        string result;
        regex selectbox("\\s{0,}\\-\\s\\[[xX]\\]\\s.{0,}");
        if (regex_match(line, selectbox)) {
            result = "<p><input type=\"checkbox\" checked /><span class=\"content-checkbox-class\">";
        } else {
            result = "<p><input type=\"checkbox\" /><span class=\"content-checkbox-class\">";
        }
        line = line.substr(line.find("] ") + 2);
        result += writeLine(line, pop_parameter, root_is);
        result += "</span></p>";
        return result;
    }

    static string writeBlockquote(string &line, vector<char> &pop_parameter, bool root_is) {
        string result;
        int nowLen, i, pos;
        result += "<blockquote>";
        for (pos = 0; pos < line.size(); pos++) {
            if (line[pos] != ' ') break;
        }
        for (i = pos; i < line.size(); i++) {
            if (line[i] == '>' && line[i + 1] == ' ') {
                i++;
            } else break;
        }
        line = line.substr(i);
        result += writeLine(line, pop_parameter, root_is);
        result += "</blockquote>";
        return result;
    }

    static string setTOC(const vector<string> &lines, bool root_is) {
        string result;
        regex h1("#\\s.{1,}");
        regex h2("##\\s.{1,}");
        regex h3("###\\s.{1,}");
        regex h4("####\\s.{1,}");
        regex h5("#####\\s.{1,}");
        regex h6("######\\s.{1,}");
        string text;
        string nowh1 = "-";
        string nowh2 = "--";
        string nowh3 = "---";
        string nowh4 = "----";
        string nowh5 = "-----";
        result = "<ul class=\"toc\">\n";
        string temp;
        vector<char> pop_toc;
        for (const string &line: lines) {
            temp = line;
            if (regex_match(line, h1)) {
                temp = deleteEscapes(temp, pop_toc);
                text = temp.substr(2);
                nowh1 = text + "-";
                nowh2 = nowh1 + "--";
                nowh3 = nowh1 + "---";
                nowh4 = nowh1 + "----";
                nowh5 = nowh1 + "-----";
                result = result.append("<li class=\"first\"><a id=\"toc-").append(text).append("\" href=\"#").append(
                        text).append("\">").append(writeLine(text, pop_toc, root_is)).append("</a></li>\n");
            } else if (regex_match(line, h2)) {
                temp = deleteEscapes(temp, pop_toc);
                text = temp.substr(3);
                nowh2 = nowh1 + text + "-";
                nowh3 = nowh2 + "--";
                nowh4 = nowh2 + "---";
                nowh5 = nowh2 + "----";
                result = result.append("<li class=\"second\"><a id=\"toc-").append(nowh1).append(text).append(
                        "\" href=\"#").append(nowh1).append(text).append("\">").append(
                        writeLine(text, pop_toc, root_is)).append("</a></li>\n");
            } else if (regex_match(line, h3)) {
                temp = deleteEscapes(temp, pop_toc);
                text = temp.substr(4);
                nowh3 = nowh2 + text + "-";
                nowh4 = nowh3 + "--";
                nowh5 = nowh3 + "---";
                result = result.append("<li class=\"third\"><a id=\"toc-").append(nowh2).append(text).append(
                        "\" href=\"#").append(nowh2).append(text).append("\">").append(
                        writeLine(text, pop_toc, root_is)).append("</a></li>\n");
            } else if (regex_match(line, h4)) {
                temp = deleteEscapes(temp, pop_toc);
                text = temp.substr(5);
                nowh4 = nowh3 + text + "-";
                nowh5 = nowh4 + "--";
                result = result.append("<li class=\"fourth\"><a id=\"toc-").append(nowh3).append(text).append(
                        "\" href=\"#").append(nowh3).append(text).append("\">").append(
                        writeLine(text, pop_toc, root_is)).append("</a></li>\n");
            } else if (regex_match(line, h5)) {
                temp = deleteEscapes(temp, pop_toc);
                text = temp.substr(6);
                nowh5 = nowh4 + text + "-";
                result = result.append("<li class=\"fifth\"><a id=\"toc-").append(nowh4).append(text).append(
                        "\" href=\"#").append(nowh4).append(text).append("\">").append(
                        writeLine(text, pop_toc, root_is)).append("</a></li>\n");
            } else if (regex_match(line, h6)) {
                temp = deleteEscapes(temp, pop_toc);
                text = temp.substr(7);
                result = result.append("<li class=\"sixth\"><a id=\"toc-").append(nowh5).append(text).append(
                        "\" href=\"#").append(nowh5).append(text).append("\">").append(
                        writeLine(text, pop_toc, root_is)).append("</a></li>\n");
            }
            pop_toc.clear();
        }
        result += "</ul>\n";
        return result;
    }

    static string changeToUTF8(const string &line) {
        int len = MultiByteToWideChar(CP_ACP, 0, line.c_str(), -1, nullptr, 0);
        wchar_t *wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_ACP, 0, line.c_str(), -1, wstr, len);
        len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        char *str = new char[len + 1];
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, nullptr, nullptr);
        string strTemp = str;
        delete[] wstr;
        delete[] str;
        return strTemp;
    }

    static string changeToGBK(const string &line) {
        int len = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, nullptr, 0);
        wchar_t *wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, wstr, len);
        len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        char *str = new char[len + 1];
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, nullptr, nullptr);
        string strTemp = str;
        delete[] wstr;
        delete[] str;
        return strTemp;
    }
};

#endif //CL_MDTOHTML_FILE_UTIL_H   