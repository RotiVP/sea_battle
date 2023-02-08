#ifndef MYEVENTFILTER_H
#define MYEVENTFILTER_H

#include <QObject>
#include <QEvent>

class MyEventFilter
        :public QObject
{

    Q_OBJECT

public:
    MyEventFilter(QObject *parent = 0);

    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void dbClick();

};

#endif // MYEVENTFILTER_H

