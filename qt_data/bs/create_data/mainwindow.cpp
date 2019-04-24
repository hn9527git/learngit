#include "mainwindow.h"
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{

}

void MainWindow::write_data()
{
    QFile file("../data.txt");
    bool isok=file.open(QIODevice::WriteOnly);
    if(true==isok)
    {
        qDebug()<<"yes to write";
        QTextStream stream(&file);
        int x=1000;
        while(x!=0)
        {
            QString temp="";
            temp=this->make_rand();
            temp+="\n";
            qDebug()<<temp;
            stream.setCodec("UTF-8");
            stream<<temp;
            x--;
        }
    }
}

QString MainWindow::make_rand()
{
    QString str;


    for(int i=0;i<11;++i)
    {
        QTime time= QTime::currentTime();
        qsrand(time.msec()*qrand()*qrand()*qrand()*qrand()*qrand()*qrand());
        int n = qrand() % 2;
        if(n==0)
        {
            str[i]='0';
        }
        else if(n==1)
        {
            str[i]='1';
        }
    }
    str[3]='0';
    str[11]='.';
    if((str[4]=='1')&&(str[5]=='1'))
    {
        str[6]='0';
        str[7]='0';
        str[9]='0';
        str[10]='0';
        for(int j=12;j<16;j++)
        {
            str[j]='0';
        }
    }
    else
    {
        for(int j=12;j<16;j++)
        {
            QTime time= QTime::currentTime();
            qsrand(time.msec()*qrand());
            int m = qrand() % 2;
            if(m==0)
            {
                str[j]='1';
            }
            else if(m==1)
            {
                str[j]='0';
            }
        }
    }
    if(str[12]=='1')
    {
        str[13]='0';
        str[14]='0';
    }
    qDebug()<<str;
    return str;
}
