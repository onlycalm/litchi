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

    connect(ui->pshBtnConn, &QPushButton::clicked, this, &winMain::onConnBtnClk);

    LogTr("Exit vidInitUi function.");
}

void winMain::onConnBtnClk(void)
{
    LogTr("Enter winMain::onConnBtnClk function");

    std::string strConn = ui->pshBtnConn->text().toStdString();

    if(strConn == "Connect")
    {
        ui->pshBtnConn->setText("Disconnect");
    }
    else if(strConn == "Disconnect")
    {
        ui->pshBtnConn->setText("Connect");
    }
    else
    {
        LogErr("Button string display error.");
    }

    LogTr("Exit winMain::onConnBtnClk function");
}

#endif // WIN_MAIN_H
