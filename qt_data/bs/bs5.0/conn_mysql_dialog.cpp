//数据库查询及其数据输出功能设计
#include "conn_mysql_dialog.h"
#include "ui_conn_mysql_dialog.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QList>

conn_mysql_dialog::conn_mysql_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conn_mysql_dialog)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置无法修改
    this->query=new QSqlQuery();
    this->stand_model=new QStandardItemModel;
    //设置表头
    this->stand_model->setHorizontalHeaderItem(0,new QStandardItem("记录ID"));
    this->stand_model->setHorizontalHeaderItem(1,new QStandardItem("流量计类型|型号"));
    this->stand_model->setHorizontalHeaderItem(2,new QStandardItem("瞬时流量"));
    this->stand_model->setHorizontalHeaderItem(3,new QStandardItem("温度"));
    this->stand_model->setHorizontalHeaderItem(4,new QStandardItem("压力"));
    this->stand_model->setHorizontalHeaderItem(5,new QStandardItem("记录时间"));

    this->p_conn_dialog=CONN_Dialog::get_conn_dialog();//获取数据库连接类(单例)指针

}

conn_mysql_dialog::~conn_mysql_dialog()
{
    delete ui;
}

void conn_mysql_dialog::on_pushButton_clicked()//查看所有数据
{
    //tableview显示数据
    this->p_conn_dialog=CONN_Dialog::get_conn_dialog();
    //this->tab_name=this->p_conn_dialog->get_tab_name();
    model->setTable("data_caiji");
    ui->tableView->setModel(model);
    //显示所有数据
    if(model->select()==false)
    {
        QMessageBox::critical(this,"查询失败","数据库未连接","确定");
        this->close();
        return ;
    }

    model->setHeaderData(0,Qt::Horizontal,"记录ID");//设置字段别名
    model->setHeaderData(1,Qt::Horizontal,"\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1\xe4\xbf\xa1\xe6\x81\xaf");//流量计信息
    model->setHeaderData(2,Qt::Horizontal,"瞬时速度(m³/h)");
    model->setHeaderData(3,Qt::Horizontal,"温度(摄氏度))");
    model->setHeaderData(4,Qt::Horizontal,"压力(Pa)");
    model->setHeaderData(5,Qt::Horizontal,"记录时间");

}

void conn_mysql_dialog::on_pushButton_2_clicked()//取消查询，关闭界面
{
    this->close();
}

void conn_mysql_dialog::on_pushButton_find_clicked()
{
    this->ui->tableView->setModel(this->stand_model);//设置模式
    int xuanze=this->ui->comboBox->currentIndex();
    QString id;
    QString name;
    //qDebug()<<"find in";
    switch (xuanze) {
    case 0:
        break;
    case 1://按ID查询
        this->stand_model->removeRows(0,stand_model->rowCount());//删除当前的数据条目
//        this->stand_model->clear();
//        this->ui->tableView->setModel(this->stand_model);//设置模式
        id=ui->lineEdit_find->text();
        query->prepare("select * from data_caiji where id=:id");
        query->bindValue(0,id);
        query->exec();
        int row;
        row=0;
        while (query->next())
        {
            for(int i=0;i<6;i++)
            {
                this->stand_model->setItem(row,i,new QStandardItem(query->value(i).toString()));
            }
            row++;
        }
        break;
    case 2://按名称查询
        this->stand_model->removeRows(0,stand_model->rowCount());//删除当前的数据条目
//        this->stand_model->clear();
//        this->ui->tableView->setModel(this->stand_model);//设置模式
        name=this->ui->lineEdit_find->text().left(2);
        qDebug()<<"name============================="<<name;
        query->prepare("select * from data_caiji where left(leixing,2)=:name");
        query->bindValue(0,name);
        query->exec();
        int rowid;
        rowid=0;
        while (query->next())
        {
            for(int i=0;i<6;i++)
            {
                this->stand_model->setItem(rowid,i,new QStandardItem(query->value(i).toString()));
            }
            rowid++;
        }
        break;
    default:
        break;
    }
}
