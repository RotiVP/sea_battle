#ifndef STATMENU_H
#define STATMENU_H

#include <QWidget>
#include <QLabel>

class StatMenu
        :public QWidget
{
public:
    StatMenu(QWidget *parent = 0);

    QLabel *header;
    QLabel *table;

protected:
    void resizeEvent(QResizeEvent *pe);
};


#endif // STATMENU_H
