#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include "chuankou.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

//int MyMainWindow::notice=0;
MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    this->timer=new QTimer;
    this->currTimeLable=new QLabel;
    this->timer->start(1000);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    //仪表绘制

    m_background = Qt::black;
    m_foreground = Qt::green;

    m_startAngle = 60;//起始角度
    m_endAngle = 60;//结束角度
    m_scaleMajor = 10;//分刻度数
    m_minValue = 0;
    m_maxValue = 100;
    m_scaleMajor = 10;//分度
    m_scaleMinor = 10;
    m_units = "m³/h";
    m_title = "\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1";//流量计
    m_precision = 0;
    m_value = 0;

    //m_updateTimer = new QTimer(this);
    //m_updateTimer->setInterval(500);//间隔，微秒单位
    //connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(UpdateAngle()));

    //    setWindowFlags(Qt::FramelessWindowHint);//无窗体
    //    setAttribute(Qt::WA_TranslucentBackground);//背景透明

    //构建数据采集定时器
    this->m_dataTimer = new QTimer;
    //采集数据定时器槽函数连接
    connect(this->m_dataTimer,SIGNAL(timeout()) ,this,SLOT(dataUpdate()));

    //构建串口类
    this->ck=new chuankou();

    connect(this,SIGNAL(update_leixing()),this,SLOT(update_ui_lx()));
    connect(this,SIGNAL(update_zhen_xh()),this,SLOT(update_zhen()));
    connect(this,SIGNAL(update_zhen_xh()),this,SLOT(update_leiji()));

    //数据库连接界面类（单例，为了获得其状态）
    this->cmysql=CONN_Dialog::get_conn_dialog();

    //数据库连接类连接成功发出conn_ok信号，通过槽函数修改数据库连接状态
    connect(this->cmysql,SIGNAL(conn_ok()),this,SLOT(mysql_status()));

    //设备状态
    this->status=0;//未连接
    this->status_db=0;//未记录数据状态
    //初始化瞬时流量正常范围
    this->run_low=0;
    this->run_high=100;

    this->set_ptr=new set();//构建设置类

    //界面设置范围发出range_changed信号，通过槽函数修改范围
    connect(this->set_ptr,SIGNAL(range_changed()),this,SLOT(set_range()));

    //数据库连接类的数据表名发生变化发出tab_name_set信号，通过槽函数修改数据表名
    connect(this->cmysql,SIGNAL(tab_name_set()),this,SLOT(set_tab_name()));
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

double MyMainWindow::two_to_ten(QString str,int n)//二进制转换为十进制
{
    qDebug()<<"two_to_ten";
    bool ret;
    double data=0;
    int length=n;
    int point_left=0;
    QString stra,strb;
    QString::iterator it=str.begin();
    for(;it!=str.end();it++,point_left++)
    {
        if(*it==".")
        {
            stra=str.left(point_left);
            qDebug()<<stra;
            strb=str.right(n-point_left-1);
            qDebug()<<strb;
            ret=true;
            break;
        }
        ret=false;
    }
    if(!ret)//无小数点
    {
        QString::const_iterator it=str.begin();
        while(it!=str.end())
        {
            QString t=(*it);
            //qDebug()<<"\xe8\xbd\xac\xe6\x8d\xa2\xe5\x89\x8d";
            data+=t.toDouble()*(pow(2,(--length)));
            //qDebug()<<"\xe8\xbd\xac\xe6\x8d\xa2\xe5\x90\x8e";
            it++;
        }
        return data;
    }
    else//有小数点
    {
        double a=0,b=0;
        int an=stra.length();
        int bn=strb.length();
        QString::const_iterator ita=stra.begin();
        QString::const_iterator itb=strb.begin();
        while(ita!=stra.end())
        {
            QString sa=(*ita);
            a+=sa.toDouble()*(pow(2,--an));
            ita++;
        }
        while(itb!=strb.end())
        {
            QString sb=(*itb);
            b+=sb.toDouble()*(pow(2,--bn));
            itb++;
        }
        qDebug()<<a<<"a,b"<<b;
        return a+b/10;
    }
    return 0;
}

void MyMainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);//一个类中的this表示一个指向该类自己的指针
    //设置背景为白色
    //    painter.setPen(Qt::NoPen);
    //    painter.setBrush(Qt::white);
    //    painter.drawRect(rect());
    painter.setRenderHint(QPainter::Antialiasing);  // 使用反锯齿(如果可用)
    //painter.translate(width() / 2, height() / 2);   // 坐标变换为窗体中心
    painter.translate(210,230);//更改位置
    //int side = qMin(width(), height());
    //painter.scale(side / 200.0, side / 200.0);      // 比例缩放

    drawCrown(&painter);                                 // 画表盘边框
    drawScaleNum(&painter);                          // 画刻度数值值
    drawScale(&painter);                                 // 画刻度线
    drawTitle(&painter);                                     //画单位
    drawNumericValue(&painter);                      // 画数字显示
    drawIndicator(&painter);                             // 画表针
    //3.10 add
    //3.26 delete

    //画图标
    painter.translate(-210,-230);
    painter.drawPixmap(15,550,50,50,QPixmap(":/bsimg/leiji.png"));
    painter.drawPixmap(230,550,50,50,QPixmap(":/bsimg/shunshi.png"));
    painter.drawPixmap(15,640,50,50,QPixmap(":/bsimg/wendu.png"));
    painter.drawPixmap(230,640,50,50,QPixmap(":/bsimg/yali.png"));
    drawLine(&painter);                    //画线
    //画状态
    if(this->status==0)
    {
        painter.drawPixmap(20,485,20,20,QPixmap(":/bsimg/weilianjie.png"));
        this->ui->label_run->setText("\xe8\xae\xbe\xe5\xa4\x87\xe5\xb7\xb2\xe5\x85\xb3\xe9\x97\xad");//设备已关闭
    }
    else if(this->status==1)
    {
        painter.drawPixmap(20,485,20,20,QPixmap(":/bsimg/ok.png"));
        this->ui->label_run->setText("运行正常");
    }
    else
    {
       painter.drawPixmap(20,485,20,20,QPixmap(":/bsimg/error.png"));
       this->ui->label_run->setText("运行异常");
    }
    if(this->status_db==0)
    {
        painter.drawPixmap(20,515,20,20,QPixmap(":/bsimg/weilianjie.png"));
    }
    else if(this->status_db==1)
    {
        painter.drawPixmap(20,515,20,20,QPixmap(":/bsimg/yilianjie.png"));
        this->ui->label_shuju_status->setText("数据库已连接");//数据库已连接
    }
    else if(this->status_db==2)
    {
        painter.drawPixmap(20,515,20,20,QPixmap(":/bsimg/jilu.png"));
        this->ui->label_shuju_status->setText("\xe6\x95\xb0\xe6\x8d\xae\xe8\xae\xb0\xe5\xbd\x95\xe4\xb8\xad");//数据记录中
    }
}
void MyMainWindow::drawCrown(QPainter *painter)
{
    painter->save();//将painter状态存在栈中
    int radius = 200;//设置外圈半径
    QLinearGradient lg1(0,-radius, 0, radius);//设置从上到下渐变

    lg1.setColorAt(0, Qt::blue); //设置渐变的颜色和路径比例
    lg1.setColorAt(1, Qt::white); //只是粗略的颜色，具体的可以参考RGB颜色查询对照表

    painter->setBrush(lg1); // 创建QBrush对象,把这个渐变对象传递进去：
    painter->setPen(Qt::NoPen); //边框线无色
    painter->drawEllipse(-radius, -radius, radius<< 1 , radius << 1);//绘制矩形定义的椭圆
    painter->setBrush(m_background = Qt::black);//设置背景颜色
    painter->drawEllipse(-175, -175, 350,350);
    painter->restore();//取出栈中的painter
}
void MyMainWindow::drawScale(QPainter *painter)
{
    painter->save();
    painter->rotate(m_startAngle);//从六十度开始画
    int steps = (m_scaleMajor * m_scaleMinor); //相乘后的值是分的份数
    double angleStep = (360.0 - m_startAngle - m_endAngle) / steps; //每一个份数的角度

    // painter->setPen(m_foreground); //m_foreground是颜色的设置
    // QPen pen = painter->pen(); //第一种方法
    QPen pen ;
    pen.setColor(Qt::green); //推荐使用第二种方式
    for (int i = 0; i <= steps; i++)
    {
        if (i % m_scaleMinor == 0)//整数刻度显示加粗
        {
            pen.setWidth(1); //设置线宽
            painter->setPen(pen); //使用面向对象的思想，把画笔关联上画家。通过画家画出来
            painter->drawLine(0, 120, 0, 160); //两个参数应该是两个坐标值
        }
        else
        {
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawLine(0, 140, 0, 160);
        }
        painter->rotate(angleStep);//下一条线
    }
    painter->restore();
}
void MyMainWindow::drawScaleNum(QPainter *painter)
{
    painter->save();
    painter->setPen(m_foreground);
    double startRad = ( 270-m_startAngle) * (3.14 / 180);
    double deltaRad = (360 - m_startAngle - m_endAngle) * (3.14 / 180) / m_scaleMajor;
    double sina,cosa;
    int x, y;
    QFontMetricsF fm(this->font());
    double w, h, tmpVal;
    QString str;

    for (int i = 0; i <= m_scaleMajor; i++)
    {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);

        tmpVal = 1.0 * i *((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue;
        // tmpVal = 50;
        str = QString( "%1" ).arg(tmpVal);  //%1作为占位符   arg()函数比起 sprintf()来是类型安全的
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = 110 * cosa - w / 2;
        y = -110 * sina + h / 4;
        painter->drawText(x, y, str); //函数的前两个参数是显示的坐标位置，后一个是显示的内容，是字符类型""

    }
    painter->restore();
}
void MyMainWindow::drawTitle(QPainter *painter)
{
    painter->save();
    painter->setPen(m_foreground);
    //painter->setBrush(m_foreground);
    QString str(m_title); //显示仪表的功能
    QFontMetricsF fm(this->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    painter->drawText(-w / 2, -30, str);
    painter->restore();
}
void MyMainWindow::drawIndicator(QPainter *painter)
{
    painter->save();
    QPolygon pts;
    pts.setPoints(3, -2, 0, 2, 0, 0, 160);//最后一个参数：指针的长度

    painter->rotate(m_startAngle);
    double degRotate = (360.0 - m_startAngle - m_endAngle) / (m_maxValue - m_minValue)*(m_value - m_minValue);

    //画指针
    painter->rotate(degRotate);  //顺时针旋转坐标系统
    QRadialGradient haloGradient(0, 0, 60, 0, 0);  //辐射渐变
    haloGradient.setColorAt(0, QColor(60, 60, 60));
    haloGradient.setColorAt(1, QColor(160, 160, 160)); //灰
    painter->setPen(Qt::white); //定义线条文本颜色  设置线条的颜色
    painter->setBrush(haloGradient);//刷子定义形状如何填满 填充后的颜色
    painter->drawConvexPolygon(pts); //这是个重载函数，绘制多边形。
    painter->restore();

    //画中心点
    QColor niceBlue(150, 150, 200);
    QConicalGradient coneGradient(0, 0, -90.0);  //角度渐变
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);
    painter->setPen(Qt::NoPen);  //没有线，填满没有边界
    painter->setBrush(coneGradient);
    painter->drawEllipse(-5, -5, 10, 10);
}
void MyMainWindow::drawNumericValue(QPainter *painter)
{
    QString str = QString("%1 %2").arg(m_value, 0, 'f', m_precision).arg(m_units);
    QFontMetricsF fm(this->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    painter->setPen(m_foreground);
    painter->drawText(-w / 2, 42, str);
}
void MyMainWindow::drawLine(QPainter *painter)
{
    painter->setPen(QPen(Qt::gray,2));
    painter->drawLine(0,540,426,540);
    painter->drawLine(0,630,426,630);
    painter->drawLine(0,720,426,720);

    //painter->drawLine(230,-200,588,-200);
    painter->drawLine(213,540,213,720);
    //painter->drawLine(230,350,588,350);
}
//void MyMainWindow::UpdateAngle()
//{
//    //emit this->data_update();//只要角度改变则发送自定义的数据更新信号
//    if(m_value<100)
//    {
//        this->m_value+=1;
//    }
//    else
//    {
//        m_value=0;
//    }
//    this->update();//重绘界面
//}
void MyMainWindow::on_action_triggered()
{
    //chuankou ck;
    ck->show();
}

void MyMainWindow::timeUpdate()//状态栏显示时间
{
    QDateTime currentTime=QDateTime::currentDateTime();
    QString timeStr=currentTime.toString("yyyy\xe5\xb9\xb4-MM\xe6\x9c\x88-dd\xe6\x97\xa5 hh:mm:ss");//年月日
    this->currTimeLable->setText(timeStr);
    ui->statusbar->addWidget(currTimeLable);
    //持续时间
    if(this->jishi==true)
    {
        QString chixu_time=QString::number(start_time.msecsTo(currentTime)/3600000)+"h-"+\
                QString::number(start_time.msecsTo(currentTime)%3600000/60000)+"min-"+\
                QString::number(start_time.msecsTo(currentTime)%60000/1000)+"sec";
        this->ui->label_time->setText(chixu_time);
    }
}

void MyMainWindow::on_action_2_triggered()
{
    this->cmd=new conn_mysql_dialog();
    cmd->exec();
}

void MyMainWindow::on_actionMySQL_triggered()
{
    //this->cmysql=new CONN_Dialog();
    cmysql->exec();
}

void MyMainWindow::dataUpdate()
{
    //    if(notice==0)
    //    {
    //        notice++;
    //        QMessageBox::about(this,"注意","若要存储数据，请连接数据库！");
    //    }

    QSqlQuery query;
    qDebug()<<"query in yes";
    QString value0=this->leixing_str;
    QString value1=QString::number(this->m_value,10,1);

    query.prepare("insert into "+this->tab+" (leixing,shunshi,wendu,yali)"
                  "values (:value0,:value1,:value2,:value3)"
                  );
    query.bindValue(":value0",value0);
    query.bindValue(":value1",value1);
    query.bindValue(":value2",this->wendu);
    query.bindValue(":value3",this->yali);
    //query.exec("insert into data1 (speed) values (22.222)");
    query.exec();
    qDebug()<<"charu ----------------------- yes";
    // qDebug()<<"插入";
    query.clear();
}

void MyMainWindow::on_pushButton_open_clicked()
{
    //设置数据库状态为记录中
    if(status_db==1)
    {
        this->status_db=2;
    }
    //设置累积流量为0
    this->leiji_ll=0;
    //打开计时
    this->jishi=true;
    //设置开始时间
    this->start_time=QDateTime::currentDateTime();
    QString timeStr=start_time.toString("yyyy\xe5\xb9\xb4-MM\xe6\x9c\x88-dd\xe6\x97\xa5 hh:mm:ss");//年月日
    this->ui->label_start_time->setText(timeStr);

    this->m_dataTimer->start(5000);//打开流量计时，打开采集数据的定时器，设置间隔为5秒
    this->ui->pushButton_open->setEnabled(false);
    //  设置串口
    ck->serial = new QSerialPort;
    //设置串口名
    ck->serial->setPortName("COM1");
    //打开串口
    if(ck->serial->open(QIODevice::ReadWrite)==false)
    {
        QMessageBox::critical(this,"串口","打开失败!","确定");
        return;
    }
    else {
        qDebug()<<"yes";
    }
    //设置波特率
    ck->serial->setBaudRate(QSerialPort::Baud9600);//设置波特率为9600

    //设置数据位数
    ck->serial->setDataBits(QSerialPort::Data8);//设置数据位8
    //设置校验位
    ck->serial->setParity(QSerialPort::NoParity);
    //设置停止位
    ck->serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
    //设置流控制
    ck->serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
    //关闭设置菜单使能
    //连接信号槽
    // connect(serial,SIGNAL(readyRead()),this,SLOT(ReadDate()));
    connect(ck->serial,SIGNAL(readyRead()),this,SLOT(data_read_fenxi()));

}

void MyMainWindow::data_read_fenxi()
{
    QByteArray buf;
    QString lx_data;//类型二进制数据
    QString sd_data;//速度二进制数据
    QString wendu_data;//温度二进制数据
    QString yali_data;//压力二进制数据
    buf=this->ck->serial->readAll();
    if(!buf.isEmpty())
    {
        QString str;
        str+=buf;
        qDebug()<<str<<"输入数据";

        lx_data=str.left(3);////////////获取流量计类型
        qDebug()<<"类型数据"<<lx_data;
        leixing=(int)two_to_ten(lx_data,3);
        emit update_leixing();//////////////////更新类型
        qDebug()<<leixing<<"类型";

        sd_data=str.mid(3,13);////////获取瞬时流量
        qDebug()<<"流量数据"<<sd_data;
        speed=two_to_ten(sd_data,sd_data.length());
        emit update_zhen_xh();//发送更改指针的信号
        qDebug()<<speed<<"流量";

        wendu_data=str.mid(16,13);////////获取温度值
        qDebug()<<"温度";
        wendu=two_to_ten(wendu_data,wendu_data.length());
        this->ui->label_wendu->setText(QString::number(wendu,3,1));

        yali_data=str.mid(19,13);////////获取压力
        qDebug()<<"压力";
        yali=two_to_ten(yali_data,yali_data.length());
        this->ui->label_yali->setText(QString::number(yali,3,1));


    }
    buf.clear();
    qDebug()<<"end";
}

void MyMainWindow::on_pushButton_close_clicked()
{
    this->status=0;//设置设备状态为未连接
    this->jishi=false;
    //this->ui->label_start_time->setText("");
//    QSqlDatabase::removeDatabase("QMYSQL");//数据库连接关不掉
//    this->cmysql->db.close();
//    this->ui->label_shuju_status->setText("数据库未连接");
//    this->status_db=0;//设置记录数据状态
    this->status_db=1;//数据库仍然在连接着
    QString style_mysql="color: rgb(129, 129, 129);";
    this->ui->label_shuju_status->setStyleSheet(style_mysql);
    this->ui->label_sv->setText("00.00");

    QString style_shebei="color: rgb(129, 129, 129);";
    this->ui->label_run->setStyleSheet(style_shebei);
    this->m_dataTimer->stop();//关闭流量计时关闭采集数据的定时器,不在采集数据
    this->ck->serial->close();
    this->ui->pushButton_open->setEnabled(true);
    this->m_value=0;
    this->ui->horizontalSlider->setValue(0);
    this->update();
}

void MyMainWindow::on_horizontalSlider_actionTriggered(int)
{

    int nDec=this->ui->horizontalSlider->value();
        int nYushu, nShang;
        QString strBin, strTemp;
        //TCHAR buf[2];
        bool bContinue = true;
        while ( bContinue )
        {
            nYushu = nDec % 2;
            nShang = nDec / 2;
            strBin=QString::number(nYushu)+strBin; //qDebug()<<strBin;
            strTemp = strBin;
            //strBin.Format("%s%s", buf, strTemp);
            nDec = nShang;
            if ( nShang == 0 )
                bContinue = false;
        }
        int nTemp = strBin.length() % 4;
        switch(nTemp)
        {
        case 1:
            //strTemp.Format(_T("000%s"), strBin);
            strTemp = "000"+strBin;
            strBin = strTemp;
            break;
        case 2:
            //strTemp.Format(_T("00%s"), strBin);
            strTemp = "00"+strBin;
            strBin = strTemp;
            break;
        case 3:
            //strTemp.Format(_T("0%s"), strBin);
            strTemp = "0"+strBin;
            strBin = strTemp;
            break;
        default:
            break;
        }
        strBin+="\n";
        this->ck->serial->write(strBin.toLatin1().data());
}

void MyMainWindow::update_ui_lx()
{
    switch (this->leixing)
    {
    case 0:
        leixing_str="\xe5\xb7\xae\xe5\x8e\x8b\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1 | CY3-172034";//差压
        break;
    case 1:
        leixing_str="\xe7\x94\xb5\xe7\xa3\x81\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  | DC3-165984";//电磁
        break;
    case 2:
        leixing_str="\xe5\xae\xb9\xe7\xa7\xaf\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  |  RJ3-542638";//容积
        break;
    case 3:
        leixing_str="\xe6\xb6\xa1\xe8\xbd\xae\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  |  WJ3-689534";//涡街
        break;
    case 4:
        leixing_str="\xe7\xa7\x91\xe9\x87\x8c\xe5\xa5\xa5\xe5\x88\xa9\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  |  KLAL3-963258";//科里奥利
        break;
    case 5:
        leixing_str="\xe8\xb6\x85\xe5\xa3\xb0\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  |  CS3-168764";//超声
        break;
    case 6:
        leixing_str="\xe6\xb6\xa1\xe8\xa1\x97\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  |  WJ3-168495";//涡街
        break;
    case 7:
        leixing_str="\xe7\x83\xad\xe8\xb4\xa8\xe9\x87\x8f\xe6\xb5\x81\xe9\x87\x8f\xe8\xae\xa1  |  RZL3-168005";//热质量
        break;
    default:
        leixing_str="未知类型";
        break;
    }
    this->ui->label_ll->setText(leixing_str);
}

void MyMainWindow::update_zhen()
{
    if((this->speed>=this->run_low)&&(this->speed<=this->run_high))
    {
        qDebug()<<this->run_low<<"---------------------------->"<<this->run_high;
        this->status=1;
        QString style="color: rgb(79, 255, 85);";
        this->ui->label_run->setStyleSheet(style);
    }
    else
    {
        this->status=2;
        QString style="color: rgb(255, 14, 46);";
        this->ui->label_run->setStyleSheet(style);
    }
    this->m_value=this->speed;
    QString sv=QString::number(this->speed,3,1);
    this->ui->label_sv->setText(sv);
    //this->ui->horizontalSlider->setValue(this->speed);//根据表盘数值设置下方调节栏的值，应删除
    this->update();
}

void MyMainWindow::mysql_status()
{
    if(this->status!=0)
    {
        this->status_db=2;
    }
    else
    {
        this->status_db=1;
    }
    QString style="color: rgb(76, 255, 76);";
    this->ui->label_shuju_status->setStyleSheet(style);
}

void MyMainWindow::update_leiji()
{
    this->leiji_ll+=(this->speed/(double)(3600))*1000;
    this->ui->label_leiji->setText(QString::number(leiji_ll,6,1));
}

void MyMainWindow::on_action_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MyMainWindow::on_action_3_triggered()
{
    this->set_ptr->show();
}

void MyMainWindow::set_range()
{
    this->run_low=this->set_ptr->low;
    this->run_high=this->set_ptr->high;
}

void MyMainWindow::set_tab_name()
{
    this->tab=this->cmysql->tab_name;
}
