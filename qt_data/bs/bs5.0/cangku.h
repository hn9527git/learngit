#ifndef CANGKU_H
#define CANGKU_H

#include <QWidget>
#include "conn_dialog.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include "cangku_add.h"
#include "cangku_in.h"
#include "cangku_out.h"

namespace Ui {
class cangku;
}

class cangku : public QWidget
{
    Q_OBJECT

public:
    explicit cangku(QWidget *parent = 0);
    ~cangku();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_clicked();

    void on_pushButton_all_clicked();

    void on_pushButton_new_clicked();

    void add_data();//新增商品

    void on_pushButton_delete_clicked();//删除商品

    void on_pushButton_in_clicked();

    void in_ok_func();//入库商品槽函数

    void on_pushButton_out_clicked();

    void ck_ok_func();//出库商品槽函数

private:
    Ui::cangku *ui;
    CONN_Dialog * c_mysql;
    QSqlQuery * query;//数据库查询类指针
    QSqlTableModel * model;
    QStandardItemModel * stand_model;
    QString tab_name;//数据表名
    cangku_add * add;//仓库新增类指针
    cangku_in * in;//入库指针
    cangku_out * out;//出库指针
};

#endif // CANGKU_H
