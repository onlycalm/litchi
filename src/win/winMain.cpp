#include <iostream>
#include <string>
#include "winMain.h"
#include "ui_winMain.h"
#include "log.h"

#ifdef WIN_MAIN_H

winMain::winMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::winMain)
{
    LogTr("Enter winMain function.");

    ui->setupUi(this);
    vidInitUi();

    LogTr("Exit winMain function.");
}

winMain::~winMain()
{
    LogTr("Enter ~winMain function.");

    delete ui;

    LogTr("Exit ~winMain function.");
}

void winMain::vidInitUi(void)
{
    LogTr("Enter vidInitUi function.");

    Tcp.Lsn();

    LogTr("Exit vidInitUi function.");
}

#endif // WIN_MAIN_H
