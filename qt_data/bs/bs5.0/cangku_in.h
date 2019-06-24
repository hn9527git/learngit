#ifndef CANGKU_IN_H
#define CANGKU_IN_H

#include <QWidget>
#include "conn_dialog.h"

namespace Ui {
class cangku_in;
}

class cangku_in : public QWidget
{
    Q_OBJECT

public:
    explicit cangku_in(QWidget *parent = 0);
    ~cangku_in();
    QString get_name();
    int get_shuliang();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
signals:
    void in_ok();

private:
    Ui::cangku_in *ui;

    QString name;
    int shuliang;
    CONN_Dialog * cmysql;//数据连接类
};

#endif // CANGKU_IN_H
