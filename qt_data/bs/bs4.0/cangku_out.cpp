#include "cangku_out.h"
#include "ui_cangku_out.h"

cangku_out::cangku_out(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cangku_out)
{
    ui->setupUi(this);
}

cangku_out::~cangku_out()
{
    delete ui;
}

QString cangku_out::get_name()
{
    return  name;
}

int cangku_out::get_shuliang()
{
    return shuliang;
}

void cangku_out::on_pushButton_close_clicked()
{
    this->close();
}

void cangku_out::on_pushButton_ok_clicked()
{
    this->name=this->ui->lineEdit_name->text();
    this->shuliang=this->ui->lineEdit_shuliang->text().toInt();
    emit ck_ok();
    this->close();
}
