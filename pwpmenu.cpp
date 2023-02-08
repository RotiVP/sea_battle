#include "pwpmenu.h"

PWPMenu::PWPMenu(QWidget *parent)
    :QWidget(parent),
      header( new QLabel(this)),
      need( new QLabel(this)),

      cIpLbl( new QLabel(this)),
      cAddr(new QLineEdit(this)),
      cPortLbl(new QLabel(this)),
      cPort(new QLineEdit(this)),
      bConnect(new ButtonMenu(13, "Connect" ,this)),

      lPortLbl(new QLabel(this)),
      lPort(new QLineEdit(this)),
      bListen(new ButtonMenu(13, "Create" ,this)),

      lose(new ButtonMenu(13, "Get lose" ,this))
{

    setStyleSheet(".QWidget {background: white;}");

    QFont font = this->font();
    font.setPointSize(13);

    cAddr->setFont(font);
    cPort->setFont(font);
    lPort->setFont(font);

    header->setText("Play with people");
    header->setStyleSheet("QLabel {background-color: rgb(04, 218, 253)}");
    header->setAlignment(Qt::AlignCenter);
    header->setFont(font);

    need->setAlignment(Qt::AlignCenter);
    need->setText("Ships created");
    need->setStyleSheet("QLabel {background-color: rgb(111, 255, 92) }");
    need->setFont(font);

    //connect
    cIpLbl->setText("IP:");
    cIpLbl->setFont(font);
    cIpLbl->setAlignment(Qt::AlignCenter);
    cAddr->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}"), cAddr));
    cPortLbl->setText("Port:");
    cPortLbl->setFont(font);
    cPortLbl->setAlignment(Qt::AlignCenter);
    cPort->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,4}"), cPort));

    //listen
    lPortLbl->setText("Port:");
    lPortLbl->setFont(font);
    lPortLbl->setAlignment(Qt::AlignCenter);
    lPort->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,4}"), lPort));

    lose->setBlock();

}

void PWPMenu::resizeEvent(QResizeEvent *pe) {
    header->resize(size().width(), 40);
    need->resize(size().width(), 40);

    cIpLbl->resize(35, 30);
    cAddr->resize(width()-cIpLbl->width(), 30);
    cPortLbl->resize(45, 30);
    cPort->resize(width()-cPortLbl->width(), 30);
    bConnect->resize(size().width(), 40);

    lPortLbl->resize(45, 30);
    lPort->resize(width()-lPortLbl->width(), 30);
    bListen->resize(size().width(), 40);

    lose->resize(size().width(), 40);

    //position
    need->move(0, 40);

    cIpLbl->move(0, 40*3);
    cAddr->move(cIpLbl->geometry().topRight());
    cPortLbl->move(cIpLbl->geometry().bottomLeft());
    cPort->move(cPortLbl->geometry().topRight());
    bConnect->move(cPortLbl->geometry().bottomLeft());

    lPortLbl->move(bConnect->geometry().bottomLeft()+QPoint(0, 10));
    lPort->move(lPortLbl->geometry().topRight());
    bListen->move(lPortLbl->geometry().bottomLeft());

    lose->move(bListen->geometry().bottomLeft()+QPoint(0, 10));
}
