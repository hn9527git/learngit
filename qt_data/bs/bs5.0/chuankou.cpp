#include "chuankou.h"
#include "ui_chuankou.h"
#include <QMessageBox>

chuankou::chuankou(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chuankou)
{
    ui->setupUi(this);
    ui->comboBox_2->setCurrentIndex(3);
    ui->pushButton_send->setEnabled(false);//串口未打开，不能发送数据，否则程序异常终止
    //在PC上搜索可用串口，并添加名字到ui界面上
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
//        QSerialPort serial;
//        serial.setPort(info);
//        if(serial.open(QIODevice::ReadWrite))
//        {
//            ui->comboBox->addItem(serial.portName());
//            serial.close();
//        }
        ui->comboBox->addItem(info.portName());
    }
}

chuankou::~chuankou()
{
    delete ui;
}

void chuankou::ReadDate()
{
    QByteArray buf;
    buf=serial->readAll();
    if(!buf.isEmpty())
    {
        ui->textEdit->append(buf);
    }
    buf.clear();
}

void chuankou::on_pushButton_ck_clicked()
{
    if(ui->pushButton_ck->text() == tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->comboBox->currentText());
        //打开串口
        if(serial->open(QIODevice::ReadWrite)==false)
        {
            QMessageBox::critical(this,"串口","打开失败!","确定");
            return;
        }
        //设置波特率
        switch (ui->comboBox_2->currentIndex())
        {
        case 0:
            serial->setBaudRate(QSerialPort::Baud1200);//设置波特率为1200
            break;
        case 1:
            serial->setBaudRate(QSerialPort::Baud2400);//设置波特率为2400
            break;
        case 2:
            serial->setBaudRate(QSerialPort::Baud4800);//设置波特率为4800
            break;
        case 3:
            serial->setBaudRate(QSerialPort::Baud9600);//设置波特率为9600
            break;
        case 4:
            serial->setBaudRate(QSerialPort::Baud19200);//设置波特率为19200
            break;
        case 5:
            serial->setBaudRate(QSerialPort::Baud38400);//设置波特率为38400
            break;
        case 6:
            serial->setBaudRate(QSerialPort::Baud57600);//设置波特率为57600
            break;
        case 7:
            serial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
            break;
        default:
            break;
        }
        //设置数据位数
        switch (ui->comboBox_3->currentIndex())
        {
        case 0:
            serial->setDataBits(QSerialPort::Data8);//设置数据位8
            break;
        default:
            break;
        }
        //设置校验位
        switch (ui->comboBox_4->currentIndex())
        {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //设置停止位
        switch (ui->comboBox_5->currentIndex())
        {
        case 0:
            serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
            break;
        case 1:
            serial->setStopBits(QSerialPort::TwoStop);
        default:
            break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

        //关闭设置菜单使能
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        ui->comboBox_5->setEnabled(false);
        ui->pushButton_ck->setText(tr("关闭串口"));

        //连接信号槽
        connect(serial,SIGNAL(readyRead()),this,SLOT(ReadDate()));
        //串口发送数据按钮使能
        ui->pushButton_send->setEnabled(true);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        ui->pushButton_send->setEnabled(false);//关闭发送按钮使能

        //恢复设置使能
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->pushButton_ck->setText(tr("打开串口"));

    }
}

void chuankou::on_pushButton_send_clicked()
{
    serial->write(ui->textEdit_2->toPlainText().toLatin1());
}
void chuankou::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}
