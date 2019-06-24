#include "cangku_add.h"
#include "ui_cangku_add.h"
#include <QDebug>

cangku_add::cangku_add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cangku_add)
{
    ui->setupUi(this);
}

cangku_add::~cangku_add()
{
    delete ui;
}

QString cangku_add::get_name()
{
    return this->name;
}

int cangku_add::get_number()
{
    return this->number;
}

double cangku_add::get_pricr()
{
    return this->price;
}

void cangku_add::on_pushButton_ok_clicked()
{
    this->name=this->ui->lineEdit_name->text();
    this->number=this->ui->lineEdit_number->text().toInt();
    this->price=this->ui->lineEdit_price->text().toDouble();

    qDebug()<<this->name<<this->number<<this->price;
    emit data_ok();

    this->ui->lineEdit_name->clear();
    this->ui->lineEdit_number->clear();
    this->ui->lineEdit_price->clear();

    this->close();
}

void cangku_add::on_pushButton_close_clicked()
{
    this->close();
}
