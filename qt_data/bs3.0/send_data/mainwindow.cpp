#include "mainwindow.h"
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>//数据流
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            //ui->portBox->addItem(serial.portName());
            serial.close();
        }
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::open()
{
    serial = new QSerialPort;
    //设置串口名
    serial->setPortName("COM2");
    //打开串口
    serial->open(QIODevice::ReadWrite);
    qDebug()<<"open";
    //设置波特率
    serial->setBaudRate(QSerialPort::Baud9600);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //connect(this->serial,SIGNAL(readyRead()),this,SLOT(read_data()));
}

void MainWindow::send()
{
    QFile file("../data.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
//    while(in.atEnd()==false)
//    {
    QString line=NULL;
    while((line=in.readLine())!=NULL)
    {
       // QString line=in.readLine();
        qDebug()<<line;
        qDebug()<<"before";
        this->serial->write(line.toLatin1().data());
        serial->waitForBytesWritten();
        qDebug()<<"send";
        int n=5000000000;
        while(n!=0)
        {
            n--;
        }
        line.clear();
    }
//    }

}
