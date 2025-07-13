#ifndef WIN_MAIN_H
#define WIN_MAIN_H

#include "tcp.h"
#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QString>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class winMain;
}

QT_END_NAMESPACE

class winMain: public QMainWindow
{
Q_OBJECT

public:
    winMain(QWidget *parent = nullptr);
    ~winMain(void);
    void vidInitUi(void);
    void vidInitConn(void);

private:
    Ui::winMain *ui;
    clTcpSer cTcpSer;
    clTcpClt cTcpClt;

    void vidConnBtnClk(void);
    void vidSndBtnClk(void);
};
#endif // WIN_MAIN_H
