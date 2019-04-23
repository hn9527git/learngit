#ifndef CONN_MYSQL_H
#define CONN_MYSQL_H
#include <QSqlDatabase>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QSqlQuery>

void conn_mysql()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("ll_data");
    db.setUserName("root");
    db.setPassword("24641314");
    if(!db.open())
    {
        QMessageBox::critical(0,"\xe6\x95\xb0\xe6\x8d\xae\xe5\xba\x93\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x8f\x90\xe7\xa4\xba",
                "连接数据库失败！","确定");
        return;
    }
//    else
//    {
//        QMessageBox::information(0,"\xe6\x95\xb0\xe6\x8d\xae\xe5\xba\x93\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x8f\x90\xe7\xa4\xba",
//                                 "\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x95\xb0\xe6\x8d\xae\xe5\xba\x93\xe6\x88\x90\xe5\x8a\x9f!","确定");
//        return;
//    }
}

#endif // CONN_MYSQL_H
