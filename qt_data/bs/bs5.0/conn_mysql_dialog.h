#ifndef CONN_MYSQL_DIALOG_H
#define CONN_MYSQL_DIALOG_H
//数据库查询类
#include <QDialog>
#include "conn_dialog.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>

class CONN_Dialog;//声明类型

namespace Ui {
class conn_mysql_dialog;
}

class conn_mysql_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit conn_mysql_dialog(QWidget *parent = 0);
    ~conn_mysql_dialog();

private slots:
    void on_pushButton_clicked();//搜索全部

    void on_pushButton_2_clicked();//关闭

    void on_pushButton_find_clicked();//条件查询

private:
    Ui::conn_mysql_dialog *ui;

    QSqlQuery * query;//数据库查询类指针
    QSqlTableModel * model;
    QStandardItemModel * stand_model;
    QStringList ql;
    QString tab_name;//数据表名
    CONN_Dialog * p_conn_dialog;//数据库连接类指针（友元）

};

#endif // CONN_MYSQL_DIALOG_H
