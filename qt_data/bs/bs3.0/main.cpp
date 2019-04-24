#include "chuankou.h"
#include <QApplication>
#include "mymainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    chuankou w;
//    w.show();

    MyMainWindow m;
    m.show();

    return a.exec();
}
