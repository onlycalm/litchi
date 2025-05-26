#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iomanip>
#include "typ.h"
#include "log.h"
#include "err.h"
#include "tcp.h"

#ifdef TCP_H

std::string HexToStr(const uint8_t* array, size_t length)
{
    std::stringstream ss;
    for(size_t i = 0; i < length; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(array[i]);
    }

    return ss.str();
}

clTcpSer::clTcpSer(void)
{
    LogTr("Enter clTcpSer::clTcpSer function.");

    u8AdrFm = AF_INET; // IPV4.
    u16SrcPt = htons(12345);
    u16TgtPt = htons(12346);
    u32SrcIpAdr = 0x0100007Fu; /* 127.0.0.1 */
    u32TgtIpAdr = 0x0100007Fu; /* 127.0.0.1 */
    s16SrcSktId = -1;
    s16TgtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16SrcPt = %04X", u16SrcPt);
    LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
    LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);
    LogInf("s16SrcSktId = %d", s16SrcSktId);
    LogInf("s16TgtSktId = %d", s16TgtSktId);
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    LogTr("Exit clTcpSer::clTcpSer function.");
}

clTcpSer::clTcpSer(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt)
{
    LogTr("Enter clTcpSer::clTcpSer function.");

    u8AdrFm = AF_INET; // IPV4.
    this->u16SrcPt = u16SrcPt;
    this->u32SrcIpAdr = u32SrcIpAdr;
    this->u32TgtIpAdr = u32TgtIpAdr;
    s16SrcSktId = -1;
    s16TgtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16SrcPt = %04X", u16SrcPt);
    LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
    LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);
    LogInf("s16SrcSktId = %d", s16SrcSktId);
    LogInf("s16TgtSktId = %d", s16TgtSktId);
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    LogTr("Exit clTcpSer::clTcpSer function.");
}

clTcpSer::~clTcpSer(void)
{
    LogTr("Enter clTcpSer::~clTcpSer function.");

    LogTr("Exit clTcpSer::~clTcpSer function.");
}

err clTcpSer::erSetBlkMd(enTcpBlkMd eTcpBlkMd)
{
    LogTr("Enter clTcpSer::erSetBlkMd function.");
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    err erRtn = EC_NOK;

    this->eTcpBlkMd = eTcpBlkMd;
    LogInf("this->eTcpBlkMd = %d", (int)this->eTcpBlkMd);

    return erRtn;

    LogTr("Exit clTcpSer::erSetBlkMd function.");
}

err clTcpSer::erSetNetParm(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt)
{
    LogTr("Enter clTcpSer::erSetNetParm function.");

    err erRtn = EC_NOK;

    LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
    LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);
    LogInf("u16SrcPt = 0x%08X", u16SrcPt);

    this->u16SrcPt = u16SrcPt;
    this->u32SrcIpAdr = u32SrcIpAdr;
    this->u32TgtIpAdr = u32TgtIpAdr;
    erRtn = EC_OK;

    LogInf("this->u16SrcPt = 0x%08X", this->u16SrcPt);
    LogInf("this->u32TgtIpAdr = 0x%08X", this->u32TgtIpAdr);
    LogInf("this->u32TgtIpAdr = 0x%08X", this->u32TgtIpAdr);

    LogTr("Exit clTcpSer::erSetNetParm function.");

    return erRtn;
}

enTcpBlkMd clTcpSer::eGetBlkMd(void)
{
    LogTr("Enter clTcpSer::eGetBlkMd function.");

    return eTcpBlkMd;

    LogTr("Exit clTcpSer::eGetBlkMd function.");
}

err clTcpSer::erLsn(void)
{
    LogTr("Enter clTcpSer::erLsn function.");

    err erRtn = EC_OK;

    // Create tcp socket.
    s16SrcSktId = socket(AF_INET, SOCK_STREAM, 0);
    LogInf("s16SrcSktId = %d", s16SrcSktId);

    if(s16SrcSktId > 0)
    {
        LogScs("Successfully created server TCP socket.");
    }
    else
    {
        erRtn = EC_NOK;
        LogErr("Failed to create server TCP socket.");
    }

    if(erRtn == EC_OK)
    {
        // Config IP address.
        struct sockaddr_in tSrcAdr = {0};

        tSrcAdr.sin_family = u8AdrFm;
        tSrcAdr.sin_addr.s_addr = u32SrcIpAdr;
        tSrcAdr.sin_port = u16SrcPt;

        LogInf("tSrcAdr.sin_family = %d", tSrcAdr.sin_family);
        LogInf("tSrcAdr.sin_addr.s_addr = 0x%08X", tSrcAdr.sin_addr.s_addr);
        LogInf("tSrcAdr.sin_port = %d", tSrcAdr.sin_port);

        // Bind tcp socket.
        if(bind(s16SrcSktId, (struct sockaddr*)&tSrcAdr, sizeof(tSrcAdr)) >= 0)
        {
            LogScs("Successfully bind server TCP socket.");
        }
        else
        {
            close(s16SrcSktId);
            erRtn = EC_NOK;
            LogErr("Failed to bind server TCP socket.");
        }
    }

    if(erRtn == EC_OK)
    {
        // Listen tcp socket.
        if(listen(s16SrcSktId, 1) >= 0)
        {
            LogScs("Successfully listen server TCP socket.");
        }
        else
        {
            close(s16SrcSktId);
            erRtn = EC_NOK;
            LogErr("Failed to listen server TCP socket.");
        }
    }

    if(erRtn == EC_OK)
    {
        // Accept connect.
        struct sockaddr_in tTgtAdr = {0};
        socklen_t u32SktLen = sizeof(tTgtAdr);

        s16TgtSktId = accept(s16SrcSktId, (struct sockaddr*)&tTgtAdr, &u32SktLen);

        if(s16TgtSktId >= 0)
        {
            LogScs("Successfully received socket connection.");
        }
        else
        {
            close(s16SrcSktId);
            erRtn = EC_NOK;
            LogErr("Failed to receive socket connection.");
        }
    }

    LogTr("Exit clTcpSer::erLsn function.");

    return erRtn;
}

err clTcpSer::erDisc(void)
{
    LogTr("Enter clTcpSer::erDisc function.");

    err erRtn = EC_NOK;

    shutdown(s16SrcSktId, SHUT_RDWR);
    close(s16SrcSktId);
    erRtn = EC_OK;

    LogTr("Exit clTcpSer::erDisc function.");

    return erRtn;
}

err clTcpSer::erSnd(u8* pu8Buf, u32 u32Sz)
{
    LogTr("Enter clTcpSer::erSnd function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) &&
       (u32Sz > 0u) &&
       (bIsConn() == true))
    {
        LogInf("Send data: 0x%s", HexToStr(pu8Buf, u32Sz).c_str());

        send(s16TgtSktId, pu8Buf, u32Sz, 0);
        LogScs("TCP successfully sent.");
        erRtn = EC_OK;
    }
    else
    {
        LogErr("TCP failed to send.");

        erRtn = EC_NOK;
    }

    LogTr("Exit clTcpSer::erSnd function.");

    return erRtn;
}

err clTcpSer::erRecv(u8* pu8Buf, u32* pu32Sz)
{
    LogTr("Enter clTcpSer::erRecv function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) && (pu32Sz != NULL))
    {
        ssize_t s64RecvRst = recv(s16TgtSktId, pu8Buf, RECV_BUF_SIZE, static_cast<int>(eTcpBlkMd));

        if(s64RecvRst > 0)
        {
            LogInf("Recv data: 0x%s", HexToStr(pu8Buf, s64RecvRst).c_str());

            *pu32Sz = s64RecvRst;
            erRtn = EC_OK;
        }
        else if(s64RecvRst == 0)
        {
            *pu32Sz = 0u;
            erRtn = EC_NOK;
            LogWrn("Recv disconnected.");
        }
        else if(s64RecvRst == -1)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                // Receive buffer is empty.
                *pu32Sz = 0u;
                erRtn = EC_OK;
            }
            else
            {
                *pu32Sz = 0u;
                erRtn = EC_NOK;
                LogErr("Recv failed 0x%08X.", errno);
            }
        }
        else
        {
            *pu32Sz = 0u;
            erRtn = EC_NOK;
            LogErr("Recv failed.");
        }
    }
    else
    {
        *pu32Sz = 0u;
        erRtn = EC_NOK;
        LogErr("NULL point.");
    }

    LogTr("Exit clTcpSer::erRecv function.");

    return erRtn;
}

bool clTcpSer::bIsConn(void)
{
    LogTr("Enter clTcpSer::bIsConn function.");

    bool bConn = false;
    u8 au8Buf[1] = {0};

    ssize_t s64RecvRst = recv(s16TgtSktId, au8Buf, 1, static_cast<int>(enTcpBlkMd::Pv));

    if(s64RecvRst != 0)
    {
        bConn = true;
    }
    else
    {
        bConn = false;
    }

    LogTr("Exit clTcpSer::bIsConn function.");

    return bConn;
}

clTcpClt::clTcpClt(void)
{
    LogTr("Enter clTcpClt::clTcpClt function.");

    u8AdrFm = AF_INET; // IPV4.
    u16SrcPt = htons(12346);
    u16TgtPt = htons(12345);
    u32SrcIpAdr = 0x0100007Fu; /* 127.0.0.1 */
    u32TgtIpAdr = 0x0100007Fu; /* 127.0.0.1 */
    s16SrcSktId = -1;
    s16TgtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16SrcPt = %04X", u16SrcPt);
    LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
    LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);
    LogInf("s16SrcSktId = %d", s16SrcSktId);
    LogInf("s16TgtSktId = %d", s16TgtSktId);
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    LogTr("Exit clTcpClt::clTcpClt function.");
}

clTcpClt::clTcpClt(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt)
{
    LogTr("Enter clTcpSer::clTcpSer function.");

    u8AdrFm = AF_INET; // IPV4.
    this->u16SrcPt = u16SrcPt;
    this->u32SrcIpAdr = u32SrcIpAdr;
    this->u32TgtIpAdr = u32TgtIpAdr;
    s16SrcSktId = -1;
    s16TgtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16SrcPt = %04X", u16SrcPt);
    LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
    LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);
    LogInf("s16SrcSktId = %d", s16SrcSktId);
    LogInf("s16TgtSktId = %d", s16TgtSktId);
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    LogTr("Exit clTcpSer::clTcpSer function.");
}

clTcpClt::~clTcpClt(void)
{
    LogTr("Enter clTcpClt::~clTcpClt function.");

    LogTr("Exit clTcpClt::~clTcpClt function.");
}

err clTcpClt::erSetBlkMd(enTcpBlkMd eTcpBlkMd)
{
    LogTr("Enter clTcpClt::erSetBlkMd function.");
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    err erRtn = EC_NOK;

    this->eTcpBlkMd = eTcpBlkMd;
    LogInf("this->eTcpBlkMd = %d", (int)this->eTcpBlkMd);

    return erRtn;

    LogTr("Exit clTcpClt::erSetBlkMd function.");
}

err clTcpClt::erSetNetParm(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt, u16 u16TgtPt)
{
    LogTr("Enter clTcpClt::erSetNetParm function.");

    err erRtn = EC_NOK;

    LogInf("u32SrcIpAdr = 0x%08X", u32SrcIpAdr);
    LogInf("u32TgtIpAdr = 0x%08X", u32TgtIpAdr);
    LogInf("u16TgtPt = 0x%04X", u16TgtPt);
    LogInf("u16SrcPt = 0x%04X", u16SrcPt);

    this->u32SrcIpAdr = u32SrcIpAdr;
    this->u32TgtIpAdr = u32TgtIpAdr;
    this->u16TgtPt = u16TgtPt;
    this->u16SrcPt = u16SrcPt;
    erRtn = EC_OK;

    LogInf("this->u16TgtPt = 0x%04X", this->u16TgtPt);
    LogInf("this->u16SrcPt = 0x%04X", this->u16SrcPt);
    LogInf("this->u32TgtIpAdr = 0x%08X", this->u32TgtIpAdr);
    LogInf("this->u32TgtIpAdr = 0x%08X", this->u32TgtIpAdr);

    LogTr("Exit clTcpClt::erSetNetParm function.");

    return erRtn;
}

enTcpBlkMd clTcpClt::eGetBlkMd(void)
{
    LogTr("Enter clTcpClt::eGetBlkMd function.");

    return eTcpBlkMd;

    LogTr("Exit clTcpClt::eGetBlkMd function.");
}

err clTcpClt::erConn(void)
{
    LogTr("Enter clTcpClt::erConn function.");

    err erRtn = EC_OK;

    // Create tcp socket.
    s16SrcSktId = socket(AF_INET, SOCK_STREAM, 0);
    LogInf("s16SrcSktId = %d", s16SrcSktId);

    if(s16SrcSktId > 0)
    {
        LogScs("Successfully created server TCP socket.");
    }
    else
    {
        erRtn = EC_NOK;
        LogErr("Failed to create server TCP socket.");
    }

    if(erRtn == EC_OK)
    {
        int Enable = 1;

        // Allow immediate reconnection.
        if(setsockopt(s16SrcSktId, SOL_SOCKET, SO_REUSEADDR, &Enable, sizeof(int)) >= 0)
        {
            LogScs("SO_REUSEADDR configuration successful.");
        }
        else
        {
            LogScs("SO_REUSEADDR configuration failed.");
        }
    }

    if(erRtn == EC_OK)
    {
        // Bind IP.
        struct sockaddr_in tSrcAdr = {0};

        tSrcAdr.sin_family = u8AdrFm;
        tSrcAdr.sin_addr.s_addr = u32SrcIpAdr;
        tSrcAdr.sin_port = u16SrcPt;

        if(bind(s16SrcSktId, (struct sockaddr*)&tSrcAdr, sizeof(tSrcAdr)) >= 0)
        {
            LogScs("Local IP binding successful.");
        }
        else
        {
            LogErr("Local IP binding failed.");
            LogInf("errno = 0x%08X", errno);

            close(s16SrcSktId);
            erRtn = EC_NOK;
        }
    }

    if(erRtn == EC_OK)
    {
        // Config IP address.
        struct sockaddr_in tTgtAdr = {0};

        tTgtAdr.sin_family = u8AdrFm;
        tTgtAdr.sin_addr.s_addr = u32SrcIpAdr;
        tTgtAdr.sin_port = u16TgtPt;

        if(connect(s16SrcSktId, (struct sockaddr*)&tTgtAdr, sizeof(tTgtAdr)) >= 0)
        {
            LogScs("Connected successfully.");
        }
        else
        {
            LogErr("Connection Failed. Error: %s", strerror(errno));

            close(s16SrcSktId);
            erRtn = EC_NOK;
        }
    }

    LogTr("Exit clTcpClt::erConn function.");

    return erRtn;
}

err clTcpClt::erDisc(void)
{
    LogTr("Enter clTcpClt::erDisc function.");

    err erRtn = EC_NOK;

    shutdown(s16SrcSktId, SHUT_RDWR);
    close(s16SrcSktId);
    erRtn = EC_OK;

    LogTr("Exit clTcpClt::erDisc function.");

    return erRtn;
}

err clTcpClt::erSnd(u8* pu8Buf, u32 u32Sz)
{
    LogTr("Enter clTcpClt::erSnd function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) &&
       (u32Sz > 0u) &&
       (bIsConn() == true))
    {
        LogInf("Send data: 0x%s", HexToStr(pu8Buf, u32Sz).c_str());

        send(s16SrcSktId, pu8Buf, u32Sz, 0);
        LogScs("TCP successfully sent.");
        erRtn = EC_OK;
    }
    else
    {
        LogErr("TCP failed to send.");

        erRtn = EC_NOK;
    }

    LogInf("clTcpClt::erSnd return err 0x%08X.", erRtn);
    LogTr("Exit clTcpClt::erSnd function.");

    return erRtn;
}

err clTcpClt::erRecv(u8* pu8Buf, u32* pu32Sz)
{
    LogTr("Enter clTcpClt::erRecv function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) && (pu32Sz != NULL))
    {
        ssize_t s64RecvRst = recv(s16SrcSktId, pu8Buf, RECV_BUF_SIZE, static_cast<int>(eTcpBlkMd));

        if(s64RecvRst > 0)
        {
            LogInf("Recv data: 0x%s", HexToStr(pu8Buf, s64RecvRst).c_str());

            *pu32Sz = s64RecvRst;
            erRtn = EC_OK;
        }
        else if(s64RecvRst == 0)
        {
            *pu32Sz = 0u;
            erRtn = EC_NOK;
            LogWrn("Recv disconnected.");
        }
        else if(s64RecvRst == -1)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                // Receive buffer is empty.
                *pu32Sz = 0u;
                erRtn = EC_OK;
            }
            else
            {
                *pu32Sz = 0u;
                erRtn = EC_NOK;
                LogErr("Recv failed.");
            }
        }
        else
        {
            *pu32Sz = 0u;
            erRtn = EC_NOK;
            LogErr("Recv failed.");
        }
    }
    else
    {
        *pu32Sz = 0u;
        erRtn = EC_NOK;
        LogErr("NULL point.");
    }

    LogTr("Exit clTcpClt::erRecv function.");

    return erRtn;
}

bool clTcpClt::bIsConn(void)
{
    LogTr("Enter clTcpClt::bIsConn function.");

    bool bConn = false;
    u8 au8Buf[1] = {0};

    ssize_t s64RecvRst = recv(s16SrcSktId, au8Buf, 1, static_cast<int>(enTcpBlkMd::Pv) | static_cast<int>(enTcpBlkMd::NonBlk));
    LogInf("recv return s64 %d", (int)s64RecvRst);

    if(s64RecvRst > 0)
    {
        bConn = true;
    }
    else if(s64RecvRst == 0)
    {
        LogTr("The other party disconnects.");

        bConn = false; // The other party disconnects.
    }
    else
    {
        LogInf("errno = 0x%08X", errno);

        if((errno == ENOTCONN) || (errno == EBADF))
        {
            LogTr("Not connected.");

            bConn = false;
        }
        else
        {
            bConn = true;
        }
    }

    LogInf("clTcpClt::bIsConn return bool 0x%02X", bConn);
    LogTr("Exit clTcpClt::bIsConn function.");

    return bConn;
}

// Tcp server.
// cTcpSer.erLsn();
// cTcpSer.erSetBlkMd(enTcpBlkMd::NonBlk);
// u8 au8Buf[1024] = {0u};
// u32 u32RecvSz = 0;
//
// do
// {
//     cTcpSer.erRecv(au8Buf, &u32RecvSz);
//
//     if(u32RecvSz > 0)
//     {
//         au8Buf[0] = 0x11;
//         au8Buf[1] = 0x22;
//         u32RecvSz = 2;
//         cTcpSer.erSnd(au8Buf, u32RecvSz);
//     }
// } while(cTcpSer.bIsConn() == true);

// Tcp client.
// cTcpClt.erConn();
// cTcpClt.erSetBlkMd(enTcpBlkMd::NonBlk);
// u8 au8Buf[1024] = {0u};
// u32 u32RecvSz = 0;
// au8Buf[0] = 0x55u;
// au8Buf[1] = 0xAAu;
// cTcpClt.erSnd(au8Buf, 2);
//
// do
// {
//     cTcpClt.erRecv(au8Buf, &u32RecvSz);
//
//     if(u32RecvSz > 0)
//     {
//         au8Buf[0] = 0x55u;
//         au8Buf[1] = 0xAAu;
//         cTcpClt.erSnd(au8Buf, 2);
//     }
// } while(cTcpClt.bIsConn() == true);

#endif // TCP_H
