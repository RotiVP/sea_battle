#ifndef PWBMENU_H
#define PWBMENU_H

#include <QWidget>
#include <QLabel>

#include "buttonmenu.h"

class PWBMenu
        :public QWidget
{
    QLabel *header;

public:
    PWBMenu(QWidget *parent = nullptr);
    QLabel *need;
    ButtonMenu *play;
    ButtonMenu *lose;

protected:
    void resizeEvent(QResizeEvent *pe);
};

#endif // PWBMENU_H
