#include "buttonmenu.h"

ButtonMenu::ButtonMenu(short int textSize, QString _txt, QWidget *parent)
    :QLabel(parent),
      block(false)
{
    txt=_txt;

    QFont font = this->font();
    font.setPointSize(textSize);
    setFont(font);
}

void ButtonMenu::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton)
        move(x()-5, y());
}

void ButtonMenu::mouseReleaseEvent(QMouseEvent *ev) {
    if(ev->button()==Qt::LeftButton) {
        move(x()+5, y());
        if (block!=true)
            emit clicked(txt);
    }
}

void ButtonMenu::paintEvent(QPaintEvent *ev) {

    QPainter pntr(this);

    if (block==false) {
        pntr.fillRect(rect(), QColor(04, 218, 253));
    }
    else {
        pntr.fillRect(rect(), QColor(185, 185, 185));
    }

    pntr.setPen(QPen(Qt::black, 2));
    pntr.drawLine(rect().bottomLeft(), rect().bottomRight());
    pntr.drawText(rect() , Qt::AlignCenter, txt);

}

void ButtonMenu::setBlock(bool is) {
    block=is;
    this->repaint();
}

bool ButtonMenu::isBlock() {
    return block;
}




