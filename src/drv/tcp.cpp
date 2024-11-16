#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tcp.h"
#include "log.h"
#include "typ.h"
#include "err.h"

#ifdef TCP_H

cTcp::cTcp(void)
{
    LogTr("Enter cTcp::cTcp function.");

    u8AdrFm = AF_INET; // IPV4.
    // u16SvrPt = 12345U;
    // u32SvrIpv4Adr = 0x6601A8C0U; [> 192.168.1.102 <]
    // u32CltIpv4Adr = 0xC801A8C0U; [> 192.168.1.200 <]
    u16SvrPt = 12345U;
    u32SvrIpv4Adr = 0x0100007FU; /* 127.0.0.1 */
    u32CltIpv4Adr = 0x0100007FU; /* 127.0.0.1 */
    s16SvrSktId = -1;
    s16CltSktId = -1;

    LogInf("u8AdrFm: %d", u8AdrFm);
    LogInf("u16SvrPt: %d", u16SvrPt);
    LogInf("u32SvrIpv4Adr: 0x%08X", u32SvrIpv4Adr);
    LogInf("u32CltIpv4Adr: 0x%08X", u32CltIpv4Adr);
    LogInf("s16SvrSktId: %d", s16SvrSktId);
    LogInf("s16CltSktId: %d", s16CltSktId);

    LogTr("Exit cTcp::cTcp function.");
}

cTcp::cTcp(u32 u32SvrIpv4Adr, u32 u32CltIpv4Adr, u16 u16SvrPt)
{
    LogTr("Enter cTcp::cTcp function.");

    u8AdrFm = AF_INET; // IPV4.
    this->u16SvrPt = u16SvrPt;
    this->u32SvrIpv4Adr = u32SvrIpv4Adr;
    this->u32CltIpv4Adr = u32CltIpv4Adr;
    s16SvrSktId = -1;
    s16CltSktId = -1;

    LogInf("u8AdrFm: %d", u8AdrFm);
    LogInf("u16SvrPt: %d", u16SvrPt);
    LogInf("u32SvrIpv4Adr: 0x%08X", u32SvrIpv4Adr);
    LogInf("u32CltIpv4Adr: 0x%08X", u32CltIpv4Adr);
    LogInf("s16SvrSktId: %d", s16SvrSktId);
    LogInf("s16CltSktId: %d", s16CltSktId);

    LogTr("Exit cTcp::cTcp function.");
}

cTcp::~cTcp(void)
{
    LogTr("Enter cTcp::~cTcp function.");

    LogTr("Exit cTcp::~cTcp function.");
}

Err cTcp::SetBlkMd(enTcpBlkMd eTcpBlkMd)
{
    LogTr("Enter cTcp::SetBlkMd function.");

    Err erRtn = EC_NOK;

    // if(eTcpBlkMd == enTcpBlkMd::Blk)
    // {
    //     if(set_nonblocking(s16CltSktId) >= 0)
    //     {
    //         erRtn = EC_OK;
    //         LogScs("Successfully set TCP socket to non blocking mode.");
    //     }
    //     else
    //     {
    //         erRtn = EC_NOK;
    //         LogErr("Failed set TCP socket to non blocking mode.");
    //     }
    // }
    // else if(eTcpBlkMd == enTcpBlkMd::NonBlk)
    // {
    //     if(set_blocking(s16CltSktId) >= 0)
    //     {
    //         erRtn = EC_OK;
    //         LogScs("Successfully set TCP socket to blocking mode.");
    //     }
    //     else
    //     {
    //         erRtn = EC_NOK;
    //         LogErr("Failed set TCP socket to blocking mode.");
    //     }
    // }
    // else
    // {
    //     erRtn = EC_NOK;
    //     LogErr("Failed set TCP socket to blocking or non blocking mode.");
    // }

    return erRtn;

    LogTr("Exit cTcp::SetBlkMd function.");
}

void cTcp::Lsn(void)
{
    LogTr("Enter cTcp::Lsn function.");

    // Create tcp socket.
    s16SvrSktId = socket(AF_INET, SOCK_STREAM, 0);

    if(s16SvrSktId != 0)
    {
        LogScs("Successfully created server TCP socket.");
    }
    else
    {
        LogErr("Failed to create server TCP socket.");
    }

    // Config IP address.
    struct sockaddr_in tSvrAdr = {0};

    tSvrAdr.sin_family = u8AdrFm;
    tSvrAdr.sin_addr.s_addr = u32SvrIpv4Adr;
    tSvrAdr.sin_port = u16SvrPt;

    LogInf("tSvrAdr.sin_family: %d", tSvrAdr.sin_family);
    LogInf("tSvrAdr.sin_addr.s_addr: 0x%08X", tSvrAdr.sin_addr.s_addr);
    LogInf("tSvrAdr.sin_port: %d", tSvrAdr.sin_port);

    // Bind tcp socket.
    if(bind(s16SvrSktId, (struct sockaddr*)&tSvrAdr, sizeof(tSvrAdr)) >= 0)
    {
        LogScs("Successfully bind server TCP socket.");
    }
    else
    {
        close(s16SvrSktId);
        LogErr("Failed to bind server TCP socket.");
    }

    // Listen tcp socket.
    if(listen(s16SvrSktId, 1) >= 0)
    {
        LogScs("Successfully listen server TCP socket.");
    }
    else
    {
        close(s16SvrSktId);
        LogErr("Failed to listen server TCP socket.");
    }

    // Accept connect.
    struct sockaddr_in tCltAdr = {0};
    socklen_t u32SktLen = 0U;

    s16CltSktId = accept(s16SvrSktId, (struct sockaddr*)&tCltAdr, &u32SktLen);

    if(s16CltSktId >= 0)
    {
        LogScs("Successfully received socket connection.");
    }
    else
    {
        close(s16SvrSktId);
        LogErr("Failed to receive socket connection.");
    }

    LogTr("Exit cTcp::Lsn function.");
}

int cTcp::GetRecvSz(void)
{
    // int RecvSz = 0;
    //
    // RecvSz = recv(s16CltSktId, byBuf, RECV_BUF_SIZE, 0);
    //
    // if (bytes_received > 0) {
    //     std::cout << "Received " << bytes_received << " bytes of data: " << buffer << std::endl;
    // } else if (bytes_received == 0) {
    //     std::cout << "Connection closed by client" << std::endl;
    //     break;
    // } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
    //     perror("recv failed");
    //     break;
    // } else {
    //     std::cout << "No data available, retrying..." << std::endl;
    //     usleep(100000); // 等待100毫秒
    // }
    //     memset(buffer, 0, RECV_BUF_SIZE);
    // }
}

int cTcp::Read(byte* pbyMsg)
{
    // int valread = read(new_socket, buffer, 1024);
    //
    // if (valread > 0) {
    //     std::cout << "Received from client: " << buffer << std::endl;
    // } else if (valread == 0) {
    //     std::cout << "Client disconnected" << std::endl;
    //     break;
    // } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
    //     std::cout << "No data available" << std::endl;
    //     usleep(500000); // 等待 500 毫秒
    // } else {
    //     perror("read error");
    //     break;
    // }
}

void cTcp::Send(byte* pbyMsg, u32 u32Size)
{
    // send(s16CltSktId, pbyMsg, u32Size, 0);
}

#endif // TCP_H
