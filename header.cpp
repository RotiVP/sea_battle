#include "header.h"

Header::Header(QWidget *parent)
    :QLabel(parent),
      lblOpenMenu(new QLabel(this)),
      secondColor(04, 218, 253)
{

    lblOpenMenu->resize(45, 35);

    QPainter painter;

    QImage menuImg(lblOpenMenu->size(), QImage::Format_ARGB32_Premultiplied);
    menuImg.fill(0);
    QRect imgRect = menuImg.rect();

    painter.begin(&menuImg);
    painter.setPen(QPen(Qt::black, 4));
    painter.drawLines(QVector<QPoint>() << imgRect.topLeft()+QPoint(10,8) << imgRect.topRight()-QPoint(10,-8)
                                        << imgRect.topLeft()+QPoint(10,17) << imgRect.topRight()-QPoint(10,-17)
                                        << imgRect.topLeft()+QPoint(10,26) << imgRect.topRight()-QPoint(10,-26));
    painter.end();

    lblOpenMenu->setPixmap(QPixmap::fromImage(menuImg));

    QHBoxLayout *play = new QHBoxLayout;
    play->setMargin(0);
    play->addWidget(lblOpenMenu);
    play->addStretch(1);
    this->setLayout(play);

}

void Header::mouseReleaseEvent(QMouseEvent *ev) {

    if (lblOpenMenu->geometry().contains(ev->pos())) {
        emit clicked_bOpenMenu();
    }

}

void Header::paintEvent(QPaintEvent *ev) {
    QPainter painter(this);
    painter.fillRect(rect(), secondColor);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(rect().bottomLeft(), rect().bottomRight());

    QFont font = this->font(); font.setPointSize(16);
    painter.setFont(font);

    painter.drawText(rect(), Qt::AlignCenter, "Sea Fight 2");
    painter.end();
}



