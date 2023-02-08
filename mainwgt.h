#pragma once

#include <QLabel>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include <cmath>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>

#include "chat.h"
#include "header.h"
#include "menu.h"
#include "pwbmenu.h"
#include "settmenu.h"
#include "pwpmenu.h"
#include "statmenu.h"
#include "paintwgt.h"
#include "addit.h"
#include "myeventfilter.h"
#include "cell.h"

class MainWgt
        :public QWidget
{

    Q_OBJECT

    QPoint pressPnt;

    qint8 sizeside;
    qint8 cellCount;

    QGraphicsView *oneFieldView;
    QGraphicsScene *oneScene;
    QGraphicsView *twoFieldView;
    QGraphicsScene *twoScene;

    QFont lblfont;

    QVector<Cell*>eitems;
    QVector<Cell*>uitems;
    qint8 shipCount, eshipCount;
    QVector<Cell*> **uships;  //корабли игрока //[2][0] - трехпалубный первый корабль
    QVector<Cell*> **eships;  //корабли бота
    QList<qint8> validPos;      //позиции потенциальных палуб для бота

    QLabel *plblState;

    QLabel *plblTime;
    QTimer timer;
    qint8 seconds[2];
    qint8 minutes[2];
    QLabel *plblScore;
    QLabel *plblFightState;
    QMap<QString, qint8> score;
    QMap<QString, qint16> mapStat; //Shots, Shot, WinsP, WinsB, Loses, Lose, Miss, Misses, Hit, Hits

    QLabel *uflag, *eflag;
    QLabel *unick, *enick;

    Header *_header;
    Addit *_addit;

    QLabel *blockBg;
    QStackedWidget *_menu2;
    PWBMenu *_pwbmenu;
    SettMenu *_settmenu;
    PWPMenu *_pwpmenu;
    StatMenu *_statmenu;
    Menu *_menu;

    qint16 blockSize;
    QLabel *lblWaitConnect;
    QTcpSocket *psocket;   //интерфейс обмена
    QTcpServer server;

    bool toCountShip();
    void rankE();
    bool setShip(QList<Cell*> &tempShip, qint8 &side, qint8 &position, qint8 countDeck, QList<qint8> &range);
    void endGame();
    bool eshot(); //true- стреляем еще, false- больше выстрелов не требуется
    void removeArea(QList<qint8> &range, QList<Cell*> &ship, bool horizontal);
    void drawFightSide();

    qint8 shot(QString side, qint8 num); //атакующая сторона //return: 0-промах, 1-попадание, 2-убит, 3-победа

    Chat *_chat;

protected:
    void resizeEvent(QResizeEvent *pe);
    void paintEvent(QPaintEvent *pe);

public:

    QString secondColor;

    enum GameState{Creating, PPlaying, BPlaying};
    enum FightSide{User=0, Enemy=1, Stop};
    GameState _gamestate;
    FightSide _fightside;

    MainWgt(QWidget* parent = 0);


protected slots:
    void sOpenMenu();
    void sSaveSettings();
    void sToggleMenu(QString);
    void sClickedCell(qint8 num, bool friendly);
    void sStartGame();

    void sConnected();
    void sNewConnection();

    void readSocket();
    void sendShot(qint8 numDeck);

    void sDisconnected(); //для сокета сервера. Дисконнект для клиента прописан в лямбде при нажатии кнопки connect
};

