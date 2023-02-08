#include "cell.h"

Cell::Cell(qreal x, qreal y, qreal width, qreal height, qint8 number, QPen pen, QBrush brush, bool _friendly)
    :QGraphicsRectItem(x, y, width, height),

      num(number),
      id(-1),
      typeShip(-1),
      friendly(_friendly)
{
    setPen(pen);
    setBrush(brush);
//    setAcceptHoverEvents(true);
}


void Cell::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    emit clicked(num, friendly);
}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent *pe) {
//    if(brush()==Qt::white)
//        setBrush(Qt::lightGray);
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *pe) {
//    if(brush()==Qt::lightGray)
//        setBrush(Qt::white);
}
