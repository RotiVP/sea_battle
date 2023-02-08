#pragma once

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

#include "buttonmenu.h"

class Menu
        :public QWidget
{
    Q_OBJECT

    QLabel *about;
    QLabel *hideb;


public:
    Menu(QWidget *parent = 0);
    ButtonMenu *playB;   //play with bot
    ButtonMenu *playP;   //play with people
    ButtonMenu *stat;
    ButtonMenu *sett;
    bool isOpen;

protected:
    void mousePressEvent(QMouseEvent *pe);
    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *pe);

signals:
    void clicked_hideb();

};

