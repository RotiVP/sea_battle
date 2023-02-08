#ifndef PAINTWGT_H
#define PAINTWGT_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>

class PaintWgt
        :public QWidget
{
public:
    PaintWgt(QWidget *parent = 0);
    bool paintMod;
    QVector<QPointF> tempBuffer;
    QVector< QVector<QPointF> > buffers;
    QVector<short int> oldColors;

    short int color;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);
};

#endif // PAINTWGT_H
