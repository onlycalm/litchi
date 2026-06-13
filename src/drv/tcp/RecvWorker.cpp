#include "RecvWorker.h"
#include "tcp.h"
#include "err.h"
#include "log.h"

#include <thread>
#include <chrono>
#include <vector>

RecvWorker::RecvWorker(clTcpClt *client, clTcpSer *server,
                       TcpMode mode, QObject *parent)
    : QObject(parent), m_mode(mode), m_client(client), m_server(server)
{
}

RecvWorker::~RecvWorker()
{
    requestStop();
}

void RecvWorker::requestStop()
{
    m_running.store(false, std::memory_order_release);
}

void RecvWorker::stop()
{
    requestStop();
}

void RecvWorker::process()
{
    m_running.store(true, std::memory_order_release);

    LogTr("RecvWorker::process started (mode=%s)",
          m_mode == TcpMode::Client ? "Client" : "Server");

    // ── Null-pointer guards ──────────────────────────────────────────────
    if (m_mode == TcpMode::Client && !m_client)
    {
        LogErr("RecvWorker: no client object provided.");
        m_running.store(false, std::memory_order_release);
        emit finished();
        return;
    }
    if (m_mode == TcpMode::Server && !m_server)
    {
        LogErr("RecvWorker: no server object provided.");
        m_running.store(false, std::memory_order_release);
        emit finished();
        return;
    }

    constexpr size_t bufSize = 2048;
    std::vector<uint8_t> buf(bufSize);

    // ── Server mode: one-time setup, then accept-loop ─────────────────────
    if (m_mode == TcpMode::Server)
    {
        err er = m_server->erLsnSetup();
        if (er != EC_OK)
        {
            LogErr("Server listen setup failed.");
            m_running.store(false, std::memory_order_release);
            emit finished();
            return;
        }

        // Outer loop: accept clients one at a time.
        while (m_running.load(std::memory_order_acquire))
        {
            // Block until a client connects (or socket is closed).
            er = m_server->erAccept();
            if (er != EC_OK)
                break;  // listening socket closed — shutting down

            // Inner loop: receive from the current client.
            while (m_running.load(std::memory_order_acquire))
            {
                u32 recvSz = 0u;
                er = m_server->erRecv(buf.data(), &recvSz);

                if (er == EC_OK && recvSz > 0u)
                {
                    QByteArray ba(reinterpret_cast<const char *>(buf.data()),
                                  static_cast<int>(recvSz));
                    emit received(ba);
                }
                else if (er != EC_OK)
                {
                    // Check whether the peer disconnected.
                    if (!m_server->bIsConn())
                    {
                        LogInf("Client disconnected. Waiting for next client.");
                        m_server->vidCloseTgt();
                        break;  // inner loop → back to accept
                    }

                    // Transient error — sleep briefly and retry.
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                else
                {
                    // No data available — sleep briefly to avoid busy-waiting.
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }
    }
    else  // ── Client mode: single-connection receive loop ─────────────────
    {
        while (m_running.load(std::memory_order_acquire))
        {
            u32 recvSz = 0u;
            err er = m_client->erRecv(buf.data(), &recvSz);

            if (er == EC_OK && recvSz > 0u)
            {
                QByteArray ba(reinterpret_cast<const char *>(buf.data()),
                              static_cast<int>(recvSz));
                emit received(ba);
            }
            else if (er != EC_OK)
            {
                if (!m_client->bIsConn())
                {
                    LogInf("Peer disconnected. Exiting receive loop.");
                    emit disconnected();
                    break;
                }

                // Transient error — sleep briefly and retry.
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            else
            {
                // No data available — sleep briefly to avoid busy-waiting.
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    }

    LogTr("RecvWorker::process exiting");
    emit finished();
}
