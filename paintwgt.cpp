#include "paintwgt.h"

PaintWgt::PaintWgt(QWidget *parent)
    :QWidget(parent),
      paintMod(false),
      color(Qt::black)
{

}

void PaintWgt::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        paintMod=true;
        tempBuffer<<event->pos();
//        qDebug() << "pos"<< event->pos() <<"local" << event->localPos() <<"global" << event->globalPos() <<event->screenPos()<<event->windowPos();
    }
}

void PaintWgt::mouseReleaseEvent(QMouseEvent *event) {
    if (paintMod==true) {
        tempBuffer<<event->pos();
        paintMod=false;

        repaint();
        oldColors<<color;
        buffers<<tempBuffer;

        tempBuffer.clear();
    }
}

void PaintWgt::mouseMoveEvent(QMouseEvent *event) {

    if (paintMod==true) {
        tempBuffer<<event->pos();
        repaint();
    }
}

void PaintWgt::paintEvent(QPaintEvent *event) {

    QPainter pntr(this);

    pntr.fillRect(rect(), Qt::white);

    pntr.setPen(Qt::NoPen);

    //рисуем

    pntr.setRenderHint(QPainter::Antialiasing, true);

    //рисуем то, что было до

    for (short int i = 0; i<buffers.size(); ++i)
    {
        //меняем кисточки, выбираем старые для старых буферов
        pntr.setPen(QPen((Qt::GlobalColor)oldColors[i], 7, Qt::SolidLine));
        QVector<QPointF>::const_iterator it = buffers[i].constBegin();
        if (buffers[i].size()>1) {
            ++it;
            for (;it!=buffers[i].end(); ++it) {
                pntr.drawLine(*(it-1), *it);
            }
        }
        else if (!buffers[i].isEmpty()) pntr.drawPoint(buffers[i].front());
    }

    //рисуем временный буфер
    pntr.setPen(QPen((Qt::GlobalColor)color, 7, Qt::SolidLine));
    QVector<QPointF>::const_iterator it = tempBuffer.constBegin();
    if (tempBuffer.size()>1) {
        ++it;
        for (;it!=tempBuffer.end(); ++it) {

            pntr.drawLine(*(it-1), *it);
        }
    }
    else if (!tempBuffer.isEmpty()) pntr.drawPoint(tempBuffer.front());


    pntr.end();
}


