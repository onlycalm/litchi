#ifndef WIN_MAIN_H
#define WIN_MAIN_H

#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QString>
#include "doip.h"

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
    clDoipClt cDoipClt;
    clDoipSer cDoipSer;

    void vidConnBtnClk(void);
    void vidSndBtnClk(void);
    void vidEcuTstBtnClk(void);
};
#endif // WIN_MAIN_H
