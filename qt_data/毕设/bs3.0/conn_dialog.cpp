//数据库连接界面功能设计
#include "conn_dialog.h"
#include "ui_conn_dialog.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

CONN_Dialog * CONN_Dialog::p_conn_dialog=NULL;

CONN_Dialog::CONN_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CONN_Dialog)
{
    ui->setupUi(this);
}

CONN_Dialog *CONN_Dialog::get_conn_dialog()
{
    if(p_conn_dialog==NULL)
    {
        p_conn_dialog=new CONN_Dialog;
        return p_conn_dialog;
    }
    else
    {
        return p_conn_dialog;
    }
}

CONN_Dialog::~CONN_Dialog()
{
    delete ui;
}

QString CONN_Dialog::get_tab_name()
{
    return tab_name;
}

void CONN_Dialog::on_pushButton_test_clicked()
{
    //存数据表名
    this->tab_name=this->ui->lineEdit_tab_name->text();
    this->hostname=this->ui->lineEdit_ip->displayText();
    this->port=this->ui->lineEdit_port->displayText().toInt();
    this->database=this->ui->lineEdit_db->text();
    this->user=this->ui->lineEdit_user->displayText();
    this->passwd=this->ui->lineEdit_passwd->text();
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(this->hostname);
    db.setPort(this->port);
    db.setDatabaseName(this->database);
    db.setUserName(this->user);
    db.setPassword(this->passwd);
    bool ret=db.open();
    if(!ret)
    {
        QMessageBox::critical(0,"\xe6\x95\xb0\xe6\x8d\xae\xe5\xba\x93\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x8f\x90\xe7\xa4\xba",
                "连接数据库失败！","确定");
        return;
    }
    else
    {
        this->query=new QSqlQuery();
        emit tab_name_set();
        QMessageBox::information(0,"\xe6\x95\xb0\xe6\x8d\xae\xe5\xba\x93\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x8f\x90\xe7\xa4\xba",
                                 "\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x95\xb0\xe6\x8d\xae\xe5\xba\x93\xe6\x88\x90\xe5\x8a\x9f!","确定");
        //新qsqlquery才可以查询，否则提示数据库未打开
//        QSqlQuery * q;
//        q=getquery();
//        q->exec("select speed from data1");
//        if(!q->isActive())
//           {
//               QMessageBox::warning(this,"\xe6\x9f\xa5\xe8\xaf\xa2\xe9\x94\x99\xe8\xaf\xaf\xef\xbc\x81",
//                                    q->lastError().text(),"确定");
//           }
//        else {
//            qDebug()<<"succeed";
//        }
        emit conn_ok();//发送数据库lian连接成功信号
        this->close();
        return;
    }
}

void CONN_Dialog::on_pushButton_cancle_clicked()
{
    this->close();
}

