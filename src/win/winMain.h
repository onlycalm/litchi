#ifndef WIN_MAIN_H
#define WIN_MAIN_H

#include "tcp.h"
#include "RecvWorker.h"
#include <QMainWindow>
#include <QString>

class QThread;
class QButtonGroup;
class ProtocolStruct;

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
    ~winMain(void) override;

    void vidInitUi(void);
    void vidInitConn(void);

private:
    Ui::winMain *ui;
    clTcpSer cTcpSer;
    clTcpClt cTcpClt;

    // TCP mode
    TcpMode       m_tcpMode{TcpMode::Client};
    QButtonGroup *m_modeGroup{nullptr};

    // Worker-object pattern: dedicated thread + worker for async TCP receive.
    QThread    *m_recvThread{nullptr};
    RecvWorker *m_recvWorker{nullptr};

    // Protocol struct (loaded from Excel-exported CSV)
    ProtocolStruct *m_protoStruct{nullptr};

    void vidConnBtnClk(void);
    void vidSndBtnClk(void);
    void vidSndStructBtnClk(void);

    void startRecvThread();
    void stopRecvThread();

    // Helpers that dispatch to the active TCP object.
    err  erTcpSnd(u8 *buf, u32 sz);
    bool bTcpIsConn();

    void vidUpdateUiForMode();

private slots:
    void onReceived(const QByteArray &data);
    void vidModeChanged(int id, bool checked);
    void vidPeerDisconnected();
};

#endif // WIN_MAIN_H
