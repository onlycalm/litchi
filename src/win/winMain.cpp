#include <iostream>
#include <qpushbutton.h>
#include <string>
#include <thread>
#include "err.h"
#include "log.h"
#include "tcp.h"
#include "winMain.h"
#include "ui_winMain.h"

#ifdef WIN_MAIN_H

using namespace std;

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

    connect(ui->pshBtnConn,
            &QPushButton::clicked,
            this,
            &winMain::vidConnBtnClk);
    connect(ui->pshBtnSnd, &QPushButton::clicked, this, &winMain::vidSndBtnClk);
    connect(ui->pshBtnEcuTst,
            &QPushButton::clicked,
            this,
            &winMain::vidEcuTstBtnClk);

    LogTr("Exit winMain::vidInitConn function.");
}

void winMain::vidConnBtnClk(void)
{
    LogTr("Enter winMain::vidConnBtnClk function");

    string strConn = ui->pshBtnConn->text().toStdString();

    if(strConn == "Connect")
    {
        LogTr("Requst connect tcp.");

        string strSrcIp = ui->lnEdSrcIp->text().toStdString();
        string strTgtIp = ui->lnEdTgtIp->text().toStdString();
        string strSrcPt = ui->lnEdSrcPt->text().toStdString();
        string strTgtPt = ui->lnEdTgtPt->text().toStdString();
        u32 u32SrcIpAdr = 0u;
        u32 u32TgtIpAdr = 0u;
        u16 u16SrcPt = static_cast<u16>(stoi(strSrcPt));
        u16 u16TgtPt = static_cast<u16>(stoi(strTgtPt));

        LogInf("strSrcIp = %s", strSrcIp.c_str());
        LogInf("strTgtIp = %s", strTgtIp.c_str());
        LogInf("strSrcPt = %s", strSrcPt.c_str());
        LogInf("strTgtPt = %s", strTgtPt.c_str());
        LogInf("u16SrcPt = %d", u16SrcPt);
        LogInf("u16TgtPt = %d", u16TgtPt);

        if((erIpToU32(strSrcIp.c_str(), EndnLe, &u32SrcIpAdr) == EC_OK) &&
           (erIpToU32(strTgtIp.c_str(), EndnLe, &u32TgtIpAdr) == EC_OK))
        {
            LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
            LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);

            if(cTcpClt.erSetNetParm(u32SrcIpAdr,
                                    u32TgtIpAdr,
                                    u16SrcPt,
                                    u16TgtPt) == EC_OK)
            {
                if(cTcpClt.erConn() == EC_OK)
                {
                    LogScs("Successfully connected to TCP.");

                    ui->pshBtnConn->setText("Disconnect");
                    ui->pshBtnConn->setStyleSheet(
                        "background-color: ;"); // Cancle background color.
                }
                else
                {
                    LogErr("Failed to connect to TCP.");

                    ui->pshBtnConn->setStyleSheet("background-color: red;");
                }
            }
            else
            {
                LogErr("Failed to configure network parameters.");
            }
        }
        else
        {
            LogErr("Failed to extract IP address.");
        }
    }
    else if(strConn == "Disconnect")
    {
        LogTr("Requst disconnect tcp.");

        // cDoipClt.erDisc();

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

    // if(cDoipClt.erReqDiag() == EC_OK)
    // {
    //     LogScs("DOIP successfully sent.");
    // }
    // else
    // {
    //     LogErr("DOIP has failed to send.");
    // }

    LogTr("Exit winMain::vidSndBtnClk function.");
}

void winMain::vidEcuTstBtnClk(void)
{
    LogTr("Enter winMain::vidEcuTstBtnClk function.");

    LogTr("Exit winMain::vidEcuTstBtnClk function.");
}

#endif // WIN_MAIN_H
