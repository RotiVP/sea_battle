#include "myeventfilter.h"

MyEventFilter::MyEventFilter(QObject *parent)
    :QObject(parent)
{

}

bool MyEventFilter::eventFilter(QObject *watched, QEvent *event) {

    if(event->type() == QEvent::MouseButtonDblClick) {
        emit dbClick();
        return true;
    }

    return false;
}

