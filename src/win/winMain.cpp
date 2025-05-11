#include <iostream>
#include <qpushbutton.h>
#include <string>
#include <thread>
#include "err.h"
#include "log.h"
#include "tcp.h"
#include "doip.h"
#include "winMain.h"
#include "ui_winMain.h"

#ifdef WIN_MAIN_H

winMain::winMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::winMain)
{
    LogTr("Enter winMain::winMain function.");

    ui->setupUi(this);
    vidInitUi();
    vidInitConn();

    LogTr("Exit winMain::winMain function.");
}

winMain::~winMain(void)
{
    LogTr("Enter winMain::~winMain function.");

    delete ui;

    LogTr("Exit winMain::~winMain function.");
}

void winMain::vidInitUi(void)
{
    LogTr("Enter winMain::vidInitUi function.");

    ui->pshBtnConn->setStyleSheet("background-color: green;");

    LogTr("Exit winMain::vidInitUi function.");
}

void winMain::vidInitConn(void)
{
    LogTr("Enter winMain::vidInitConn function.");

    connect(ui->pshBtnConn, &QPushButton::clicked, this, &winMain::vidConnBtnClk);
    connect(ui->pshBtnSnd, &QPushButton::clicked, this, &winMain::vidSndBtnClk);
    connect(ui->pshBtnEcuTst, &QPushButton::clicked, this, &winMain::vidEcuTstBtnClk);

    LogTr("Exit winMain::vidInitConn function.");
}

void winMain::vidConnBtnClk(void)
{
    LogTr("Enter winMain::vidConnBtnClk function");

    std::string strConn = ui->pshBtnConn->text().toStdString();

    if(strConn == "Connect")
    {
        LogTr("Requst connect DOIP.");

        if(cDoipClt.erConn() == EC_OK)
        {
            LogScs("Successfully connected to TCP.");

            if(cDoipClt.erReqRtgAct() == EC_OK)
            {
                LogScs("Successfully connected to DOIP.");

                ui->pshBtnConn->setText("Disconnect");
                ui->pshBtnConn->setStyleSheet("background-color: ;"); // Cancle background color.
            }
            else
            {
                LogErr("Failed to connect to TCP.");
            }
        }
        else
        {
            LogErr("Failed to connect to DOIP.");

            ui->pshBtnConn->setStyleSheet("background-color: red;");
        }
    }
    else if(strConn == "Disconnect")
    {
        LogTr("Requst disconnect DOIP.");

        cDoipClt.erDisc();

        ui->pshBtnConn->setText("Connect");
        ui->pshBtnConn->setStyleSheet("background-color: green;");
    }
    else
    {
        LogErr("Button string display error.");
    }

    LogTr("Exit winMain::vidConnBtnClk function");
}

void winMain::vidSndBtnClk(void)
{
    LogTr("Enter winMain::vidSndBtnClk function.");

    if(cDoipClt.erReqDiag() == EC_OK)
    {
        LogScs("DOIP successfully sent.");
    }
    else
    {
        LogErr("DOIP has failed to send.");
    }

    LogTr("Exit winMain::vidSndBtnClk function.");
}

void winMain::vidEcuTstBtnClk(void)
{
    LogTr("Enter winMain::vidEcuTstBtnClk function.");

    LogTr("Exit winMain::vidEcuTstBtnClk function.");
}

#endif // WIN_MAIN_H
