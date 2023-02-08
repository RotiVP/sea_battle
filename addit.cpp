#include "addit.h"

Addit::Addit(QWidget *parent)
    :QLabel(parent),
      widget(new QStackedWidget(this)),
      pntplace(new PaintPlace),
      winTable(new QLabel(this)),
      loseTable(new QLabel(this)),
      invalidTable(new QLabel(this))
{
    setStyleSheet("QLabel {background: rgb(0,0,0,150) }");

    QGraphicsOpacityEffect *goe = new QGraphicsOpacityEffect;
    goe->setOpacity(0);
    this->setGraphicsEffect(goe);

    anim = new QPropertyAnimation(goe, "opacity");
    anim->setDuration(300);
    anim->setEasingCurve(QEasingCurve::OutQuad);

    hide();

    connect(anim, &QPropertyAnimation::finished, [this, goe](){
        if(goe->opacity()==0) {hide();}
    } );

    widget->resize(400, 300);
    widget->addWidget(pntplace);
    widget->addWidget(winTable);
    widget->addWidget(loseTable);
    widget->addWidget(invalidTable);

    winTable->setStyleSheet("QLabel {background-color: rgb(235,255,235);"
                            "        border: 4px solid rgb(04, 218, 253);}");
    winTable->setAlignment(Qt::AlignCenter);

    loseTable->setStyleSheet("QLabel {background-color: rgb(255,235,235);"
                             "        border: 4px solid rgb(04, 218, 253);}");
    loseTable->setAlignment(Qt::AlignCenter);

    invalidTable->setStyleSheet("QLabel {background-color: rgb(220,220,220);"
                                "        border: 4px solid rgb(04, 218, 253);}");
    invalidTable->setAlignment(Qt::AlignCenter);
}

void Addit::called(QMap<QString, qint16> *pdata) {
    winTable->setText("<style> p.t {color: green; font-size: 50px;} p {font-size: 20px;} </style>"+
            QString("<p class=\"t\">Win</p>")+
            "<p> "
            "<br>Shot: " + QString::number(pdata->value("Shot", 0)) +
            "<br>Hit: "+QString::number(pdata->value("Hit", 0)) +
            "<br>Miss: "+QString::number(pdata->value("Miss", 0)) +
            "<br>Ratio hit\\shot: "+QString::number((float)pdata->value("Hit", 1) / pdata->value("Shot", 1), 'g', 2)+
            "</p>");
    loseTable->setText("<style> p.t {color: red; font-size: 50px;} p {font-size: 20px;}  </style>"+
            QString("<p class=\"t\">Lose</p>")+
            "<p> "
            "<br>Shot: " + QString::number(pdata->value("Shot", 0)) +
            "<br>Hit: "+QString::number(pdata->value("Hit", 0)) +
            "<br>Miss: "+QString::number(pdata->value("Miss", 0)) +
            "<br>Ratio hit\\shot: "+QString::number((float)pdata->value("Hit", 1) / pdata->value("Shot", 1), 'g', 2)+
            "</p>");
    invalidTable->setText("<style> p.t {color: red; font-size: 50px;} p {font-size: 20px;}  </style>"+
            QString("<p class=\"t\">Error</p>")+
            "<p>"
            "<br>Sorry, error of connect."
            "<br>Disconnected."
            "</p>");
    pdata->remove("Shot");
    pdata->remove("Hit");
    pdata->remove("Miss");
}

void Addit::mouseDoubleClickEvent(QMouseEvent *pe) {
    if(pe->button()==Qt::LeftButton) {
        anim->setEndValue(0);
        anim->start();
        if(widget->currentIndex()==0) {
            emit repaintFlag();
        }
    }
}

void Addit::resizeEvent(QResizeEvent *pe) {
    QSize wgtsize = widget->size();
    widget->move(QPoint(width()/2, height()/2)-QPoint(wgtsize.width()/2, wgtsize.height()/2));
}
