#ifndef PAINTPLACE_H
#define PAINTPLACE_H

#include "paintwgt.h"

class PaintPlace
        :public QWidget
{
public:
    PaintPlace(QWidget *parent = 0);
    PaintWgt *_paintwgt;
    QRect isColor[5];

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void resizeEvent(QResizeEvent *pe);

};

#endif // PAINTPLACE_H
