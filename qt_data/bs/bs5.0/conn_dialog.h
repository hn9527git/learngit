#ifndef CONN_DIALOG_H
#define CONN_DIALOG_H
//数据库连接类
#include <QDialog>
#include <QString>
#include <QSqlQuery>
#include "conn_mysql_dialog.h"

//int CONN_DB=0;

namespace Ui {
class CONN_Dialog;
}

class CONN_Dialog : public QDialog
{
    Q_OBJECT

public:
    static CONN_Dialog * get_conn_dialog();
    ~CONN_Dialog();
    //QString get_tab_name();
private slots:
    void on_pushButton_test_clicked();//数据库连接测试槽函数

    void on_pushButton_cancle_clicked();//取消连接

signals:
    void conn_ok();//数据库连接成功信号
    //void tab_name_set();//设置数据表名信号

private:
    explicit CONN_Dialog(QWidget *parent = 0);
    static CONN_Dialog * p_conn_dialog;

    CONN_Dialog(const CONN_Dialog&);//禁止复制本类
    Ui::CONN_Dialog *ui;
    QString hostname;//地址
    int port;//端口
   // QString database;//数据库
    QString user;//用户
    QString passwd;//密码
    QSqlQuery *query;//连接后执行数据库操作类
    QSqlDatabase db;
   // QString tab_name;
    friend class MyMainWindow;
    friend class conn_mysql_dialog;

};

#endif // CONN_DIALOG_H
