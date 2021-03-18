#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "lzw.h"
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QTimer>
//#include "mythread.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("lzw压缩");
    ui->progressBar->setValue(0);
    qDebug()<<"main_id:"<<QThread::currentThreadId();
    timer = new QTimer();
    value = 0;
    timer->setInterval(1000);
    ui->lcdNumber->setDecMode();
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    //ui->lcdNumber->setDigitCount(100);
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber->display(0);
    connect(timer,&QTimer::timeout,[=]()mutable{
       value++;
       ui->lcdNumber->display(value);
       if(ui->progressBar->value() == 100)
           timer->stop();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()    // 点击进行压缩
{
    ui->progressBar->setValue(0);
    QString format;
    if(ui->comboBox->currentIndex() < 3)
    {
        if(ui->comboBox->currentIndex() == 0)
    {
        format = "*.txt";
    }
    if(ui->comboBox->currentIndex() == 1)
    {
        format = "*.bmp";
    }
    if(ui->comboBox->currentIndex() == 2)
    {
        format = "*.mov";
    }
    QString filePath =  QFileDialog::getOpenFileName(this,"",format);
    QString filePath_trsult = filePath.mid(0,filePath.size()-4);
    filePath_trsult += ".lzw";



    emit start("compress",filePath,filePath_trsult);
    }

    if(ui->comboBox->currentIndex() == 3) //其他任意类型文件
    {
        QString filePath = QFileDialog::getOpenFileName(this,"","");
        QString filePath_result = filePath + ".lzw";
        emit start("compress",filePath,filePath_result);
    }
    timer->start();// 计时器开始
    value = 0;
}

void MainWindow::on_pushButton_2_clicked()  // 点击解压
{
    ui->progressBar->setValue(0);
    QString format;
    if(ui->comboBox->currentIndex() < 3)
    {
    if(ui->comboBox->currentIndex() == 0)
    {
        format = "_result.txt";
    }
    if(ui->comboBox->currentIndex() == 1)
    {
        format = "_result.bmp";
    }
    if(ui->comboBox->currentIndex() == 2)
    {
        format = "_result.mov";
    }
    QString filePath =  QFileDialog::getOpenFileName(this,"","/Users/wssxw2/my_file/qt_files/lzw/test","*.lzw");
    QString filePath_result = filePath.mid(0,filePath.size()-4);
    filePath_result += format;
    emit start("uncompress",filePath,filePath_result);
    }

    if(ui->comboBox->currentIndex() == 3)
    {
        QString filePath = QFileDialog::getOpenFileName(this,"","","*.lzw");
        QString filePath_result = filePath.mid(0,filePath.size() - 4);
        int position = filePath_result.lastIndexOf('.');
        filePath_result.insert(position,"_result");
        emit start("uncompress",filePath,filePath_result);
    }
    timer->start();// 计时器开始
    value = 0;
}


void MainWindow::set_progress_slot(int value)
{
    ui->progressBar->setValue(value);
}
