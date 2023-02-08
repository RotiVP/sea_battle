#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPainter>
#include <QMouseEvent>

class Chat
        :public QLabel
{
    Q_OBJECT

    QSize chatSize;
    QPoint pressPos;
    bool moveMod;
    QLabel *phideBtn;

public:
    Chat(QWidget *parent);
    QTextEdit *pted;
    QLineEdit *pled;
    QLabel *pbtn;

private:
    QLabel *header;

private slots:
    void sChatRelease();

protected:
    void mousePressEvent(QMouseEvent *pe);
    void mouseMoveEvent(QMouseEvent *pe);
    void mouseReleaseEvent(QMouseEvent *pe);

signals:
    void toSend();
};

#endif // CHAT_H
