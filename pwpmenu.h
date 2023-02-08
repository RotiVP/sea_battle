#ifndef PWPMENU_H
#define PWPMENU_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>

#include "buttonmenu.h"

class PWPMenu
        :public QWidget
{


public:
    PWPMenu(QWidget *parent =0);
    QLabel *header;
    QLabel *need;

    QLabel *cIpLbl;
    QLineEdit *cAddr;
    QLineEdit *cPort;
    ButtonMenu *bConnect;

    QLabel *lPortLbl;
    QLineEdit *lPort;
    ButtonMenu *bListen;
    ButtonMenu *lose;

    QLabel *cPortLbl;

protected:
    void resizeEvent(QResizeEvent *pe);
};

#endif // PWPMENU_H
