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
    void ReadDate();

    void on_pushButton_ck_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_clear_clicked();

private:
    Ui::chuankou *ui;

};

#endif // CHUANKOU_H
