#ifndef BUTTONMENU_H
#define BUTTONMENU_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class ButtonMenu
        :public QLabel
{
    Q_OBJECT

    bool block;
    QString txt;

public:
    ButtonMenu(short int textSize, QString _txt, QWidget *parent);
    void setBlock(bool is = true);
    bool isBlock();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);


signals:
    void clicked(QString);
};

#endif // BUTTONMENU_H
