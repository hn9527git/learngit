#ifndef CK_SET_H
#define CK_SET_H

#include <QDialog>

namespace Ui {
class ck_set;
}

class ck_set : public QDialog
{
    Q_OBJECT

public:
    explicit ck_set(QWidget *parent = 0);
    ~ck_set();
\
    QString com;
    int botelv;
    int shujuwei;
    int jiaoyanwei;
    int tingzhiwei;
signals:
    void set_ck_ok();
private slots:

    void on_pushButton_ok_clicked();

    void on_pushButton_cancle_clicked();

private:
    Ui::ck_set *ui;
};

#endif // CK_SET_H
