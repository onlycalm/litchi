#ifndef RECVWORKER_H
#define RECVWORKER_H

#include <QObject>
#include <QByteArray>
#include <atomic>

class clTcpClt;
class clTcpSer;

/// TCP operation mode.
enum class TcpMode : uint8_t { Client = 0, Server = 1 };

/// Worker object that runs the TCP receive loop on a dedicated QThread.
///
/// Supports both client and server modes.
///
/// Usage (standard Qt worker-object pattern):
///   QThread *thread = new QThread;
///   RecvWorker *worker = new RecvWorker(client, server, mode);
///   worker->moveToThread(thread);
///   connect(thread, &QThread::started, worker, &RecvWorker::process);
///   connect(worker, &RecvWorker::finished, thread, &QThread::quit);
///   connect(thread, &QThread::finished, worker, &QObject::deleteLater);
///   connect(thread, &QThread::finished, thread, &QObject::deleteLater);
///   thread->start();
///
///   // To stop: call requestStop() directly (thread-safe), then thread->quit() + wait().
class RecvWorker : public QObject
{
    Q_OBJECT

public:
    explicit RecvWorker(clTcpClt *client, clTcpSer *server,
                        TcpMode mode, QObject *parent = nullptr);
    ~RecvWorker() override;

    /// Thread-safe stop request. Call from any thread to ask process() to exit.
    void requestStop();

public slots:
    /// Entry point — runs the listen/accept (server) then receive loop.
    /// Connect to QThread::started.
    void process();

    /// Convenience slot: same as requestStop(). Safe for direct or queued calls.
    void stop();

signals:
    /// Emitted each time data is received from the socket.
    void received(const QByteArray &data);

    /// Emitted when the peer disconnects (connection lost unexpectedly).
    void disconnected();

    /// Emitted when the receive loop exits (process() returns).
    void finished();

private:
    TcpMode          m_mode{TcpMode::Client};
    clTcpClt        *m_client{nullptr};
    clTcpSer        *m_server{nullptr};
    std::atomic_bool m_running{false};
};

#endif // RECVWORKER_H
