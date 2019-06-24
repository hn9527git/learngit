#include "set.h"
#include "ui_set.h"
#include <QDebug>

set::set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::set)
{
    ui->setupUi(this);
}

set::~set()
{
    delete ui;
}

int set::get_low()
{
    return this->low;
}

int set::get_high()
{
    return this->high;
}

int set::get_sec()
{
    return this->sec;
}

void set::on_pushButton_close_clicked()
{
    this->close();
}

void set::on_pushButton_ok_clicked()
{
    int l=this->ui->lineEdit_low->text().toInt();
    int h=this->ui->lineEdit_high->text().toInt();
    if( l>=0 && h>0 && l<h)
    {
        this->low=this->ui->lineEdit_low->text().toInt();
        this->high=this->ui->lineEdit_high->text().toInt();
        emit range_changed();
    }
    // qDebug()<<"zuigao1shangxianshi1"<<this->high;
    if(this->ui->lineEdit->text().toInt()>0)
    {
        this->sec=this->ui->lineEdit->text().toInt();
        emit sec_changed();
    }
    //设置完再通知主界面取值

    this->close();
}
