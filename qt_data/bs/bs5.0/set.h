#ifndef SET_H
#define SET_H

#include <QWidget>

namespace Ui {
class set;
}

class set : public QWidget
{
    Q_OBJECT

public:
    explicit set(QWidget *parent = 0);
    ~set();

    int get_low();
    int get_high();
    int get_sec();
signals:
    void range_changed();
    void sec_changed();
private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::set *ui;

    int low;//正常流量范围下限
    int high;//上限
    int sec;//采集数据间隔
};

#endif // SET_H
