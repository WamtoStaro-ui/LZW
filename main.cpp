#include "mainwindow.h"

#include <QApplication>
#include "mythread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    /*由于压缩过程计算量较大 放在主线程会阻塞ui界面 将压缩过程放在子线程中进行 主线程只执行ui操作*/
    mythread * MYt = new mythread;
    QThread *thread = new QThread();
    MYt->moveToThread(thread);
    QObject::connect(&w,SIGNAL(start(QString,QString,QString)),MYt,SLOT(mylzw(QString,QString,QString)));
    QObject::connect(MYt,SIGNAL(current_process(int)),&w,SLOT(set_progress_slot(int)));

    w.show();
    thread->start();
    return a.exec();
}
