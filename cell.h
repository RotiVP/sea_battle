#pragma once

#include <QGraphicsRectItem>
#include <QBrush>
#include <QObject>
#include <QPen>
#include <QGraphicsSceneHoverEvent>

class Cell
        :public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:

    short int num;
    short int id;         //номер массива с кораблем
    short int typeShip;   //тип корабля(по палубам)

    bool friendly;

    Cell(qreal x, qreal y, qreal width, qreal height, qint8 number, QPen pen, QBrush brush, bool _friendly);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);

signals:
    void clicked(qint8 num, bool friendly);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *pe);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *pe);
};


