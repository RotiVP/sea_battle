#include "settmenu.h"

SettMenu::SettMenu(QWidget *parent)
    :QWidget(parent),
      header(new QLabel(this)),
      lblSize(new QLabel(this)),
      sizeLine(new QLineEdit(this)),
      lblNick(new QLabel(this)),
      nickLine(new QLineEdit(this)),
      apply(new ButtonMenu(13, "Apply", this))
{
    setStyleSheet(".QWidget {background: white;}");

    QFont font = this->font();
    font.setPointSize(13);
    lblSize->setFont(font);
    lblNick->setFont(font);
    header->setFont(font);

    sizeLine->setFont(font);
    nickLine->setFont(font);

    header->setText("Settings");
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("QLabel {background: rgb(04, 218, 253);}");

    lblSize->setText("Cell size:");
    lblSize->setAlignment(Qt::AlignCenter);
    QRegExpValidator *val = new QRegExpValidator(QRegExp("[0-9]{2}"), sizeLine);
    sizeLine->setValidator(val);

    lblNick->setText("Nick:");
    lblNick->setAlignment(Qt::AlignCenter);
    val = new QRegExpValidator(QRegExp("([0-9]|[A-z]){1,13}"), nickLine);
    nickLine->setValidator(val);

}

void SettMenu::resizeEvent(QResizeEvent *pe) {
    header->resize(size().width(), 40);

    lblSize->resize(80, 30);
    sizeLine->resize(size().width()-lblSize->width(), 30);

    lblNick->resize(50, 30);
    nickLine->resize(size().width()-lblNick->width(), 30);

    apply->resize(size().width(), 40);

    //position
    lblSize->move(0, 40*2);
    sizeLine->move(lblSize->geometry().topRight());

    lblNick->move(lblSize->geometry().bottomLeft());
    nickLine->move(lblNick->geometry().topRight());

    apply->move(lblNick->geometry().bottomLeft());
}
