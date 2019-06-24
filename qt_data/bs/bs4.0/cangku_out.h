#ifndef CANGKU_OUT_H
#define CANGKU_OUT_H

#include <QWidget>

namespace Ui {
class cangku_out;
}

class cangku_out : public QWidget
{
    Q_OBJECT

public:
    explicit cangku_out(QWidget *parent = 0);
    ~cangku_out();
    QString get_name();
    int get_shuliang();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_ok_clicked();
signals:
    void ck_ok();
private:
    Ui::cangku_out *ui;
    QString name;
    int shuliang;
};

#endif // CANGKU_OUT_H
