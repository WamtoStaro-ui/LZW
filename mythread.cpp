/*
    该线程执行lzw解压缩的执行过程
*/

#include "mythread.h"
#include <QDateTime>
#include <stdio.h>
mythread::mythread(QObject * parent) : QObject(parent)
{

}

void mythread::mylzw(QString a1,QString a2,QString a3)  //当mainwinodw主线程触发信号start()时 子线程调用该函数
{
    QTimer *timer = new QTimer();
    timer->start(100);
    QThread *thread = new QThread();
    timer->moveToThread(thread);
    connect(timer,&QTimer::timeout,[=](){
        int process = (int)(((double)current_length/(double)total_length)*100);
        emit current_process(process);
        if(process == 100)
            timer->stop();
    });
    thread->start();
    const char **a = new const char *[3];
    string tmp = a1.toStdString();
    if(a1 == "compress")
        a[0] = "compress";
    else
        a[0] = "uncompress";
    string tmp2 = a2.toStdString();
    a[1] = tmp2.data();
    string tmp3 = a3.toStdString();
    a[2] = tmp3.data();
    cout<<a[0]<<endl;
    cout<<a[1]<<endl;
    cout<<a[2]<<endl;

    lzw(a);
}

void mythread::uncompress(const string &input, const string &output)    // 解压过程
{
    long start_,end_;
    start_ = QDateTime::currentDateTime().toMSecsSinceEpoch();
    MyStream stream(input);
    ofstream ofs;
    string tmp = output.substr(output.size()-3,3);
    //cout<<"TYPE:"<<tmp<<endl;
    if(tmp == "txt")
    {
        ofs.open(output,ios::out);
        //cout<<"11111"<<endl;
    }
        else
    ofs.open(output, ios::out|ios::binary);
    //stream.to_end();
    //total_length = stream.tellg();
    //stream.to_begin();
    total_length = stream.length;
    Dictionary dict;
    uint16_t current_i;
    uint16_t key = 256;
    string current_str;
    string last_inserted_hash_str;
    while(stream.read(current_i)) {
    //current_length = stream.tellg();
    current_length = stream.position;
    //cout<<"hello world, current i ="<<current_i<<endl;
    bool must_exist = dict.exist(current_i);
    stream.error(must_exist, "cannot decode in uncompress!");
    string sss= dict.get_str(current_i);
    //cout <<"sss="<<sss<<endl;
    ofs << sss;
    uint16_t next_i;
    if(stream.peek(next_i)) {
        //cout <<"peek OK for "<<next_i<<endl;
        if(dict.exist(next_i)) {
        string next_str = dict.get_str(next_i);
        dict.insert(key++, sss+ next_str[0]);
        } else {
        stream.error(key == next_i, "help me.. i am confused!");
        dict.insert(key++, sss+ sss[0]);
        }
    }
    }
    //dict.print();
    end_ = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug()<<"解压时间差(微妙):"<<end_ - start_;
    ofs.close();
}

void mythread::compress(const string &input, const string &output)  //压缩过程
{
    long start_,end_;
    start_ = QDateTime::currentDateTime().toMSecsSinceEpoch();
    ifstream ifs(input,ios::binary);				//文件输入流
    //ofstream ofs(output, ios::binary);	//文件输出流

    const char * filename = output.data();
    FILE *fw = fopen(filename,"wb");


    ifs.seekg(0,ios::end);
    total_length = ifs.tellg(); // 文件总长度
    ifs.seekg(0,ios::beg);
    // 将内容一次读出
    char * str = new char[total_length];
    char * result_str = new char[total_length * 2];
    ifs.read(str,total_length);
    string current_str;
    Dictionary dict;
    uint16_t key = 256;
    char read_char;
    int i = 0,j = 0;
    while(i < total_length){		//流中读入一个字符到c
        read_char = *(str + i);
        i++;
    //current_length = ifs.tellg();
    current_length = i;
    current_str += read_char;
    //int pk = ifs.peek();
    int pk = static_cast<int>(*(str + i));
    string key_str = current_str + static_cast<char>(pk);
    if(dict.exist(current_str) && (!dict.exist(key_str) || pk==EOF)) {	// 未到文件结尾处// 当前字符存在于dictionary压缩表内时
        ben bb;
        bb.num = dict.get_num(current_str);
        //ofs.write(bb.c,2);					//写入输出流
        result_str[j] = bb.c[0];
        result_str[j+1] = bb.c[1];
        j += 2;
        if(key < 65535) {					// 然后将key_str写入压缩表中 key_str由当前字符和下一个字符组成
        dict.insert(key++, key_str);
        } else {
           // cout <<"   "<<key_str<<endl;
        }
        current_str.clear();                                                              }
                    }
    //ofs.write(result_str,j);
    fwrite(result_str,sizeof(char),j,fw);
    delete []str;   //删除申请的字符数组空间
    delete []result_str;
    ifs.close();
    //ofs.close();
    fclose(fw);
    end_ = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug()<<"时间差(微妙):"<<end_ - start_;
}

void mythread::lzw(const char *argv[])
{
    string option(argv[0]);
    string input(argv[1]);
    string output(argv[2]);

    if(0 == option.compare("compress")) {        		//第1个参数 字符匹配的是压缩
    compress(input, output);
    } else if(0 == option.compare("uncompress")) {		//匹配字符是解压
    uncompress(input, output);
    } else {
    cout <<"invalid option!"<<endl;						//参数不匹配
    }
}
