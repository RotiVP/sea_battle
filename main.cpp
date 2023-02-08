#include <QApplication>

#include "mainwgt.h"

#include <QScrollArea>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWgt shell;

//    shell.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    shell.setWindowTitle("Sea_Fight_2.1");

    shell.show();

//    QScrollArea sa;
//    QLabel *lbl = new QLabel;
//    lbl->setPixmap(QPixmap(":/cogwheel.png"));
//    sa.setWidget(lbl);
//    sa.show();
//    sa.viewport()->setFixedSize(100, 100);

//    int *r = new int[10];
//    qDebug() << r[3];

//    qDebug() << ( true && (true || true || false) );
//    QList<int> d; d<<0<<1<<2<<3<<4<<5;
//    for(int i = 0; i<3; ++i) {qDebug() << d.takeAt(2);};

    int *g  = new int(3);
//    delete g; delete g;
//    delete g; if(g){delete g;}

//    delete g; g=nullptr; if(g){delete g;} верно
//    if(g){delete g; g=nullptr;}; if(g){delete g;} //тоже верно

    return a.exec();
}
