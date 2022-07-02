#include "widget.h"
#include "ui_widget.h"
#include "highlighter.h"
#include <QFile>
#include <QFileDialog>
#include <QTextCodec>
#include <QFileInfo>
#include <QDebug>
#include <QPalette>
#include <QProcess>
#include <QDateTime>
#include<stdlib.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

}
void Widget::delayMSec(unsigned int msec)
{
    QTime Time_set = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < Time_set )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    QString fileName = this->ui->lineEdit->text();
    qDebug()<<fileName;
    if(fileName.isEmpty())
    {
        return ;
    }
    system(("export CLASSPATH=\".:/usr/local/lib/antlr-4.9.3-complete.jar:$CLASSPATH\" && cd ../../grammar/java &&java org.antlr.v4.gui.TestRig Go sourceFile " + fileName.toStdString() + " -gui").c_str());
}

void Widget::on_pushButton_3_clicked()
{

//    qDebug()<<fileName;
    system("cd ../../build && . ../scripts/run_asm.sh out > result.txt");

    QFile file4(QString::fromStdString("../../build/result.txt"));
    file4.open(QIODevice::ReadOnly);

    QByteArray array4;

    do
    {
        //单行读取
        array4 += file4.readLine();
    }while(!file4.atEnd());


    //将QByteArray转化成QStirng
//    QString content = QString(array);

    //使用gbk编码
    QTextCodec *codec4 = QTextCodec::codecForName("UTF-8");

    QString content4 = codec4->toUnicode(array4);

    //输出到edit上
    this->ui->textEdit_5->append(content4);
    Highlighter * h = new Highlighter(this->ui->textEdit_4->document());//传一个QTextDocument

    file4.close();

}
void Widget::on_pushButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this,"打开一个txt");
    //对filename做判断，如果没有选择文件，那么就是个空串
    if(fileName.isEmpty())
    {
        return ;
    }
    //不为空串，选择了某个文件，将文件名显示到lineEdit
    this->ui->lineEdit->setText(fileName);

    //使用qfile来读取文件

    std::string cmd_ = "cd ../../build/&&./tjugo " + fileName.toStdString();
    QFile file(fileName);
    this->ui->lineEdit->setText(fileName);
    QProcess *proc = new QProcess;

//    proc->start(cmd_.c_str());
//    proc->close();
    int sys_out = system(cmd_.c_str());
    qDebug()<<std::to_string(sys_out).c_str();
    if (sys_out != 0)
    {
        //程序出现问题

        QFile file4(QString::fromStdString("../../build/log/tjugo.INFO"));
        file4.open(QIODevice::ReadOnly);

        QByteArray array4;

        do
        {
            //单行读取
            array4 += file4.readLine();
        }while(!file4.atEnd());


        //将QByteArray转化成QStirng
    //    QString content = QString(array);

        //使用gbk编码
        QTextCodec *codec4 = QTextCodec::codecForName("UTF-8");

        QString content4 = codec4->toUnicode(array4);

        //输出到edit上
        this->ui->textEdit_4->append(content4);
        Highlighter * h = new Highlighter(this->ui->textEdit_4->document());//传一个QTextDocument

        file4.close();
        return ;

    }
    //打开文件
    file.open(QIODevice::ReadOnly);

    //读取文件的内容
    //将所有内容全部读取出来
//    QByteArray array = file.readAll();
    QByteArray array;

    do
    {
        //单行读取
        array += file.readLine();
    }while(!file.atEnd());


    //将QByteArray转化成QStirng
//    QString content = QString(array);

    //使用gbk编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    //将array转化到QString
    QString content = codec->toUnicode(array);

    //输出到edit上
    this->ui->textEdit->append(content);
    Highlighter * h1 = new Highlighter(this->ui->textEdit->document());//传一个QTextDocument



    //关闭文件
    file.close();

    QFile file2(QString::fromStdString("../../build/3code.txt"));
    file2.open(QIODevice::ReadOnly);

    QByteArray array2;

    do
    {
        //单行读取
        array2 += file2.readLine();
    }while(!file2.atEnd());


    //将QByteArray转化成QStirng
//    QString content = QString(array);

    //使用gbk编码
    QTextCodec *codec2 = QTextCodec::codecForName("UTF-8");

    QString content2 = codec2->toUnicode(array2);

    //输出到edit上
    this->ui->textEdit_2->append(content2);
    Highlighter * h2 = new Highlighter(this->ui->textEdit_2->document());//传一个QTextDocument

    file2.close();

    QFile file3(QString::fromStdString("../../build/out.asm"));
    file3.open(QIODevice::ReadOnly);

    QByteArray array3;

    do
    {
        //单行读取
        array3 += file3.readLine();
    }while(!file3.atEnd());


    //将QByteArray转化成QStirng
//    QString content = QString(array);

    //使用gbk编码
    QTextCodec *codec3 = QTextCodec::codecForName("UTF-8");

    QString content3 = codec3->toUnicode(array3);

    //输出到edit上
    this->ui->textEdit_3->append(content3);
    Highlighter * h3 = new Highlighter(this->ui->textEdit_3->document());//传一个QTextDocument
    file3.close();

    QFile file4(QString::fromStdString("../../build/log/tjugo.INFO"));
    file4.open(QIODevice::ReadOnly);

    QByteArray array4;

    do
    {
        //单行读取
        array4 += file4.readLine();
    }while(!file4.atEnd());


    //将QByteArray转化成QStirng
//    QString content = QString(array);

    //使用gbk编码
    QTextCodec *codec4 = QTextCodec::codecForName("UTF-8");

    QString content4 = codec4->toUnicode(array4);

    //输出到edit上
    this->ui->textEdit_4->append(content4);
    Highlighter * h4 = new Highlighter(this->ui->textEdit_4->document());//传一个QTextDocument
    file4.close();
}
