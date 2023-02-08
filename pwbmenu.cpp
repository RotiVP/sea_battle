#include "pwbmenu.h"

PWBMenu::PWBMenu(QWidget *parent)
    :QWidget(parent),
      header(new QLabel(this)),
      need(new QLabel(this)),
      play(new ButtonMenu(13, "Start" ,this)),
      lose(new ButtonMenu(13, "Get lose" ,this))
{
    QFont font = this->font();
    font.setPointSize(13);
    need->setFont(font);
    header->setFont(font);

    setStyleSheet(".QWidget {background: white;}");

    header->setText("Play with bot");
    header->setStyleSheet("QLabel {background-color: rgb(04, 218, 253)}");
    header->setAlignment(Qt::AlignCenter);

    need->setAlignment(Qt::AlignCenter);
    need->setText("Ships created");
    need->setStyleSheet("QLabel {background-color: rgb(111, 255, 92) }");

    //rect не меняется после move, поэтому используем geometry

    lose->setBlock();
}

void PWBMenu::resizeEvent(QResizeEvent *pe) {
    header->resize(size().width(), 40);
    need->resize(size().width(), 40);
    play->resize(size().width(), 40);
    lose->resize(size().width(), 40);

    //position
    need->move(0, 40);
    play->move(0, 40*3);
    lose->move(0, 40*4);
}

