#include "chat.h"
#include <QDebug>

Chat::Chat(QWidget *parent)
    :QLabel(parent),
      chatSize(350, 250),
      moveMod(false),
      phideBtn(new QLabel),
      pted(new QTextEdit),
      pled(new QLineEdit),
      pbtn(new QLabel)

{
    resize(chatSize);
    pbtn->setFixedSize(80, 35);
    pbtn->setText("Send");
    pbtn->setAlignment(Qt::AlignCenter);

    pted->setReadOnly(true);

    QBoxLayout *mlay = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *field = new QBoxLayout(QBoxLayout::LeftToRight);

    header = new QLabel;
    header->setFixedHeight(30);
    header->setText("Chat");
    header->setAlignment(Qt::AlignCenter);
    phideBtn->setFixedSize(30,30);

    QImage hidepix(phideBtn->size(), QImage::Format_ARGB32_Premultiplied);
    QPainter pntr(&hidepix);
    pntr.setPen(QPen(Qt::black, 3));
    pntr.drawLine(0+5, hidepix.height()/2, hidepix.width()-5, hidepix.height()/2);
//    pntr.drawLine(0, 0, 0, hidepix.height());
    pntr.end();
    phideBtn->setPixmap(QPixmap::fromImage(hidepix));

    field->addWidget(header);
    field->addWidget(phideBtn);
    mlay->addLayout(field);
    field=new QBoxLayout(QBoxLayout::LeftToRight);

    mlay->addWidget(pted);

    field->addWidget(pled);
    field->addWidget(pbtn);
    field->setMargin(0);
    mlay->addLayout(field);

    mlay->setMargin(0);
    mlay->setSpacing(0);
    this->setLayout(mlay);

    setStyleSheet("QLabel {background: rgb(04, 218, 253);}"
                  "Chat {border: 3px solid black;}"
                  "QTextEdit{border-top: 2px solid black;}");

    QFont font = this->font();
    font.setPointSize(12);
    pted->setFont(font);
    pled->setFont(font);
    header->setFont(font);
    pbtn->setFont(font);

    connect(pled, SIGNAL(returnPressed()), SLOT(sChatRelease()));

}

void Chat::sChatRelease() {
    if(pled->text()!="") {
        pted->append(QString("<font color=green>You: </font>")+pled->text());
        emit toSend();
        pled->clear();
    }
}

void Chat::mousePressEvent(QMouseEvent *pe) {
    if(pe->button()==Qt::LeftButton && phideBtn->geometry().contains(pe->pos())) {

    }

    else if(pe->button()==Qt::LeftButton && header->geometry().contains(pe->pos())) {
        pressPos = pe->pos();
        moveMod=true;
//        qDebug() << pe->pos() <<"pos" << pe->globalPos() << "g" << pe->screenPos()<<"screen" << pe->localPos() <<"local" << pe->windowPos() <<"wind";
    }
    else if (pe->button()==Qt::LeftButton && pbtn->geometry().contains(pe->pos())) {
        sChatRelease();
    }
}

void Chat::mouseMoveEvent(QMouseEvent *pe) {
    if(pe->buttons()==Qt::LeftButton && moveMod==true) {
        QPoint dpos = pe->pos()-pressPos;
        this->move(this->pos()+dpos);
    }
    parentWidget()->repaint();
}

void Chat::mouseReleaseEvent(QMouseEvent *pe) {
    if(moveMod==true) {moveMod=false;}
    else if(pe->button()==Qt::LeftButton && phideBtn->geometry().contains(pe->pos())) {
        if(pbtn->isHidden()) {
            pbtn->show(); pted->show(); pled->show(); this->resize(chatSize);
        }
        else {
            pbtn->hide(); pted->hide(); pled->hide(); this->resize(this->width(), header->height()+6);
            parentWidget()->repaint();
        }
    }
}
