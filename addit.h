#ifndef ADDIT_H
#define ADDIT_H

#include <QLabel>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "paintplace.h"

class Addit
        :public QLabel
{
    Q_OBJECT

public:
    Addit(QWidget *parent = 0);

    QPropertyAnimation *anim;
    QStackedWidget *widget;

    PaintPlace *pntplace;
    QLabel *winTable;
    QLabel *loseTable;
    QLabel *invalidTable;

    void called(QMap<QString, qint16> *pdata);

protected:
    void mouseDoubleClickEvent(QMouseEvent *pe);
    void resizeEvent(QResizeEvent *pe);

signals:
    void repaintFlag();

};

#endif // ADDIT_H
