#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include "chuankou.h"
#include <QTimer>
#include <QString>
#include <QLabel>
#include "conn_mysql_dialog.h"
#include "conn_dialog.h"
#include <QPainter>
#include <QSqlQuery>
#include <QDateTime>
#include <set.h>

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();
double two_to_ten(QString str,int n);

protected:
    void paintEvent(QPaintEvent *);//绘图事件
    void drawCrown(QPainter *painter);//绘制表盘
    void drawBackground(QPainter *painter);//绘制背景
    void drawScale(QPainter *painter);//绘制刻度线
    void drawScaleNum(QPainter *painter);//绘制刻度值
    void drawTitle(QPainter *painter);//绘制单位
    void drawIndicator(QPainter *painter);//绘制表针
    void drawNumericValue(QPainter *painter);//画数字显示
    void drawLine(QPainter *painter);//绘线

private slots:
    void on_action_triggered();//显示串口界面
    void timeUpdate();

    void on_action_2_triggered();//查看数据界面

    void on_actionMySQL_triggered();//连接数据库界面

    void dataUpdate();//自定义的槽函数插入数据
    void on_pushButton_open_clicked();

    void data_read_fenxi();//数据读取分析

    void on_pushButton_close_clicked();

    void on_horizontalSlider_actionTriggered(int action);//调节器数据变化发送槽函数

    void update_ui_lx();//更新类型标签
    void update_zhen();//更新流量计指针值
    void mysql_status();//更新数据库连接状态
    void update_leiji();//更改累积流量值

    void on_action_Qt_triggered();//显示关于Qt界面

    void on_action_3_triggered();//设置流量范围界面
    void set_range();//设置正常流量范围槽函数

    void set_tab_name();//设置数据表名

signals:
    void data_update();//自定义信号
    void update_leixing();//更新类型标签
    void update_zhen_xh();//更改流量计指针值信号
    //void shebei_conn();//设备连接信号

private:
    Ui::MyMainWindow *ui;

    chuankou * ck;//串口类
    QTimer *timer;//状态栏时间触发器
    QTimer *m_dataTimer;//存储数据时间触发器
    QLabel *currTimeLable;
    conn_mysql_dialog * cmd;
    CONN_Dialog * cmysql;//数据连接类
    //仪表所用数据
    QColor m_background;
    QColor m_foreground;
    int m_maxValue;
    int m_minValue;
    int m_startAngle;
    int m_endAngle;
    int m_scaleMajor;
    int m_scaleMinor;
    double m_value;
    int m_precision;
    //QTimer *m_updateTimer;//暂用流量计时间触发器，后期使用数据触发
    QString m_units;
    QString m_title;

    //定义设备相关属性
    int leixing;//类型
    QString leixing_str;//字符串类型
    double  speed;//流量
    double wendu;//温度
    double yali;//压力
    double leiji_ll;//累积流量
    //double sv;//瞬时流量
    QDateTime start_time;
    bool jishi;//关闭时记录时长不变
    int status;//设备运行状态
    int status_db;//记录数据状态
    int run_low;//正常状态最低值
    int run_high;//正常状态最高值
    set * set_ptr;//设置类指针
    QString tab;//数据表名称

public Q_SLOTS:
    //void UpdateAngle();
};

#endif // MYMAINWINDOW_H
