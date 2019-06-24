#ifndef CANGKU_ADD_H
#define CANGKU_ADD_H

#include <QWidget>

namespace Ui {
class cangku_add;
}

class cangku_add : public QWidget
{
    Q_OBJECT

public:
    explicit cangku_add(QWidget *parent = 0);
    ~cangku_add();

    QString get_name();
    int get_number();
    double get_pricr();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_close_clicked();

signals:
    void data_ok();

private:
    Ui::cangku_add *ui;
    QString name;
    int number;
    double price;
};

#endif // CANGKU_ADD_H
