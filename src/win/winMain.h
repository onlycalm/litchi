#ifndef WIN_MAIN_H
#define WIN_MAIN_H

#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QString>
#include "tcp.h"

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

private:
    Ui::winMain *ui;
    cTcp Tcp;
};
#endif // WIN_MAIN_H
