#include "ck_set.h"
#include "ui_ck_set.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

ck_set::ck_set(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ck_set)
{
    ui->setupUi(this);
    ui->comboBox_bote->setCurrentIndex(3);
    //在PC上搜索可用串口，并添加名字到ui界面上
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->comboBox_ckname->addItem(info.portName());
    }
}

ck_set::~ck_set()
{
    delete ui;
}

void ck_set::on_pushButton_ok_clicked()
{

    this->com=this->ui->comboBox_ckname->currentText();
    this->botelv=this->ui->comboBox_bote->currentText().toInt();
    this->shujuwei=this->ui->comboBox_shuju->currentText().toInt();
    this->jiaoyanwei=this->ui->comboBox_jiaoyan->currentText().toInt();
    this->tingzhiwei=this->ui->comboBox_tingzhi->currentIndex();
    qDebug()<<com<<botelv<<shujuwei<<jiaoyanwei<<tingzhiwei;
    emit set_ck_ok();
    this->close();
}

void ck_set::on_pushButton_cancle_clicked()
{
    this->close();
}
