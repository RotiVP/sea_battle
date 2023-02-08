#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>

#include "buttonmenu.h"

class SettMenu
        :public QWidget
{

public:
    SettMenu(QWidget *parent = 0);
    QLabel *header;

    QLabel *lblSize;
    QLineEdit *sizeLine;

    QLabel *lblNick;
    QLineEdit *nickLine;

    ButtonMenu *apply;


protected:
    void resizeEvent(QResizeEvent *pe);
};
