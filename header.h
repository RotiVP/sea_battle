#ifndef MAINHEADER_H
#define MAINHEADER_H

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QHBoxLayout>


class Header
        :public QLabel
{
     Q_OBJECT

    QLabel *lblOpenMenu;
    QColor secondColor;

public:
    Header(QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);

signals:
    void clicked_bOpenMenu();
};

#endif // MAINHEADER_H
