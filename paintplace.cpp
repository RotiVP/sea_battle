#include "paintplace.h"

PaintPlace::PaintPlace(QWidget *parent)
    :QWidget(parent),
      _paintwgt(new PaintWgt(this))
{
}

void PaintPlace::mouseReleaseEvent(QMouseEvent *ev) {
    if (isColor[0].contains(ev->pos())) {
        _paintwgt->color=Qt::black;
    }
    else if (isColor[1].contains(ev->pos())) {
        _paintwgt->color=Qt::green;
    }
    else if (isColor[2].contains(ev->pos())) {
        _paintwgt->color=Qt::blue;
    }
    else if (isColor[3].contains(ev->pos())) {
        _paintwgt->color=Qt::red;
    }
    else if (isColor[4].contains(ev->pos())) {
        _paintwgt->color=Qt::white;
    }
}

void PaintPlace::paintEvent(QPaintEvent *event) {
    QPainter pntr(this);
    pntr.fillRect(rect(), Qt::gray);

    pntr.fillRect(isColor[0], Qt::black);
    pntr.fillRect(isColor[1], Qt::green);
    pntr.fillRect(isColor[2], Qt::blue);
    pntr.fillRect(isColor[3], Qt::red);
    pntr.fillRect(isColor[4], Qt::white);

    pntr.setPen(QPen(QColor(04, 218, 253), 10));
    pntr.drawRect(rect());
    pntr.drawLine(_paintwgt->geometry().topRight(), _paintwgt->geometry().bottomRight());

    pntr.end();
}

void PaintPlace::resizeEvent(QResizeEvent *pe) {
    _paintwgt->resize(height()-10, height()-10);
    _paintwgt->move(5, 5);

    int heightcell = height()/(sizeof(isColor)/sizeof(QRect));
    int widthcell = this->width()-_paintwgt->width();

    for (int i = 0; i<(sizeof(isColor)/sizeof(QRect)); ++i) {
        isColor[i] = QRect(_paintwgt->width(), i*heightcell, widthcell, heightcell);
    }
}
