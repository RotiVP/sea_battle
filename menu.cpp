#include "menu.h"

Menu::Menu(QWidget *parent)
    :QWidget(parent),
      isOpen(false),
      about(new QLabel(this)),
      playB(new ButtonMenu( 13, "Play with bot" ,this) ),
      playP(new ButtonMenu( 13, "Play with people" ,this) ),
      stat(new ButtonMenu( 13, "Statistics" ,this) ),
      sett(new ButtonMenu( 13, "Settings" ,this) ),
      hideb(new QLabel(this))

{
    hideb->setStyleSheet("QLabel{background-color: rgb(04, 218, 253) ;"
                         "border-bottom: 2px solid black }");

    QSize defaultSize(150, 40);

    playB->move(0, defaultSize.height()*2);
    playP->move(0, defaultSize.height()*3);
    stat->move(0, defaultSize.height()*5);
    sett->move(0, defaultSize.height()*6);

    QPainter pntr;
    QImage imgHideMenu(defaultSize, QImage::Format_ARGB32_Premultiplied);
    imgHideMenu.fill(0);
    pntr.begin(&imgHideMenu);
    pntr.setPen(QPen(Qt::black, 3));
    QRect itemRect2 = imgHideMenu.rect();
    pntr.drawLines(QVector<QPoint>() << itemRect2.topLeft()+QPoint(55, 20) << itemRect2.topRight()-QPoint(55, -20) );
    pntr.end();

    hideb->setPixmap(QPixmap::fromImage(imgHideMenu));

    about->setText("Created by\nPanchishin Ivan\nBuild: 26032017");
    about->setAlignment(Qt::AlignCenter);
    about->setStyleSheet("QLabel {color: black;}");

    QFont font = this->font();
    font.setPointSize(11);
    about->setFont(font);
}

void Menu::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::black, 3));

    painter.drawLine(rect().topRight(), rect().bottomRight());

    painter.end();

}

void /*virtual*/ Menu::mousePressEvent(QMouseEvent *pe)
{

    if (hideb->geometry().contains(pe->pos())) {
        emit clicked_hideb();
    }

}

void Menu::resizeEvent(QResizeEvent *pe) {
    playB->resize(width()-2, 40);
    playP->resize(width()-2, 40);
    stat->resize(width()-2, 40);
    sett->resize(width()-2, 40);
    hideb->resize(width()-2, 40);

    about->resize(width()-2, 100);
    about->move(0, size().height()-100);

}

