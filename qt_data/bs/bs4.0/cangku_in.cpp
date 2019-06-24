#include "cangku_in.h"
#include "ui_cangku_in.h"

cangku_in::cangku_in(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cangku_in)
{
    ui->setupUi(this);
}

cangku_in::~cangku_in()
{
    delete ui;
}

QString cangku_in::get_name()
{
    return this->name;
}

int cangku_in::get_shuliang()
{
    return this->shuliang;
}

void cangku_in::on_pushButton_2_clicked()
{
    this->close();
}

void cangku_in::on_pushButton_clicked()
{
    this->name=this->ui->lineEdit_name->text();
    this->shuliang=this->ui->lineEdit_shuliang->text().toInt();
    emit in_ok();
    this->close();
}
