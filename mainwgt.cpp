#include "mainwgt.h"
#include <QDebug>

MainWgt::MainWgt(QWidget *parent)
    : QWidget(parent),
      sizeside(40),
      cellCount(10),
      oneFieldView(new QGraphicsView(this)),
      twoFieldView(new QGraphicsView(this)),
      lblfont(this->font()),
      shipCount(0),
      plblState(new QLabel(this)),
      plblTime(new QLabel(this)),
      plblScore(new QLabel(this)),
      plblFightState(new QLabel(this)),
      uflag(new QLabel(this)),
      eflag(new QLabel(this)),
      unick(new QLabel(this)),
      enick(new QLabel(this)),
      _header(new Header(this)),
      _addit(new Addit(this)),
      blockBg(new QLabel(this)),
      _menu2(new QStackedWidget(this)),
      _pwbmenu(new PWBMenu),
      _settmenu(new SettMenu),
      _pwpmenu(new PWPMenu),
      _statmenu(new StatMenu),
      _menu(new Menu(this)),
      blockSize(0),
      lblWaitConnect(new QLabel(this)),
      psocket(nullptr),
      secondColor("04, 218, 253"),
      _gamestate(Creating),
      _fightside(Stop),
      _chat(new Chat(this))
{
    for(qint8 i =0; i<2; ++i){
        seconds[i]=0;
        minutes[i]=0;
    }

    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, [this]{
        ++seconds[0];
        if(seconds[0]==10){++seconds[1]; seconds[0]=0;}
        if(seconds[1]==6){++minutes[0]; seconds[1]=0;}
        if(minutes[0]==10){++minutes[1]; minutes[0]=0;}
        if(minutes[1]==6){timer.stop();}
        plblTime->setText(QString("Time:\n%1%2:%3%4").arg(minutes[1]).arg(minutes[0]).arg(seconds[1]).arg(seconds[0]));
    });
    plblTime->setAlignment(Qt::AlignCenter);
    plblTime->setText(QString("Time:\n%1%2:%3%4").arg(minutes[1]).arg(minutes[0]).arg(seconds[1]).arg(seconds[0]));
    plblTime->setStyleSheet("QLabel {background: white; border: 3px solid rgb(04, 218, 253); }");

    plblScore->setAlignment(Qt::AlignCenter);
    plblScore->setText(QString("Score:\n0:0"));
    plblScore->setStyleSheet("QLabel {background: white; border: 3px solid rgb(04, 218, 253); }");

    plblFightState->setAlignment(Qt::AlignCenter);
    plblFightState->setStyleSheet("QLabel {background: white; border: 3px solid rgb(04, 218, 253); }");

    uships = new QVector<Cell*>*[4];
    for(int i = 0, j = 4; i<4 && j>0; ++i, --j) {
        uships[i] = new QVector<Cell*>[j];
    }
    eships = new QVector<Cell*>*[4];
    for(int i = 0, j = 4; i<4 && j>0; ++i, --j) {
        eships[i] = new QVector<Cell*>[j];
    }

    uflag->setStyleSheet("QWidget {background: white; border: 3px solid rgb(04, 218, 253);}");
    MyEventFilter *mef = new MyEventFilter(uflag);
    uflag->installEventFilter(mef);
    connect(mef, &MyEventFilter::dbClick, [this]{
        if(_gamestate!=Creating) {return;}
        _addit->widget->setCurrentIndex(0);
        _addit->show();
        _addit->anim->setEndValue(1);
        _addit->anim->start();
    } );

    eflag->setStyleSheet("QWidget {background: white; border: 3px solid rgb(04, 218, 253);}");
    unick->setStyleSheet("QLabel {background: white; border-bottom: 3px solid rgb(04, 218, 253);}");
    unick->setAlignment(Qt::AlignCenter);
    enick->setStyleSheet("QLabel {background: white; border-bottom: 3px solid rgb(04, 218, 253);}");
    enick->setAlignment(Qt::AlignCenter);

    _menu->move(-150, 0);
    _menu2->move(-150, 0);
    _menu2->addWidget(_pwbmenu);
    _menu2->addWidget(_pwpmenu);
    _menu2->addWidget(_statmenu);
    _menu2->addWidget(_settmenu);

    connect(_header, SIGNAL(clicked_bOpenMenu()), SLOT(sOpenMenu()));
    connect(_menu, SIGNAL(clicked_hideb()), SLOT(sOpenMenu()));

    _menu2->setCurrentIndex(0);

    blockBg->setStyleSheet("QLabel {background: rgb(0,0,0,150);}");
    blockBg->hide();

    connect(_settmenu->apply, SIGNAL(clicked(QString)), SLOT(sSaveSettings()));

    connect(_menu->playB, SIGNAL(clicked(QString)), SLOT(sToggleMenu(QString)));
    connect(_menu->playP, SIGNAL(clicked(QString)), SLOT(sToggleMenu(QString)));
    connect(_menu->sett, SIGNAL(clicked(QString)), SLOT(sToggleMenu(QString)));
    connect(_menu->stat, SIGNAL(clicked(QString)), SLOT(sToggleMenu(QString)));

    QGraphicsOpacityEffect* eff = new QGraphicsOpacityEffect;
    eff->setOpacity(0);
    blockBg->setGraphicsEffect(eff);

    oneFieldView->resize(sizeside*cellCount, sizeside*cellCount);

    oneScene = new QGraphicsScene(oneFieldView->rect());
    oneFieldView->setScene(oneScene);
    oneFieldView->setStyleSheet("QGraphicsView {border: none;}");
    for(qint16 i = 0; i<(sizeside*cellCount); i+=sizeside) {
        for(qint16 j = 0; j<(sizeside*cellCount); j+=sizeside) {
            Cell *item = new Cell( j, i, sizeside, sizeside, i/sizeside*cellCount+j/sizeside, QPen(Qt::blue, 1), Qt::white, true );
            oneScene->addItem(item);
            connect(item, SIGNAL(clicked(qint8, bool)), SLOT(sClickedCell(qint8, bool)));
            uitems << item;

        }
    }

    twoFieldView->resize(sizeside*cellCount, sizeside*cellCount);

    twoScene = new QGraphicsScene(twoFieldView->rect());
    twoFieldView->setScene(twoScene);
    twoFieldView->setStyleSheet("QGraphicsView {border: none;}");
    for(qint16 i = 0; i<(sizeside*cellCount); i+=sizeside) {
        for(qint16 j = 0; j<(sizeside*cellCount); j+=sizeside) {
            Cell *item = new Cell( j, i, sizeside, sizeside, i/sizeside*cellCount+j/sizeside, QPen(Qt::blue, 1), Qt::white, false );
            twoScene->addItem(item);
            connect(item, SIGNAL(clicked(qint8, bool)), SLOT(sClickedCell(qint8, bool)));
            eitems<<item;
        }
    }

    plblState->setStyleSheet(QString("QLabel {background: rgba(255, 255, 255, 200);}"));

    connect(_pwbmenu->play, &ButtonMenu::clicked, [this]{
        _gamestate=BPlaying; sStartGame();
    });
    connect(_pwbmenu->lose, &ButtonMenu::clicked, [this]{
       shipCount=0;
       for(qint8 i = 0, j = 3; i<4 && j>=0; ++i, --j) {
           for(qint8 k = j; k>=0; --k)
           {
               for(auto it=uships[i][k].begin(); it!=uships[i][k].end(); ++it) {
                    (*it)->setBrush(Qt::black);
                    if((*it)->id!=-1 || (*it)->typeShip!=-1) {
                        (*it)->id=-1;
                        (*it)->typeShip=-1;
                    }
               }
           }
       }
       sOpenMenu();
       endGame();
    });

    connect(_pwpmenu->lose, &ButtonMenu::clicked, [this]{
       shipCount=0;
       for(qint8 i = 0, j = 3; i<4 && j>=0; ++i, --j) {
           for(qint8 k = j; k>=0; --k)
           {
               for(auto it=uships[i][k].begin(); it!=uships[i][k].end(); ++it) {
                    (*it)->setBrush(Qt::black);
                    if((*it)->id!=-1 || (*it)->typeShip!=-1) {
                        (*it)->id=-1;
                        (*it)->typeShip=-1;
                    }
               }
           }
       }
       sOpenMenu();
       endGame();
    });

    connect(_addit, &Addit::repaintFlag, [this]{
       uflag->setPixmap(_addit->pntplace->_paintwgt->grab().scaled(uflag->size()));
    });

    uflag->setScaledContents(true);
    eflag->setScaledContents(true);


    MyEventFilter *mef2 = new MyEventFilter(lblWaitConnect);
    lblWaitConnect->installEventFilter(mef2);

    lblWaitConnect->setStyleSheet("QLabel {background: rgba(0,0,0,100); color: white;}");
    lblWaitConnect->setText("Waiting of connect...\nDouble click that cancel");
    lblWaitConnect->setAlignment(Qt::AlignCenter);
    lblWaitConnect->hide();
    QFont font = this->font();
    font.setPointSize(16);
    lblWaitConnect->setFont(font);

    QGraphicsOpacityEffect *goeff = new QGraphicsOpacityEffect;
    goeff->setOpacity(0);
    lblWaitConnect->setGraphicsEffect(goeff);

    QPropertyAnimation *anim = new QPropertyAnimation(goeff, "opacity");
    anim->setDuration(400);
    anim->setEasingCurve(QEasingCurve::OutQuad);

    connect(anim, &QPropertyAnimation::finished, [this, goeff](){
        if(goeff->opacity()==0) {lblWaitConnect->hide(); server.close();}
    } );

    connect(_pwpmenu->bListen, &ButtonMenu::clicked, [this, anim] {
        if(QRegExp("[0-9]{1,4}").exactMatch(_pwpmenu->lPort->text())) {
            lblWaitConnect->show();
            anim->setEndValue(1);
            anim->start();
            server.listen(QHostAddress::Any, _pwpmenu->lPort->text().toInt());
        }
    });

    connect(mef2, &MyEventFilter::dbClick, [this, anim]{
        anim->setEndValue(0);
        anim->start();
    } );

    connect(&server, &QTcpServer::newConnection, [this, mef2] {
        emit mef2->dbClick();
        sNewConnection();
    });

    connect(_pwpmenu->bConnect, &ButtonMenu::clicked, [this] {
        if(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}").exactMatch(_pwpmenu->cAddr->text()) &&
               QRegExp("[0-9]{1,4}").exactMatch(_pwpmenu->cPort->text())) {

            psocket = new QTcpSocket(this);

//            void (QAbstractSocket::*signalf)(QAbstractSocket::SocketError socketError) = &QAbstractSocket::error;
//            connect(psocket, signalf, [this]{
//                if(psocket) {psocket->disconnect(); psocket->deleteLater();}
//            });

            connect(psocket, SIGNAL(connected()), this, SLOT(sConnected()) );

            psocket->connectToHost(_pwpmenu->cAddr->text(), _pwpmenu->cPort->text().toInt());

            connect(psocket, &QAbstractSocket::stateChanged, [this]{
               if(psocket->state() == QAbstractSocket::UnconnectedState) { //если состояние измениться на неподкюченное(неудачное подкючение)
                    psocket->disconnect(); psocket->deleteLater(); psocket=nullptr;
               }
            });
        }
    });

    connect(_chat, &Chat::toSend, [this]{
        if(psocket) {
           QByteArray ba;
           QDataStream ds(&ba, QIODevice::WriteOnly);
           ds<<qint16(0)<<QString("message")<<_chat->pled->text();
           qDebug() << _chat->pled->text();
           ds.device()->seek(0);
           ds<<(qint16)(ba.size()-sizeof(qint16));
           psocket->write(ba);
       }
    });
}

void MainWgt::sConnected() {

    psocket->disconnect(); //сбрасываем проверку не неудачное подключение
    connect(psocket, SIGNAL(readyRead()), SLOT(readSocket()));
    connect(psocket, SIGNAL(disconnected()), this, SLOT(sDisconnected()));

    QByteArray ba;
    QDataStream dstrm(&ba, QIODevice::WriteOnly);

    QList<qint8> shipPos;
    for(qint8 i=0, j=3; i<4 && j>=0; ++i, --j)
        for(qint8 k = j; k>=0; --k) {
            for(auto it = uships[i][k].begin(); it!=uships[i][k].end(); ++it) {
                shipPos<<(*it)->num;
                qDebug() << "W" << (*it)->num << (*it)->typeShip << (*it)->id;
            }
        }
    qDebug() << shipPos.size();
    QPixmap flag = uflag->grab();
    QString nick = unick->text();
    _fightside=User; drawFightSide();

    dstrm<<qint16(0)<<shipPos<<flag<<nick;
    dstrm.device()->seek(0);
    dstrm<<(qint16)(ba.size()-sizeof(qint16));

    psocket->write(ba);
}

void MainWgt::readSocket() {
    QDataStream ds(psocket);

    qDebug() << ds.device()->size();

    //принимаем ПАКЕТ. Данные приходят не пакетом
    if(!blockSize) {
        if (psocket->bytesAvailable()<sizeof(qint16)) {return;}
        else ds>>blockSize; //blockSize теперь ОБНУЛЯТЬ НЕЛЬЗЯ, тк данные из blockSize, отвечающие за его размер УЖЕ ВЫТАЩЕНЫ из него.
    }
    if(psocket->bytesAvailable() < blockSize) {return;}

    if(_gamestate==Creating) {

        QList<qint8> pos;
        QPixmap flag;
        QString nick;
        ds>>pos>>flag>>nick;

        enick->setText(nick);
        eflag->setPixmap(flag);

        for(qint8 i = 0, j=3; i<4 && j>=0; ++i, --j) {
            for(qint8 k = j; k>=0; --k ) {
                eships[i][k].clear();
                for(qint8 m = i; m>=0; --m) {
                    eships[i][k]<<eitems[pos.takeFirst()];
                    eships[i][k].back()->id=k;
                    eships[i][k].back()->typeShip=i;
                    qDebug() << "R" << eships[i][k].back()->num << eships[i][k].back()->typeShip << eships[i][k].back()->id;
                }
            }
        }
        qDebug() <<pos;

       _gamestate=PPlaying;
       eshipCount=20;
       sStartGame();
    }
    else {
        QString type;
        ds>>type;
        if(type=="shot"){
            qint8 num; ds>>num;
            shot("e", num);
        }
        else if(type=="message") {
            QString mes; ds>>mes;
            _chat->pted->append("<font color=red>Enemy: </font>"+mes);
        }

    }

    blockSize=0;
}

qint8 MainWgt::shot(QString side, qint8 num) {
    QVector<Cell*> *items = (side=="e")?&uitems:&eitems;
    QVector<Cell*> **ships = (side=="e")?uships:eships;
    qint8 &countDeck = (side=="e")?shipCount:eshipCount;

    if(side!="e") mapStat["Shots"]=mapStat.value("Shots", 0)+1;
    if(side!="e") mapStat["Shot"]=mapStat.value("Shot", 0)+1;

    if((*items)[num]->typeShip!=-1) {

        if(side!="e") mapStat["Hits"]=mapStat.value("Hits", 0)+1;
        if(side!="e") mapStat["Hit"]=mapStat.value("Hit", 0)+1;

        (*items)[num]->setBrush(Qt::red);
        --countDeck;

        bool isDead=true;
        qint8 id = (*items)[num]->id, typeShip = (*items)[num]->typeShip;
        (*items)[num]->id=-1;
        (*items)[num]->typeShip=-1;
        for(auto it = ships[typeShip][id].begin(); it!=ships[typeShip][id].end(); ++it) {
            qDebug() <<(*it)->num;
            if((*it)->brush()!=Qt::red) {
                isDead=false;
                break;
            }
        }
        if(isDead) {
            for(auto it = ships[typeShip][id].begin(); it!=ships[typeShip][id].end(); ++it) {
                (*it)->setBrush(Qt::black);
            }
            if(countDeck==0) {
                endGame();
                _fightside=Stop; drawFightSide();
                return 3;
            }
            return 2;
        }
        return 1;
    }
    if(side!="e") mapStat["Miss"]=mapStat.value("Miss", 0)+1;
    if(side!="e") mapStat["Misses"]=mapStat.value("Misses", 0)+1;
    (*items)[num]->setBrush(Qt::cyan);
    (side=="e")?_fightside=User:_fightside=Enemy;
    drawFightSide();
    return 0;
}

void MainWgt::sendShot(qint8 numDeck) {
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds<<qint16(0)<<QString("shot")<<numDeck;
    ds.device()->seek(0);
    ds<<(qint16)(ba.size()-sizeof(qint16));
    psocket->write(ba);
}

void MainWgt::sNewConnection() {
    psocket=server.nextPendingConnection();
    connect(psocket, SIGNAL(readyRead()), SLOT(readSocket()));
    connect(psocket, SIGNAL(disconnected()), this, SLOT(sDisconnected()));

    QByteArray ba;
    QDataStream dstrm(&ba, QIODevice::WriteOnly);

    QList<qint8> shipPos;
    for(qint8 i=0, j=3; i<4 && j>=0; ++i, --j)
        for(qint8 k = j; k>=0; --k) {
            for(auto it = uships[i][k].begin(); it!=uships[i][k].end(); ++it) {
                shipPos<<(*it)->num;
            }
        }

    QPixmap flag = uflag->grab();
    QString nick = unick->text();
    _fightside=Enemy; drawFightSide();//определяем, кто первый стреляет

    dstrm<<qint16(0)<<shipPos<<flag<<nick;
    dstrm.device()->seek(0);
    dstrm<<(qint16)(ba.size()-sizeof(qint16));

    psocket->write(ba);
}

void MainWgt::sDisconnected() {
    if(eshipCount!=0 && shipCount!=0) {          //аварийный дисконнект. Обычный вызывается в endGame
        _fightside=Stop; drawFightSide();       //обычно это делается в shot
        endGame();
    }

    psocket->disconnect(); psocket->deleteLater(); psocket=nullptr;
    qDebug() << "Disconnected";
}

void MainWgt::removeArea(QList<qint8> &range, QList<Cell*> &ship, bool horizontal) {

    //сортируем по возростанию
    QList<qint8> intShip;
    for (int i = 0; i<ship.size(); ++i) {
        intShip << ship[i]->num;
    }
    for(int i = 1; i<intShip.size(); ++i) {
        for(int j=i; j>0; --j) {
            if(intShip[j]<intShip[j-1]) {
                int tmp = intShip[j-1];
                intShip[j-1]=intShip[j];
                intShip[j]=tmp;
            }
            else {break;}
        }
    }

    if (
        ( horizontal==true && ( intShip.back()/10==floor( (intShip.back()+1)/(float)10 ) ) ) ||
        ( horizontal==false && (intShip.back()+10<100) )
       )
        intShip.push_back(intShip.back()+( 10+(-10+1)*horizontal ));
    if (
        ( horizontal==true && ( intShip.front()/10==floor( (intShip.front()-1)/(float)10 ) ) ) ||
        ( horizontal==false && (intShip.front()-10>=0) )
       )
        intShip.push_front(intShip.front()-( 10+(-10+1)*horizontal ));

    for(qint8 l = 0; l<intShip.size(); l++) {
        for (qint8 u = -1+(1-10)*horizontal; u<=1+(-1+10)*horizontal; u+=1+(-1+10)*horizontal) {
            if ( (intShip[l]/10==floor((intShip[l]+u)/(float)10) )+horizontal ) { //проверка на переход на новую строку
                range.removeOne(intShip[l]+u);
//                if(ship[0]->friendly && intShip[l]+u<100 && intShip[l]+u>=0)
//                    uitems[intShip[l]+u]->setBrush(Qt::yellow);
            }
        }
    }

}

void MainWgt::rankE() {

    srand( time(0) );

    QList<qint8> range;               //допустимые значения
    for (qint8 i = 0; i<100; i++) {
        range<<i;
    }

    QList<Cell*> tempShip;
    qint8 numShip;        //номер палубного корабля
    qint8 countDeck;

    for(qint8 countShip = 10; countShip>0; countShip--) {             //номер корабля

        bool result = false;

        if(countShip==10) {countDeck=4; numShip=0;}                                 //определение количество палуб
        else if(countShip==9) {countDeck=3; numShip=0;}
        else if(countShip==7) {countDeck=2; numShip=0;}
        else if(countShip==4) {countDeck=1; numShip=0;}

        qint8 side;//сторона
        while (result==false) {                                     //выполнять, пока подбор не будет верным

            qint8 position = range[rand() % range.size()];      //позиция
            side = rand() % 2 + 1;

            result = setShip(tempShip, side, position, countDeck, range);

        }

        numShip++;

//        отмечаем корабль
//        QList<Cell*>::iterator it = tempShip.begin();
//        for (;it!=tempShip.end();++it) {
//            (*it)->setBrush(Qt::green);
//        }

        //заполнение

        eships[countDeck-1][numShip-1] = tempShip.toVector();
        QVector<Cell*>::iterator it2 = eships[countDeck-1][numShip-1].begin();
        for (;it2!=eships[countDeck-1][numShip-1].end(); ++it2){
            (*it2)->id=numShip-1; (*it2)->typeShip=countDeck-1;
        }

        //удаление из range, чтоб не было повторений при удачном рандоме
        removeArea(range, tempShip, ((side==1)?0:1) );

        //отчистка
        tempShip.clear();
    }

    eshipCount=20;

}

bool MainWgt::setShip(QList<Cell*> &tempShip, qint8 &side, qint8 &position, qint8 countDeck, QList<qint8> &range)
{

    tempShip<<eitems[position];
    qint8 forward = 1;
    for (qint8 i = 1; i<countDeck; ++i) {
        switch(side) {
            case 1:
            {
                auto it = std::find(range.begin(), range.end(), position+10*i*forward);
                if (it!=range.end()) {                                      //в одну сторону
                    tempShip<<eitems[*it];
                }
                else if(forward>0){ forward=-1; countDeck-=(i-1); i=(1-1);}        //и в другую
                else { tempShip.clear(); return false;}
                break;
            }
            case 2:
            {
                auto it = std::find(range.begin(), range.end(), position+1*i*forward);
                if (it!=range.end() && position/10 == floor((position+1*i*forward)/(float)10) ) {  //обязательно должен находится в одной строке
                    tempShip<<eitems[*it];
                }

                else if(forward>0){ forward=-1; countDeck-=(i-1); i=(1-1);}
                else { tempShip.clear(); return false;}
                break;
            }
        }
    }
    return true;
}

void MainWgt::sStartGame() {
    if(_gamestate==BPlaying){
        for(qint8 i = 0; i<100; ++i)
            validPos<<i;

        rankE();

        enick->setText("Mr.BOT");
        QImage botFlag(eflag->size(), QImage::Format_ARGB32_Premultiplied);
        botFlag.fill(0);
        QPainter pntr(&botFlag);
        pntr.setPen(QPen(Qt::red, 5));
        pntr.setRenderHint(QPainter::Antialiasing, true);
        pntr.drawLines(QVector<QPoint>() << QPoint(0, 0) << QPoint(botFlag.width(), botFlag.height()) <<
                       QPoint(botFlag.width(), 0) << QPoint(0, botFlag.height()) );
        pntr.end();
        eflag->setPixmap(QPixmap::fromImage(botFlag));
        _fightside=User; drawFightSide();

        _pwpmenu->lose->setBlock(true);
        _pwbmenu->lose->setBlock(false);

    }
    if(_gamestate==PPlaying){
        _pwpmenu->lose->setBlock(false);
        _pwbmenu->lose->setBlock(true);

    }

    for(auto it = eitems.begin(); it!=eitems.end(); ++it) {
        if((*it)->brush()!=Qt::white) {
            (*it)->setBrush(Qt::white);
        }
    }

    for(qint8 i = 0; i<2; ++i) {
        seconds[i]=0;
        minutes[i]=0;
    }
    timer.start();
    plblScore->setText(QString("Score:\n%1:%2").arg(score.value(enick->text(), 0)/10).arg(score.value(enick->text(), 0)%10));

    _pwbmenu->play->setBlock(true);
    _pwpmenu->bListen->setBlock(true);
    _pwpmenu->bConnect->setBlock(true);
    sOpenMenu();

}

bool MainWgt::toCountShip() {

    if (shipCount!=20) {return false;}

    qint8 count = 1;
    qint8 four = 0, three = 0, two = 0, one = 0;

    QList<Cell*> ships;

    for (qint8 i = 0; i<10; i++) {
        for (qint8 j = 0; j<10; j++) {

            if (!ships.contains(uitems[i*10+j])) {

                bool maybeone=true;
                //вперед
                if( j+1<10 &&
                    !ships.contains(uitems[i*10+j]) &&
                    uitems[i*10+j]->brush()==Qt::green &&
                    uitems[i*10+j+count]->brush()==Qt::green) {

                    maybeone=false;
                    ships<<uitems[i*10+j];

                    while(j+count<10 && uitems[i*10+j+count]->brush()==Qt::green) {
                        ships<<uitems[i*10+j+count];
                        count++;
                    }
                }
                //вниз
                else if( i+1<10 &&
                         uitems[i*10+j]->brush()==Qt::green &&
                         uitems[(i+count)*10+j]->brush()==Qt::green) {

                    maybeone=false;
                    ships<<uitems[i*10+j];

                    while(i+count<10 && uitems[(i+count)*10+j]->brush()==Qt::green) {
                        ships<<uitems[(i+count)*10+j];
                        count++;
                    }
                }

                if (count>4) { return false;}

                qint8 id=-1, typeShip=-1;
                //не однопалубный
                if(maybeone==false) {

                    if (count==4){four++; if(four>1){return false;} }
                    else if (count==3){three++; if(three>2){return false;} }
                    else if(count==2){two++; if(two>3){return false;}}

                    id = (count==4)?four-1:(count==3)?three-1:(count==2)?two-1:-1;
                    typeShip = count-1;
                }

                //однопалоубный возможно
                else if (uitems[i*10+j]->brush()==Qt::green &&
                            maybeone==true) {
                   one++;
                   ships<<uitems[i*10+j];

                   if(one>4){return false;}

                   id = one-1;
                   typeShip = count-1; //==0;
                }

                if (id!=-1 && typeShip!=-1) {
                //добавляем в массив
                    uships[typeShip][id].clear();
                    for(int i = count-1; i>=0; --i) {
                        uships[typeShip][id]<<ships[ships.size()-1-i];
                        ships[ships.size()-1-i]->id=id;
                        ships[ships.size()-1-i]->typeShip=typeShip;
                    }
                }

            }
            count =1;
        }
    }

    if (one==4 && two==3 && three==2 && four==1) {return true;}
    else {return false;}
}

void MainWgt::sClickedCell(qint8 num, bool friendly) {
    if(_gamestate==Creating) {
        if (friendly) {
            bool block=false;
            if (uitems[num]->brush() == Qt::white) {
                if (shipCount<20 ) {
                    if(num+10<100) {
                        if(floor((float)(num+1)/10)==num/10 && uitems[num+10+1]->brush()==Qt::green){
                            block=true;
                        }
                        else if(floor((float)(num-1)/10)==num/10 && uitems[num+10-1]->brush()==Qt::green) {
                            block=true;
                        }
                    }

                    if (num-10>=0) {
                        if (floor((float)(num+1)/10)==num/10 && uitems[num-10+1]->brush()==Qt::green) {
                            block=true;
                        }
                        else if(floor((float)(num-1)/10)==num/10 && uitems[num-10-1]->brush()==Qt::green) {
                            block=true;
                        }
                    }
                    if (block==false) {
                        uitems[num]->setBrush(Qt::green);
                        shipCount++;
                    }
                    else if (block==true) {
                        block=false;
                    }
                }
            }

            else if(uitems[num]->brush() == Qt::green) {
                uitems[num]->setBrush(Qt::white);
                shipCount--;
            }
        }
    }
    else if (_gamestate==BPlaying || _gamestate==PPlaying) {
        if(!friendly) {
            if(eitems[num]->brush()==Qt::white && _fightside==User) {

                if(_gamestate==PPlaying) {
                    sendShot(num);
                }
                qint8 result = shot("u", num);

                if(result==0) {
                    if(_gamestate==BPlaying) {
                        while(eshot()){};
                    }
                    else //PPlaying
                    {

                    }
                }
            }
        }
    }

}

void MainWgt::endGame() {
    if(_gamestate==BPlaying) {
        if(shipCount==0) {
            score["Mr.BOT"]=score.value("Mr.BOT", 0)+1;
            _addit->widget->setCurrentIndex(2);
            mapStat["Loses"]=mapStat.value("Loses", 0)+1;
        }
        else if(eshipCount == 0) {
            //пользовательские палубы не обнулять, они создаются синхронизировано при расстановке и уменьшаются при ранении
            score["Mr.BOT"]=score.value("Mr.BOT", 0)+10;
            _addit->widget->setCurrentIndex(1);
            mapStat["WinsB"]=mapStat.value("WinsB", 0)+1;
        }

        validPos.clear(); //заполнятеся в startGame
        //колчичество палуб заполняется при расстановке
        //указатели на динамические массивы: пользовательский отчищается при заполнении, бота- копируется при заполнении оператором присвоения
        //id and typeShip приравнивается -1 также синхронизированно с уничтножением корабля
    }
    else if(_gamestate==PPlaying) {
        if(shipCount==0) {
            score[enick->text()]=score.value(enick->text(), 0)+1;
            _addit->widget->setCurrentIndex(2);
            mapStat["Loses"]=mapStat.value("Loses", 0)+1;
            psocket->disconnectFromHost();
        }
        else if(eshipCount == 0) {
            //пользовательские палубы не обнулять, они создаются синхронизировано при расстановке и уменьшаются при ранении
            score[enick->text()]=score.value(enick->text(), 0)+10;
            _addit->widget->setCurrentIndex(1);
            mapStat["WinsP"]=mapStat.value("WinsP", 0)+1;
            psocket->disconnectFromHost();
        }


        //при аварийном дисконекте
        else {
            for(qint8 i = 0, j = 3; i<4 && j>=0; ++i, --j) {
                for(qint8 k = j; k>=0; --k)
                {
                    for(auto it=eships[i][k].begin(); it!=eships[i][k].end(); ++it) {
                        if((*it)->brush()==Qt::white)
                            (*it)->setBrush(Qt::green);
                        if((*it)->id!=-1 || (*it)->typeShip!=-1) {
                            (*it)->id=-1;
                            (*it)->typeShip=-1;
                        }
                    }
                }
            }
            eshipCount=0;
            _addit->widget->setCurrentIndex(3);
        }
    }

    if(shipCount==0) {
        for(qint8 i = 0, j = 3; i<4 && j>=0; ++i, --j) {
            for(qint8 k = j; k>=0; --k)
            {
                for(auto it=eships[i][k].begin(); it!=eships[i][k].end(); ++it) {
                    if((*it)->brush()==Qt::white)
                        (*it)->setBrush(Qt::green);
                    if((*it)->id!=-1 || (*it)->typeShip!=-1) {
                        (*it)->id=-1;
                        (*it)->typeShip=-1;
                    }
                }
            }
        }
        eshipCount=0;
    }
    else if(eshipCount == 0) { //пользователь победил, у неубитых кораблей пользователя остались идентификаторы. Их нужно убрать
        for(qint8 i = 0, j = 3; i<4 && j>=0; ++i, --j) {
            for(qint8 k = j; k>=0; --k)
            {
                for(auto it=uships[i][k].begin(); it!=uships[i][k].end(); ++it) {
                    if((*it)->id!=-1 || (*it)->typeShip!=-1) {
                        (*it)->id=-1;
                        (*it)->typeShip=-1;
                    }
                }
            }
        }
    }

    for(auto it = uitems.begin(); it!=uitems.end(); ++it) {
        if((*it)->brush()!=Qt::green && (*it)->brush()!=Qt::white) {
            (*it)->setBrush(Qt::white);
        }
    }

    _gamestate=Creating;
    plblScore->setText(QString("Score:\n%1:%2").arg(score.value(enick->text(), 0)/10).arg(score.value(enick->text(), 0)%10));
    timer.stop();
    _pwbmenu->lose->setBlock(true);
    _pwpmenu->lose->setBlock(true);
    _pwbmenu->play->setBlock(false);
    _pwpmenu->bListen->setBlock(false);
    _pwpmenu->bConnect->setBlock(false);

    _addit->called(&mapStat);
    _addit->show();
    _addit->anim->setEndValue(1);
    _addit->anim->start();

    _statmenu->table->setText(QString("<style> p{font-size: 18px} </style>")+
            "<p>"
            "WinsP: " + QString::number(mapStat.value("WinsP", 0)) +
            "<br>WinsB: "+QString::number(mapStat.value("WinsB", 0)) +
            "<br>Loses: "+QString::number(mapStat.value("Loses", 0)) +
            "<br>Shots: " + QString::number(mapStat.value("Shots", 0)) +
            "<br>Hits: "+QString::number(mapStat.value("Hits", 0)) +
            "<br>Misses: "+QString::number(mapStat.value("Misses", 0)) +
            "<br>Ratio h\\s: "+QString::number((float)mapStat.value("Hits", 1) / mapStat.value("Shots", 1), 'g', 2)+
            "</p>" );
}

bool MainWgt::eshot() {

    srand(time(0));

    static bool inj = false;            //ранение

    static QList<qint8> startPos;   //возможные позиции расположения второй палубы
    static QList<qint8> startPosFirst;

    static QList<Cell*> tempDeck;       //временная коллекция атакуемого корабля

//    static qint8 typeShip;          //тип корабля
    qint8 randomPos;         //зарандомленная позиция

    static qint8 verticalMod = -1;  //эта строка исполнится строго один раз, тк переменная статическая
    static qint8 forwardMod = 0;

     //начало, ищем первую палубу

    qint8 one=0, two=0, three=0, four=0; //убитые корабли юзера
    for(qint8 i = 0, j=3; i<4 && j>=0; ++i, --j) {
        for(qint8 k=j; k>=0; --k) {
            if(uships[i][k].front()->id==-1) {
                (i==0)? ++one : (i==1)? ++two : (i==2)? ++three : ++four;
            }
        }
    }
    QList<qint8> posForRand;

        if (inj==false) {

            qint8 nShip = (four!=1)?4:(three!=2)?3:(two!=3)?2:1;

            //искать позиции, где потенциально может находится самый крупный корабль
            for(qint8 i = 0; i<validPos.size(); ++i) {
                bool toArr=true;
                bool horizontal = rand() % 2;
                for(int n = 0; n<2; ++n) {
                    for(qint8 j = 0; j<nShip; ++j) {
                        if(horizontal==true && ( (i+j)>=validPos.size()
                                                 || !validPos.contains(validPos[i]+j)
                                                 || validPos[i]/10!=(validPos[i]+j)/10
                                                 || posForRand.contains(validPos[i]+j)
                                               ) ) {
                            toArr=false;
                            break;
                        }
                        else if(horizontal==false && ( (i+j)>=validPos.size() || !validPos.contains(validPos[i]+j*10) || posForRand.contains(validPos[i]+j*10)  ) ) {
                            toArr=false;
                            break;
                        }
                    }
                    if(toArr) {
                        for(qint8 j = 0; j<nShip; ++j) {
                            (horizontal)?posForRand << validPos[i]+j:posForRand << validPos[i]+j*10;
                        }
                        break;
                    }
                    else {
                        (horizontal==1)?horizontal=0:horizontal=1;
                        toArr=true;
                    }
                }
            }

            qint8 size = posForRand.size()-1;
            if(size!=0)
                randomPos = posForRand[rand()% size];
            else {randomPos = posForRand[0];}

            qint8 result = shot("e", randomPos);

            if (result==0) {
                validPos.removeOne(randomPos);
                return false;
            }
            else if(result==1) { //ранел
                tempDeck << uitems[randomPos];
                inj=true;
                if (validPos.contains(randomPos+10)) {
                    if(posForRand.contains(randomPos+10)) { startPosFirst << randomPos+10; }
                    else { startPos << randomPos+10; }
                }
                if (validPos.contains(randomPos-10)) {
                    if(posForRand.contains(randomPos-10)) { startPosFirst << randomPos-10; }
                    else { startPos << randomPos-10; }
                }
                if (validPos.contains(randomPos-1) && floor((float)(randomPos-1)/10)==randomPos/10) {
                    if(posForRand.contains(randomPos-1)) { startPosFirst << randomPos-1; }
                    else { startPos << randomPos-1; }
                }
                if (validPos.contains(randomPos+1) && floor((float)(randomPos+1)/10)==randomPos/10) {
                    if(posForRand.contains(randomPos+1)) { startPosFirst << randomPos+1; }
                    else { startPos << randomPos+1; }
                }
            }
            else if (result==2) {
                //убираем позиции, на которых корабль не может находится
                removeArea(validPos, (QList<Cell*>()<<uitems[randomPos]), 1);
                return true;
            }
            else if(result==3) {
                return false;
            }

        }

        //ищем вторую палубу и определяем направление дальнейшего боя
        if (inj==true && verticalMod == -1 && forwardMod == 0) {

            qint8 side; //сторона
            qDebug() << startPos << startPosFirst;
            qint8 size = startPos.size()-1;
            if(!startPosFirst.isEmpty()) {
                side = startPosFirst.takeFirst();
            }
            else if(size!=0)
                side = startPos.takeAt(rand() % size);
            else {side=startPos.takeFirst();}

            qint8 result = shot("e", side);

            if (result==0) {
                validPos.removeOne(side);
                return false;
            }
            else if(result==1) { //ранел
                if (tempDeck[0]->num < uitems[side]->num)
                    tempDeck << uitems[side];
                else tempDeck.push_front(uitems[side]);

                if (tempDeck[1]->num - tempDeck[0]->num == 10)
                    verticalMod=1;
                else
                    verticalMod=0;

                forwardMod = (rand()%2==0)?-1:1;        //определение, в какую сторону стрелять
                startPos.clear();
                startPosFirst.clear();
            }
            else if (result==2) {

                tempDeck << uitems[side];

                if ((tempDeck[1]->num - tempDeck[0]->num)%10==0)
                    verticalMod=1;
                else
                    verticalMod=0;

                removeArea(validPos, tempDeck, 1-verticalMod);
                verticalMod=-1;
                tempDeck.clear();
                inj=false;
                startPos.clear();
                startPosFirst.clear();
                return true;
            }
            else if(result==3) {
                tempDeck.clear();
                inj=false;
                startPos.clear();
                startPosFirst.clear();
                return false;
            }
        }


        if (inj==true && verticalMod != -1 && forwardMod != 0) {
            for(;;) {
                if ( (validPos.contains(tempDeck[ (forwardMod==-1)?0:tempDeck.size()-1 ]->num+(1+(-1+10)*verticalMod)*forwardMod) ) &&
                     (
                      (verticalMod==0 && floor((float)(tempDeck[ (forwardMod==-1)?0:tempDeck.size()-1 ]->num+1*forwardMod)/10) == tempDeck.front()->num/10 ) ||
                      (verticalMod==1)
                     )
                   )
                {
                    qint8 pos = tempDeck[ (forwardMod==-1)?0:tempDeck.size()-1 ]->num+(1+(-1+10)*verticalMod)*forwardMod;

                    qint8 result = shot("e", pos);

                    if (result==0) {
                        forwardMod = (forwardMod==-1)?1:-1;
                        validPos.removeOne(pos);
                        return false;
                    }
                    else if(result==1) { //ранел
                        (forwardMod==1)?tempDeck.push_back(uitems[pos]):tempDeck.push_front(uitems[pos]);
                        return true;
                    }
                    else if (result==2) {
                        (forwardMod==1)?tempDeck.push_back(uitems[pos]):tempDeck.push_front(uitems[pos]);
                        removeArea(validPos, tempDeck, 1-verticalMod);
                        tempDeck.clear();
                        inj=false;
                        verticalMod = -1;
                        forwardMod = 0;
                        return true;
                    }
                    else if(result==3) {
                        tempDeck.clear();
                        inj=false;
                        verticalMod = -1;
                        forwardMod = 0;
                        return false;
                    }
                }
                else {forwardMod = (forwardMod==-1)?1:-1;}
            }
        }

}

void MainWgt::sToggleMenu(QString name) {
    if(name=="Play with bot") {
        _menu2->setCurrentIndex(0);
    }
    else if (name=="Play with people") {
        _menu2->setCurrentIndex(1);
    }
    else if (name=="Statistics") {
        _menu2->setCurrentIndex(2);
    }
    else if (name=="Settings") {
        _menu2->setCurrentIndex(3);
    }
}

void MainWgt::sSaveSettings() {
    if(_settmenu->sizeLine->text()!="" &&
            _settmenu->sizeLine->text().toInt()!=sizeside &&
            _settmenu->sizeLine->text().toInt()>=15 &&
            _settmenu->sizeLine->text().toInt()<=40) {

        sizeside=_settmenu->sizeLine->text().toInt();

        oneFieldView->resize(sizeside*cellCount, sizeside*cellCount);
        oneScene->setSceneRect(oneFieldView->rect());
        twoFieldView->resize(sizeside*cellCount, sizeside*cellCount);
        twoScene->setSceneRect(oneFieldView->rect());

        QPoint center(size().width()/2, size().height()/2);
        twoFieldView->move(center + QPoint(QPoint(3*sizeside, -(cellCount/2)*sizeside)));
        oneFieldView->move(center - QPoint(QPoint(3*sizeside+cellCount*sizeside, (cellCount/2)*sizeside)));

        for(qint16 i = 0; i<(sizeside*cellCount); i+=sizeside) {
            for(qint16 j = 0; j<(sizeside*cellCount); j+=sizeside) {
                eitems[(i/sizeside)*cellCount+j/sizeside]->setRect(j, i, sizeside, sizeside);
                uitems[(i/sizeside)*cellCount+j/sizeside]->setRect(j, i, sizeside, sizeside);
            }
        }


        QApplication::sendEvent(this, new QResizeEvent(size(), size()));
        repaint();
    }

    if(_settmenu->nickLine->text()!="" && _settmenu->nickLine->text()!=unick->text()) {
        unick->setText(_settmenu->nickLine->text());
    }
}

void MainWgt::sOpenMenu() {
    QPropertyAnimation *panim = new QPropertyAnimation(_menu, "pos");
    QPropertyAnimation *panim2 = new QPropertyAnimation(_menu2, "pos");

    QGraphicsOpacityEffect *popEf = dynamic_cast<QGraphicsOpacityEffect*>(blockBg->graphicsEffect());
    QPropertyAnimation *panimGE = new QPropertyAnimation(popEf, "opacity");
    if(!_menu->isOpen){
        blockBg->show();
        if(_gamestate==Creating) {
            if(toCountShip()) {
                _pwbmenu->need->setStyleSheet("QLabel {background: rgb(111, 255, 92);}");
                _pwpmenu->need->setStyleSheet("QLabel {background: rgb(111, 255, 92);}");
                _pwbmenu->play->setBlock(false);
                _pwpmenu->bConnect->setBlock(false);
                _pwpmenu->bListen->setBlock(false);
            }
            else {
                _pwbmenu->need->setStyleSheet("QLabel {background: rgb(255, 100, 100);}");
                _pwpmenu->need->setStyleSheet("QLabel {background: rgb(255, 100, 100);}");
                _pwbmenu->play->setBlock(true);
                _pwpmenu->bConnect->setBlock(true);
                _pwpmenu->bListen->setBlock(true);
            }
        }
    }

    panim->setEndValue((_menu->isOpen)?QPoint(-_menu->width(), 0):QPoint(0, 0));
    panim->setDuration(400);
    panim->setEasingCurve(QEasingCurve::OutQuad);

    panim2->setEndValue((_menu->isOpen)?QPoint(-_menu->width(), 0):QPoint(_menu->width(), 0));
    panim2->setDuration(400);
    panim2->setEasingCurve(QEasingCurve::OutQuad);

    panimGE->setEndValue((_menu->isOpen)?0:1);
    panimGE->setDuration(400);
    panimGE->setEasingCurve(QEasingCurve::OutQuad);

    QParallelAnimationGroup *panimGrp = new QParallelAnimationGroup;
    panimGrp->addAnimation(panim);
    panimGrp->addAnimation(panim2);
    panimGrp->addAnimation(panimGE);
    panimGrp->start(QAbstractAnimation::DeleteWhenStopped);

    (_menu->isOpen)?_menu->isOpen=false:_menu->isOpen=true;

    connect(panimGrp, &QParallelAnimationGroup::finished, [this]{ if(!_menu->isOpen){blockBg->hide();} });
}

void MainWgt::resizeEvent(QResizeEvent *pe) {

    QPoint center(pe->size().width()/2, pe->size().height()/2);
    twoFieldView->move(center + QPoint(QPoint(3*sizeside, -(cellCount/2)*sizeside)));
    oneFieldView->move(center - QPoint(QPoint(3*sizeside+cellCount*sizeside, (cellCount/2)*sizeside)));

    plblState->resize(4*sizeside+1, height());
    plblState->move(center.x()-plblState->width()/2, 0);

    _header->resize(this->width(), 35);

    _menu->resize(150, this->height());
    _menu2->resize(150, this->height());

    blockBg->resize(size());

    _pwbmenu->resize(150, this->height());
    _settmenu->resize(150, this->height());
    _pwpmenu->resize(150, this->height());
    _statmenu->resize(150, this->height());

    _addit->resize(size());

    uflag->resize(5*sizeside, 5*sizeside);
    uflag->move(oneFieldView->geometry().bottomRight()-QPoint(uflag->width()/2, 0)-QPoint(oneFieldView->width()/2, -sizeside));

    eflag->resize(5*sizeside, 5*sizeside);
    eflag->move(twoFieldView->geometry().bottomRight()-QPoint(uflag->width()/2, 0)-QPoint(oneFieldView->width()/2, -sizeside));

    lblfont.setPixelSize(sizeside/1.7);

    unick->resize(5*sizeside, sizeside);
    unick->move(oneFieldView->geometry().topRight()-QPoint(unick->width()/2, 0)-QPoint(oneFieldView->width()/2, sizeside*2));
    unick->setFont(lblfont);

    enick->resize(5*sizeside, sizeside);
    enick->move(twoFieldView->geometry().topRight()-QPoint(unick->width()/2, 0)-QPoint(oneFieldView->width()/2, sizeside*2));
    enick->setFont(lblfont);

    plblTime->resize(4*sizeside, 2*sizeside);
    plblTime->setFont(lblfont);
    plblTime->move(center-QPoint(plblTime->width()/2, center.y()-unick->pos().y()));

    plblScore->resize(4*sizeside, 2*sizeside);
    plblScore->setFont(lblfont);
    plblScore->move(center-QPoint(plblScore->width()/2, 0));

    plblFightState->resize(2*sizeside, 2*sizeside);
    plblFightState->move(center-QPoint(plblFightState->width()/2, center.y()-uflag->pos().y()-uflag->height()/4));

    drawFightSide();

    lblWaitConnect->resize(size());

    _chat->move(center-QPoint(_chat->width()/2, _chat->height()/2));
}

void MainWgt::drawFightSide() {
    QImage stateimg(plblFightState->size()-QSize(sizeside/3, sizeside/3), QImage::Format_ARGB32_Premultiplied);
    stateimg.fill(0);
    QPainter pntr(&stateimg);
    pntr.setBrush(QColor(04, 218, 253));
    pntr.setPen(Qt::NoPen);
    pntr.setRenderHint(QPainter::Antialiasing, true);

    if(_fightside==User)
        pntr.drawPolygon(QVector<QPoint>()<<QPoint(stateimg.width(), stateimg.height()) << QPoint(stateimg.width(), 0) << QPoint(0, stateimg.height()/2));
    else if(_fightside==Enemy)
        pntr.drawPolygon(QVector<QPoint>()<<QPoint(0, 0) <<QPoint(0, stateimg.height()) << QPoint(stateimg.width(), stateimg.height()/2));
    else if(_fightside==Stop) {
        pntr.drawRect(QRect(QPoint(0, 0), QPoint(stateimg.width()/3, stateimg.height())));
        pntr.drawRect(QRect(QPoint(stateimg.width(), 0), QPoint(stateimg.width()-stateimg.width()/3, stateimg.height())));
    }
    plblFightState->setPixmap(QPixmap::fromImage(stateimg));
    pntr.end();

}

void MainWgt::paintEvent(QPaintEvent *pe) {
    QRect drwrct = pe->rect();

    QPainter pntr(this);
    pntr.fillRect(drwrct, Qt::white);

    QVector<QPoint> points;

    pntr.setPen(QPen(Qt::blue, 1));
    QPoint pntcenter(drwrct.width() / 2, drwrct.height() / 2);

    for (short int i = pntcenter.x(); i < drwrct.width(); i += sizeside)
        points << QPoint(i, 0) << QPoint(i, drwrct.height());

    for(short int i = pntcenter.x(); i>0; i-=sizeside)
        points<<QPoint(i, 0)<<QPoint(i, drwrct.height());

    for(short int i = pntcenter.y(); i<drwrct.height(); i+=sizeside)
        points<<QPoint(0, i)<<QPoint(drwrct.width(), i);

    for(short int i = pntcenter.y(); i>0; i-=sizeside)
        points<<QPoint(0, i)<<QPoint(drwrct.width(), i);

    pntr.drawLines(points);
    points.clear();

    pntr.setPen(QPen(Qt::black, 5));
    pntr.setFont(lblfont);
    QPoint leftPointField = pntcenter + QPoint(3*sizeside, -(cellCount/2)*sizeside);
    for(int n = 0; n<2; ++n ) {
        for(short int i = 0; i<=sizeside*cellCount; i+=sizeside) {
            points<<(QPoint(i, 0)+leftPointField)<<(QPoint(i, sizeside*cellCount)+leftPointField);
            if (i<sizeside*cellCount) {
                QRect rect((QPoint(i, 0)+leftPointField), QPoint(i, 0)+leftPointField+QPoint(sizeside, -sizeside));
                pntr.drawText(rect, Qt::AlignCenter, QChar(Qt::Key_A+i/sizeside));
            }

            points<<(QPoint(0, i)+leftPointField)<<(QPoint(sizeside*cellCount, i)+leftPointField);
            if (i<sizeside*cellCount) {
                QRect rect((QPoint(0, i)+leftPointField), QPoint(0, i)+leftPointField-QPoint(sizeside, -sizeside));
                pntr.drawText(rect, Qt::AlignCenter, (i==sizeside*cellCount-sizeside)?QString("10"):QChar(Qt::Key_1+i/sizeside));
            }
        }
        leftPointField = pntcenter - QPoint(3*sizeside+cellCount*sizeside, (cellCount/2)*sizeside);
    }


    pntr.drawLines(points);
}
