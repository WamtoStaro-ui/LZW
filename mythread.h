#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <iostream>
#include <unordered_map> 	//拥有更高的查询效率 哈希数实现
#include <fstream>
#include <cstdint>
#include <string>
#include <QTimer>
using namespace std;
union ben{              // 双字符使用
    uint16_t num;
    char c[2];
};
class Dictionary {      // 字典类
    //able to lookup the has both ways.
    public:
    void error(bool condition, const string &msg)           // 错误处理函数
    {
        if(!condition) {
        cout << msg<<endl;
        exit(1);
        }
    }

    void insert(uint16_t i, const string &str) {        // 插入的键值对存入两个hash里面
        hash_.insert(make_pair(i, str));
        hash2_.insert(make_pair(str, i));
    }

    bool exist(uint16_t i) {							// 两个判断是否存在函数 分别可判断uint16类型和string类型
        return end(hash_) != hash_.find(i);
    }

    bool exist(const string &str) {
        return end(hash2_) != hash2_.find(str);
    }

    uint16_t get_num(const string &str){				//通过 get_num和get_str  只需或得键对值里面的一个 即可知道另外一个
        auto found = hash2_.find(str);
        error(end(hash2_) != found, "get_num error");
        return found->second;
    }

    string get_str(uint16_t i) {
        auto found = hash_.find(i);
        error(end(hash_) != found, "get_str error");
        return found->second;
    }
    Dictionary() {
        for(uint16_t i=0; i<256; ++i) {				//初始化插入  0~255 对应的ascll值
        char c= i;
        hash_.insert(make_pair(i, string() + c));

        hash2_.insert(make_pair(string() + c, i));
        }
    }
    void print() {
        for(auto &p:hash_) {
        if(p.first >= 256)
            cout<<"["<<p.first<<", "<<p.second<<"]\n";
        }
    }
    private:
    unordered_map<uint16_t, string> hash_;		// i定义了两个容器
    unordered_map<string, uint16_t> hash2_;
};

class MyStream {		//获得文件中下一个值
    public:
    void error(bool condition, const string &msg)           // 错误处理函数
    {
        if(!condition) {
        cout << msg<<endl;
        exit(1);
        }
    }
    bool read(uint16_t &i) {
        ben bb;
        if(position + 1 <= length) {//ifs_.get(bb.c[0]) && ifs_.get(bb.c[1])
        bb.c[0] = *(str+position);
        bb.c[1] = *(str+position+1);
        position += 2;
        i = bb.num;
        return true;
        } else {
        return false;
        }
    }
    bool peek(uint16_t &i) {
        if(read(i)) {
        //ben bb;
        //bb.num = i;
        position -= 2;
        //ifs_.putback(bb.c[1]);
        //ifs_.putback(bb.c[0]);
        return true;
        } else {
        return false;
        }
    }
    int tellg() //当前文件指针所处文件位置
    {
        //return ifs_.tellg();
        return position;
    }
    void to_begin() // 文件指针设为文件开始处
    {
        ifs_.seekg(0,ios::beg);
    }
    void to_end()   // 文件指针设为文件结束处
    {
        ifs_.seekg(0,ios::end);
    }
    MyStream(const string &filename) {
        ifs_.open(filename, ios::in|ios::binary);
        ifs_.seekg(0,ios::end);
        length = ifs_.tellg();
        ifs_.seekg(0,ios::beg);
        str = new char[length];
        ifs_.read(str,length);  // 将ifs_内容读入字符串中
    }
    ~MyStream(){
        ifs_.close();
    }
    private:
    ifstream ifs_;
    char * str;
public:
    int length;
    int position = 0;  //当前文件指针位置
};



class mythread:public QObject   // 子线程
{
    Q_OBJECT
public:
    explicit mythread(QObject * parent = 0);
    void uncompress(const string &input, const string &output);
    void compress(const string &input, const string &output);
    void lzw(const char * argc[]);
protected:
signals:
    void current_process(int);

private slots:
    void mylzw(QString a1,QString a2,QString a3);
private:
    long total_length;
    long current_length;
};

#endif // MYTHREAD_H
