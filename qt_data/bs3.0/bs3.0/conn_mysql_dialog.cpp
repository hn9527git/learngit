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

    this->p_conn_dialog=CONN_Dialog::get_conn_dialog();

}

conn_mysql_dialog::~conn_mysql_dialog()
{
    delete ui;
}

void conn_mysql_dialog::on_pushButton_clicked()
{
    //使用select语句查询，后边在设计ui，暂不用--加一个textEdit_v
//    query=new QSqlQuery();
//    query->exec("SELECT * FROM DATA1");
//    if(!query->isActive())
//    {
//        QMessageBox::warning(this,"\xe6\x9f\xa5\xe8\xaf\xa2\xe9\x94\x99\xe8\xaf\xaf\xef\xbc\x81",
//                             query->lastError().text(),"确定");
//    }
//    while(query->next())
//    {
//        QString id = query->value(0).toString();
//        QString speed=query->value(1).toString();
//        //QString buf=this->ui->textEdit_v->toPlainText();
//        QString buf=this->ui->textEdit_v->toPlainText()+id+" "+speed+"\n";
//        if(buf=="")
//        {
//            this->ui->textEdit_v->setPlainText("no data!");
//        }
//        else
//        {
//            //speed=buf+'\n'+speed;
//            this->ui->textEdit_v->setPlainText(buf);
//        }
//    }
    //tableview显示数据
    this->p_conn_dialog=CONN_Dialog::get_conn_dialog();
    this->tab_name=this->p_conn_dialog->get_tab_name();
    model->setTable(this->tab_name);
    ui->tableView->setModel(model);
    //显示所有数据
    if(model->select()==false)
    {
        QMessageBox::critical(this,"查询失败","数据库未连接","确定");
        this->close();
        return ;
    }

    model->setHeaderData(0,Qt::Horizontal,"记录ID");//设置字段别名
    model->setHeaderData(1,Qt::Horizontal,"瞬时速度(m³/h)");
    model->setHeaderData(2,Qt::Horizontal,"记录时间");

}

void conn_mysql_dialog::on_pushButton_2_clicked()
{
    this->close();
}

void conn_mysql_dialog::on_pushButton_find_clicked()
{
    this->ui->tableView->setModel(this->stand_model);//设置模式
    int xuanze=this->ui->comboBox->currentIndex();
    QString id;
    //qDebug()<<"find in";
    switch (xuanze) {
    case 0:
        break;
    case 1:
        id=ui->lineEdit_find->text();
        query->prepare("select * from data where id=:id");
        query->bindValue(0,id);
        query->exec();
        int row;
        row=0;
        while (query->next())
        {
//            qDebug()<<"yes";
//            QString temp;
//            temp=query->value(0).toString()+" "+query->value(1).toString()+" "+query->value(2).toString();
//            qDebug()<<temp;
            for(int i=0;i<6;i++)
            {
                this->stand_model->setItem(row,i,new QStandardItem(query->value(i).toString()));
            }
            row++;
        }
        break;
    case 2:

        break;
    case 3:

        break;
    default:
        break;
    }
}
