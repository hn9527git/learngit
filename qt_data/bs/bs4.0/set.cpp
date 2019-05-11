#include "set.h"
#include "ui_set.h"

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

void set::on_pushButton_close_clicked()
{
    this->close();
}

void set::on_pushButton_ok_clicked()
{
    this->low=this->ui->lineEdit_low->text().toInt();
    this->high=this->ui->lineEdit_high->text().toInt();
    //设置完再通知主界面取值
    emit range_changed();
    this->close();
}
