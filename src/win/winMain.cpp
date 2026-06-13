#include <iostream>
#include <qpushbutton.h>
#include <QPlainTextEdit>
#include <QThread>
#include <QDateTime>
#include <QRadioButton>
#include <QButtonGroup>
#include <string>
#include <sstream>
#include <iomanip>
#include "ProtocolStruct.h"
#include "RecvWorker.h"
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

    // Load the protocol struct definition (Excel-exported CSV)
    m_protoStruct = new ProtocolStruct;
    if (m_protoStruct->loadCsv("../test/struct_def.csv"))
    {
        LogInf("Protocol struct loaded: %zu fields, %zu bytes total.",
               m_protoStruct->fields().size(),
               m_protoStruct->totalSize());
    }
    else
    {
        LogErr("Failed to load protocol struct definition.");
    }

    LogTr("Exit winMain::winMain function.");
}

winMain::~winMain(void)
{
    LogTr("Enter winMain::~winMain function.");

    // Ensure receive thread is stopped before destroying UI.
    stopRecvThread();

    delete m_protoStruct;
    delete ui;

    LogTr("Exit winMain::~winMain function.");
}

void winMain::vidInitUi(void)
{
    LogTr("Enter winMain::vidInitUi function.");

    // ── Mode button group (logical grouping, not a widget) ───────────────
    m_modeGroup = new QButtonGroup(this);
    m_modeGroup->addButton(ui->rdoClient, static_cast<int>(TcpMode::Client));
    m_modeGroup->addButton(ui->rdoServer, static_cast<int>(TcpMode::Server));

    // ── Connect button style ─────────────────────────────────────────────
    ui->pshBtnConn->setStyleSheet("background-color: green;");

    // ── Initial visibility according to default mode ────────────────────
    vidUpdateUiForMode();

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
    connect(ui->pshBtnSndStruct,
            &QPushButton::clicked,
            this,
            &winMain::vidSndStructBtnClk);

    // Mode switch
    connect(m_modeGroup, &QButtonGroup::idToggled,
            this, &winMain::vidModeChanged);

    LogTr("Exit winMain::vidInitConn function.");
}

void winMain::vidModeChanged(int id, bool checked)
{
    if (!checked)
        return;  // only act on the button that became checked

    TcpMode newMode = static_cast<TcpMode>(id);
    if (newMode == m_tcpMode)
        return;

    m_tcpMode = newMode;
    vidUpdateUiForMode();
}

void winMain::vidUpdateUiForMode()
{
    bool isClient = (m_tcpMode == TcpMode::Client);

    // Target port: only meaningful for client mode.
    ui->lblTgtPt->setVisible(isClient);
    ui->lnEdTgtPt->setVisible(isClient);

    // Button text.
    if (isClient)
    {
        ui->pshBtnConn->setText("Connect");
    }
    else
    {
        ui->pshBtnConn->setText("Listen");
    }
    ui->pshBtnConn->setStyleSheet("background-color: green;");
}

void winMain::vidConnBtnClk(void)
{
    LogTr("Enter winMain::vidConnBtnClk function");

    string strBtnText = ui->pshBtnConn->text().toStdString();

    if (m_tcpMode == TcpMode::Client)
    {
        if (strBtnText == "Connect")
        {
            LogTr("Request connect tcp (client mode).");

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

            if ((erIpToU32(strSrcIp.c_str(), EndnLe, &u32SrcIpAdr) == EC_OK) &&
                (erIpToU32(strTgtIp.c_str(), EndnLe, &u32TgtIpAdr) == EC_OK))
            {
                LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
                LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);

                if (cTcpClt.erSetNetParm(u32SrcIpAdr,
                                         u32TgtIpAdr,
                                         u16SrcPt,
                                         u16TgtPt) == EC_OK)
                {
                    if (cTcpClt.erConn() == EC_OK)
                    {
                        LogScs("Successfully connected to TCP.");

                        // Start the background receive thread.
                        startRecvThread();

                        // Lock mode while connected.
                        ui->rdoClient->setEnabled(false);
                        ui->rdoServer->setEnabled(false);

                        ui->pshBtnConn->setText("Disconnect");
                        ui->pshBtnConn->setStyleSheet(
                            "background-color: ;"); // Cancel background color.
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
        else if (strBtnText == "Disconnect")
        {
            LogTr("Request disconnect tcp (client mode).");

            // Stop the receive thread before closing the socket.
            stopRecvThread();

            cTcpClt.erDisc();

            // Re-enable mode switching.
            ui->rdoClient->setEnabled(true);
            ui->rdoServer->setEnabled(true);

            ui->pshBtnConn->setText("Connect");
            ui->pshBtnConn->setStyleSheet("background-color: green;");
        }
        else
        {
            LogErr("Button string display error.");
        }
    }
    else  // Server mode
    {
        if (strBtnText == "Listen")
        {
            LogTr("Request listen tcp (server mode).");

            string strSrcIp = ui->lnEdSrcIp->text().toStdString();
            string strTgtIp = ui->lnEdTgtIp->text().toStdString(); // informational
            string strSrcPt = ui->lnEdSrcPt->text().toStdString();
            u32 u32SrcIpAdr = 0u;
            u32 u32TgtIpAdr = 0u;
            u16 u16SrcPt = static_cast<u16>(stoi(strSrcPt));

            LogInf("strSrcIp = %s", strSrcIp.c_str());
            LogInf("strSrcPt = %s", strSrcPt.c_str());
            LogInf("u16SrcPt = %d", u16SrcPt);

            if ((erIpToU32(strSrcIp.c_str(), EndnLe, &u32SrcIpAdr) == EC_OK) &&
                (erIpToU32(strTgtIp.c_str(), EndnLe, &u32TgtIpAdr) == EC_OK))
            {
                LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);

                // Server erSetNetParm takes 3 params (no target port).
                if (cTcpSer.erSetNetParm(u32SrcIpAdr, u32TgtIpAdr, u16SrcPt) == EC_OK)
                {
                    // Start the worker thread.  The worker calls erLsnSetup()
                    // then loops on accept() — keeping the UI responsive.
                    startRecvThread();

                    // Lock mode while listening / connected.
                    ui->rdoClient->setEnabled(false);
                    ui->rdoServer->setEnabled(false);

                    ui->pshBtnConn->setText("Disconnect");
                    ui->pshBtnConn->setStyleSheet("background-color: ;");

                    LogInf("Server listening on %s:%d...",
                           strSrcIp.c_str(), u16SrcPt);
                }
                else
                {
                    LogErr("Failed to configure server network parameters.");
                    ui->pshBtnConn->setStyleSheet("background-color: red;");
                }
            }
            else
            {
                LogErr("Failed to extract IP address.");
                ui->pshBtnConn->setStyleSheet("background-color: red;");
            }
        }
        else if (strBtnText == "Disconnect")
        {
            LogTr("Request disconnect tcp (server mode).");

            // Close the listening socket to unblock accept(),
            // then stop the worker thread.
            cTcpSer.erDisc();

            stopRecvThread();

            // Re-enable mode switching.
            ui->rdoClient->setEnabled(true);
            ui->rdoServer->setEnabled(true);

            ui->pshBtnConn->setText("Listen");
            ui->pshBtnConn->setStyleSheet("background-color: green;");
        }
        else
        {
            LogErr("Button string display error.");
        }
    }

    LogTr("Exit winMain::vidConnBtnClk function");
}

void winMain::vidSndBtnClk(void)
{
    LogTr("Enter winMain::vidSndBtnClk function.");

    std::string strSndMsg = ui->plainTextEdit->toPlainText().toStdString();

    if (erTcpSnd((u8*)strSndMsg.c_str(), strSndMsg.length()) == EC_OK)
    {
        LogScs("Tcp successfully sent. Message: %s", strSndMsg.c_str());
    }
    else
    {
        LogErr("Tcp has failed to send.");
    }

    LogTr("Exit winMain::vidSndBtnClk function.");
}

// ------------------------------------------------------------------
// Send Struct  — serialize the protocol struct with test values
// ------------------------------------------------------------------
void winMain::vidSndStructBtnClk(void)
{
    LogTr("Enter winMain::vidSndStructBtnClk function.");

    if (!m_protoStruct || m_protoStruct->isEmpty())
    {
        LogErr("No protocol struct loaded. Cannot send.");
        return;
    }

    if (!bTcpIsConn())
    {
        LogErr("TCP not connected. Please connect first.");
        return;
    }

    // Serialize the struct with auto-generated test values.
    std::vector<uint8_t> buf = m_protoStruct->serializeTest();

    // Hex dump for the log.
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < buf.size(); ++i)
        oss << std::setw(2) << static_cast<int>(buf[i]) << ' ';
    LogInf("Sending struct (%zu bytes): %s", buf.size(), oss.str().c_str());

    if (erTcpSnd(buf.data(), static_cast<u32>(buf.size())) == EC_OK)
    {
        LogScs("Struct sent successfully (%zu bytes).", buf.size());
    }
    else
    {
        LogErr("Failed to send struct.");
    }

    LogTr("Exit winMain::vidSndStructBtnClk function.");
}

// ------------------------------------------------------------------
// Helpers — dispatch to the active TCP object.
// ------------------------------------------------------------------
err winMain::erTcpSnd(u8 *buf, u32 sz)
{
    if (m_tcpMode == TcpMode::Client)
        return cTcpClt.erSnd(buf, sz);
    else
        return cTcpSer.erSnd(buf, sz);
}

bool winMain::bTcpIsConn()
{
    if (m_tcpMode == TcpMode::Client)
        return cTcpClt.bIsConn();
    else
        return cTcpSer.bIsConn();
}

// ------------------------------------------------------------------
// Receive thread management  (standard Qt worker-object pattern)
// ------------------------------------------------------------------
void winMain::startRecvThread()
{
    if (m_recvThread)
        return;  // already running

    // 1. Create thread and worker (no parent — moveToThread transfers affinity).
    m_recvThread = new QThread;
    m_recvWorker = new RecvWorker(&cTcpClt, &cTcpSer, m_tcpMode);

    // 2. Move worker to the new thread.
    m_recvWorker->moveToThread(m_recvThread);

    // 3. Wire up lifecycle signals (all new-style compile-time-checked syntax).
    connect(m_recvThread,  &QThread::started,
            m_recvWorker,  &RecvWorker::process);
    connect(m_recvWorker,  &RecvWorker::finished,
            m_recvThread,  &QThread::quit);
    connect(m_recvWorker,  &RecvWorker::finished,
            m_recvWorker,  &QObject::deleteLater);
    connect(m_recvThread,  &QThread::finished,
            m_recvThread,  &QObject::deleteLater);

    // 4. Application-level signals: forward received data and disconnect
    //    notifications to the UI.
    connect(m_recvWorker,  &RecvWorker::received,
            this,          &winMain::onReceived,
            Qt::QueuedConnection);
    connect(m_recvWorker,  &RecvWorker::disconnected,
            this,          &winMain::vidPeerDisconnected,
            Qt::QueuedConnection);

    // 5. Go.
    m_recvThread->start();
}

void winMain::stopRecvThread()
{
    if (!m_recvThread || !m_recvWorker)
        return;

    // 1. Tell the worker to leave its receive loop (thread-safe atomic store).
    m_recvWorker->requestStop();

    // 2. Ask the thread's event loop to exit and wait for it to finish.
    m_recvThread->quit();
    m_recvThread->wait(3000);

    // 3. Drop our pointers. The thread and worker will be deleted by the
    //    deleteLater connections above once the event loop drains.
    m_recvWorker  = nullptr;
    m_recvThread  = nullptr;
}

void winMain::onReceived(const QByteArray &ba)
{
    // ── Application log ─────────────────────────────────────────────
    QString s = QString::fromUtf8(ba.constData(), ba.size());
    LogInf("Received data (text): %s", s.toStdString().c_str());

    // ── UI receive area ─────────────────────────────────────────────
    // Build a one-line hex dump with timestamp.
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    const auto *bytes = reinterpret_cast<const unsigned char *>(ba.constData());
    for (int i = 0; i < ba.size(); ++i)
        oss << std::setw(2) << static_cast<int>(bytes[i]) << ' ';

    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    QString line = QString("[%1] %2 B  %3\n")
                       .arg(ts)
                       .arg(ba.size())
                       .arg(QString::fromStdString(oss.str()));

    // Append without losing scroll position.
    ui->txtEditRecv->moveCursor(QTextCursor::End);
    ui->txtEditRecv->insertPlainText(line);
    ui->txtEditRecv->moveCursor(QTextCursor::End);
}

void winMain::vidPeerDisconnected()
{
    // Server mode: worker handles re-accept internally; this signal
    // should not fire.  Handle defensively just in case.
    if (m_tcpMode == TcpMode::Server)
    {
        LogWrn("vidPeerDisconnected called in server mode — unexpected.");
        return;
    }

    LogInf("Peer disconnected. Resetting UI state.");

    // Reset connection UI.
    ui->pshBtnConn->setText("Connect");
    ui->pshBtnConn->setStyleSheet("background-color: green;");
    cTcpClt.erDisc();

    // Re-enable mode switching.
    ui->rdoClient->setEnabled(true);
    ui->rdoServer->setEnabled(true);

    // Drop thread/worker pointers. The thread and worker are deleted
    // by deleteLater connections once their event loops drain.
    m_recvWorker  = nullptr;
    m_recvThread  = nullptr;
}

#endif // WIN_MAIN_H
