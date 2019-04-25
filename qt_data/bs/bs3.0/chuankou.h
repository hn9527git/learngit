#ifndef CHUANKOU_H
#define CHUANKOU_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class chuankou;
}

class chuankou : public QDialog
{
    Q_OBJECT

public:
    explicit chuankou(QWidget *parent = 0);
    ~chuankou();

    QSerialPort * serial;

private slots:
    void ReadDate();//从串口读取数据

    void on_pushButton_ck_clicked();//打开串口槽函数

    void on_pushButton_send_clicked();//向串口发送数据

    void on_pushButton_clear_clicked();//清空数据显示区

private:
    Ui::chuankou *ui;

};

#endif // CHUANKOU_H
