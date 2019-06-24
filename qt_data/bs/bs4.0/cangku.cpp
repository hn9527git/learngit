#include "cangku.h"
#include "ui_cangku.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>

cangku::cangku(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cangku)
{
    ui->setupUi(this);
    this->c_mysql=CONN_Dialog::get_conn_dialog();

    model = new QSqlTableModel(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置无法修改
    this->query=new QSqlQuery();
    this->stand_model=new QStandardItemModel;
    //设置表头
    this->stand_model->setHorizontalHeaderItem(0,new QStandardItem("记录ID"));
    this->stand_model->setHorizontalHeaderItem(1,new QStandardItem("\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1\xe5\x90\x8d\xe7\xa7\xb0"));
    this->stand_model->setHorizontalHeaderItem(2,new QStandardItem("库存"));
    this->stand_model->setHorizontalHeaderItem(3,new QStandardItem("单价"));
    this->stand_model->setHorizontalHeaderItem(4,new QStandardItem("时间"));

    this->add=new cangku_add;
    connect(this->add,SIGNAL(data_ok()),this,SLOT(add_data()));
    this->in=new cangku_in;
    connect(this->in,SIGNAL(in_ok()),this,SLOT(in_ok_func()));
    this->out=new cangku_out;
    connect(this->out,SIGNAL(ck_ok()),this,SLOT(ck_ok_func()));
}

cangku::~cangku()
{
    delete ui;
}

void cangku::on_pushButton_close_clicked()
{
    this->close();
}

void cangku::on_pushButton_clicked()
{
    this->ui->tableView->setModel(this->stand_model);//设置模式，非常重要不然查不出数据
    int chance=this->ui->comboBox->currentIndex();
    int id;
    int row;
    QString name;
    switch (chance) {
    case 0:
        QMessageBox::critical(this,"错误","请选择查询方式","确定");
        break;
    case 1:
        id=ui->lineEdit->text().toInt();
        if(id==NULL)
        {
            QMessageBox::critical(this,"错误","请输入要查询的流量计id","确定");
            break;
        }
        query->prepare("select * from cangku where id=:id");
        query->bindValue(0,id);
        query->exec();
        //int row;
        row=0;
        if(query->next()==false)
        {
            QMessageBox::critical(this,"错误","查询不到相关数据","确定");
            break;
        }
        else
        {
            for(int i=0;i<5;i++)
            {
                this->stand_model->setItem(row,i,new QStandardItem(query->value(i).toString()));
            }
            row++;
            while (query->next())
            {
                for(int i=0;i<5;i++)
                {
                    this->stand_model->setItem(row,i,new QStandardItem(query->value(i).toString()));
                }
                row++;
            }
            break;
        }
    case 2:
        name=ui->lineEdit->text();
        if(name==NULL)
        {
            QMessageBox::critical(this,"错误","请输入要查询的流量计名称","确定");
            break;
        }
        query->prepare("select * from cangku where name=:name");
        query->bindValue(0,name);
        if(query->exec())
        {
           // int row;
            row=0;
            if(query->next()==false)
            {
                QMessageBox::critical(this,"错误","查询不到相关数据","确定");
                break;
            }
            else
            {
                for(int i=0;i<5;i++)
                {
                    this->stand_model->setItem(row,i,new QStandardItem(query->value(i).toString()));
                }
                row++;
                while(query->next())
                {
                    for(int i=0;i<5;i++)
                    {
                        this->stand_model->setItem(row,i,new QStandardItem(query->value(i).toString()));
                    }
                    row++;
                }
                break;
            }
        }
        else
        {
            QMessageBox::critical(this,"错误","查询失败","确定");
            break;
        }
    default:
        break;
    }
}

void cangku::on_pushButton_all_clicked()
{
    //tableview显示数据
    this->tab_name="cangku";
    model->setTable(this->tab_name);
    ui->tableView->setModel(model);
    //显示所有数据
    if(model->select()==false)
    {
        QMessageBox::critical(this,"查询失败","数据库未连接","确定");
        this->close();
        return ;
    }
    model->setHeaderData(0,Qt::Horizontal,"ID");//设置字段别名
    model->setHeaderData(1,Qt::Horizontal,"\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1\xe7\xb1\xbb\xe5\x9e\x8b");//流量计信息
    model->setHeaderData(2,Qt::Horizontal,"\xe5\xba\x93\xe5\xad\x98\xe6\x95\xb0\xe9\x87\x8f\xef\xbc\x88\xe4\xb8\xaa\xef\xbc\x89");
    model->setHeaderData(3,Qt::Horizontal,"\xe5\x8d\x95\xe4\xbb\xb7\xef\xbc\x88\xef\xbf\xa5\xef\xbc\x89");
    model->setHeaderData(4,Qt::Horizontal,"\xe8\xae\xb0\xe5\xbd\x95\xe6\x97\xb6\xe9\x97\xb4  ");
}

void cangku::on_pushButton_new_clicked()
{
    this->add->show();
}

void cangku::add_data()
{
    this->query->prepare("insert into cangku (name,number,price) values (:name,:number,:price)");
    query->bindValue(":name",this->add->get_name());
    query->bindValue(":number",this->add->get_number());
    query->bindValue(":price",this->add->get_pricr());
//    query->bindValue(":name","\xe5\x91\xb5\xe5\x91\xb5\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1");
//    query->bindValue(":number","564123");
//    query->bindValue(":price","1111111");
    if(this->query->exec())
    {
        QMessageBox::information(this,"通知","新增成功","确定");
    }
    else
    {
        QMessageBox::critical(this,"通知","新增失败","确定");
    }
}

void cangku::on_pushButton_delete_clicked()
{
    QModelIndexList index_list=this->ui->tableView->selectionModel()->selectedIndexes();
    QModelIndex index;
    foreach(index,index_list)
    {
        this->model->removeRow(index.row());
    }
}

void cangku::on_pushButton_in_clicked()
{
    this->in->show();
}

void cangku::in_ok_func()
{
    this->query->prepare("select number from cangku where name=:name");
    int number=500;
    query->bindValue(0,this->in->get_name());
    query->exec();
    if(query->next()==false)
    {
        QMessageBox::critical(this,"错误","仓库中未找到此商品!","确定");
        return ;
    }
//    QSqlRecord re=query->record();
//    int pos=re.indexOf("number");
   // query->next();
    qDebug()<<query->value("number");
    number=query->value("number").toInt()+this->in->get_shuliang();
    qDebug()<<number;
    query->prepare("update cangku set number=:number where name=:name");
    query->bindValue(0,number);
    query->bindValue(1,this->in->get_name());
    if(query->exec()==false)
    {
        QMessageBox::critical(this,"错误","\xe5\x85\xa5\xe5\xba\x93\xe5\xa4\xb1\xe8\xb4\xa5\xef\xbc\x8c\xe8\xaf\xb7\xe9\x87\x8d\xe8\xaf\x95\xef\xbc\x81","确定");
        return ;
    }
    QMessageBox::information(this,"通知","入库成功","确定");
}

void cangku::on_pushButton_out_clicked()
{
    this->out->show();
}

void cangku::ck_ok_func()
{
    int number=this->out->get_shuliang();
    query->prepare("select number from cangku where name=:name");
    query->bindValue(0,this->out->get_name());
    query->exec();
    query->next();
    number=query->value("number").toInt()-number;
    if(number>=0)
    {
        query->prepare("update cangku set number=:number where name=:name");
        query->bindValue(0,number);
        query->bindValue(1,this->out->get_name());
        query->exec();
        QMessageBox::information(this,"通知","出库成功","确定");
    }
    else
    {
        QMessageBox::critical(this,"错误","仓库商品短缺","确定");
    }
}








