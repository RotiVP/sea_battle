#include "statmenu.h"

StatMenu::StatMenu(QWidget *parent)
    :QWidget(parent),
      header(new QLabel(this)),
      table(new QLabel(this))
{
    setStyleSheet(".QWidget {background: white;}");

    QFont font = this->font();
    font.setPointSize(13);

    header->setText("Statistics");
    header->setFont(font);
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("QLabel { background-color: rgb(04, 218, 253) }");

//    table.setMargin(4);
    table->setStyleSheet("QLabel {background-color: rgb(04, 218, 253);}");
    table->setFont(font);

    table->setText(QString("<style> p{font-size: 18px} </style>")+
           "<p>" +
           "WinsP: 0" +
           "<br>WinsB: 0" +
           "<br>Loses: 0" +
           "<br>Shots: 0" +
           "<br>Hits: 0" +
           "<br>Misses: 0" +
           "<br>Ratio h\\s: 1" +
            "</p>" );
}

void StatMenu::resizeEvent(QResizeEvent *pe) {
    header->resize(size().width(), 40);
    table->resize(size().width(), 200);

    //position
    table->move(0, 40*2);
}
