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

    int low;
    int high;

signals:
    void range_changed();
private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::set *ui;
};

#endif // SET_H
