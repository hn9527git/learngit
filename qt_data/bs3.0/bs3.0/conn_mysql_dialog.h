#ifndef CONN_MYSQL_DIALOG_H
#define CONN_MYSQL_DIALOG_H

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_find_clicked();

private:
    Ui::conn_mysql_dialog *ui;
    QSqlQuery * query;
    QSqlTableModel * model;
    QStandardItemModel * stand_model;
    QStringList ql;
    QString tab_name;
    CONN_Dialog * p_conn_dialog;

};

#endif // CONN_MYSQL_DIALOG_H
