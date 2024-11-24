#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iomanip>
#include "tcp.h"
#include "log.h"
#include "typ.h"
#include "err.h"

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
    u16LclPt = htons(12345);
    u32LclIpv4Adr = 0x0100007Fu; /* 127.0.0.1 */
    u32RmtIpv4Adr = 0x0100007Fu; /* 127.0.0.1 */
    s16LclSktId = -1;
    s16RmtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16LclPt = %04X", u16LclPt);
    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("s16LclSktId = %d", s16LclSktId);
    LogInf("s16RmtSktId = %d", s16RmtSktId);
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    LogTr("Exit clTcpSer::clTcpSer function.");
}

clTcpSer::clTcpSer(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt)
{
    LogTr("Enter clTcpSer::clTcpSer function.");

    u8AdrFm = AF_INET; // IPV4.
    this->u16LclPt = u16LclPt;
    this->u32LclIpv4Adr = u32LclIpv4Adr;
    this->u32RmtIpv4Adr = u32RmtIpv4Adr;
    s16LclSktId = -1;
    s16RmtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16LclPt = %04X", u16LclPt);
    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("s16LclSktId = %d", s16LclSktId);
    LogInf("s16RmtSktId = %d", s16RmtSktId);
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

err clTcpSer::erSetNetParm(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt)
{
    LogTr("Enter clTcpSer::erSetNetParm function.");

    err erRtn = EC_NOK;

    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("u16LclPt = 0x%08X", u16LclPt);

    this->u16LclPt = u16LclPt;
    this->u32LclIpv4Adr = u32LclIpv4Adr;
    this->u32RmtIpv4Adr = u32RmtIpv4Adr;
    erRtn = EC_OK;

    LogInf("this->u16LclPt = 0x%08X", this->u16LclPt);
    LogInf("this->u32RmtIpv4Adr = 0x%08X", this->u32RmtIpv4Adr);
    LogInf("this->u32RmtIpv4Adr = 0x%08X", this->u32RmtIpv4Adr);

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
    s16LclSktId = socket(AF_INET, SOCK_STREAM, 0);
    LogInf("s16LclSktId = %d", s16LclSktId);

    if(s16LclSktId > 0)
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
        struct sockaddr_in tLclAdr = {0};

        tLclAdr.sin_family = u8AdrFm;
        tLclAdr.sin_addr.s_addr = u32LclIpv4Adr;
        tLclAdr.sin_port = u16LclPt;

        LogInf("tLclAdr.sin_family = %d", tLclAdr.sin_family);
        LogInf("tLclAdr.sin_addr.s_addr = 0x%08X", tLclAdr.sin_addr.s_addr);
        LogInf("tLclAdr.sin_port = %d", tLclAdr.sin_port);

        // Bind tcp socket.
        if(bind(s16LclSktId, (struct sockaddr*)&tLclAdr, sizeof(tLclAdr)) >= 0)
        {
            LogScs("Successfully bind server TCP socket.");
        }
        else
        {
            close(s16LclSktId);
            erRtn = EC_NOK;
            LogErr("Failed to bind server TCP socket.");
        }
    }

    if(erRtn == EC_OK)
    {
        // Listen tcp socket.
        if(listen(s16LclSktId, 1) >= 0)
        {
            LogScs("Successfully listen server TCP socket.");
        }
        else
        {
            close(s16LclSktId);
            erRtn = EC_NOK;
            LogErr("Failed to listen server TCP socket.");
        }
    }

    if(erRtn == EC_OK)
    {
        // Accept connect.
        struct sockaddr_in tRmtAdr = {0};
        socklen_t u32SktLen = sizeof(tRmtAdr);

        s16RmtSktId = accept(s16LclSktId, (struct sockaddr*)&tRmtAdr, &u32SktLen);

        if(s16RmtSktId >= 0)
        {
            LogScs("Successfully received socket connection.");
        }
        else
        {
            close(s16LclSktId);
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

    shutdown(s16LclSktId, SHUT_RDWR);
    close(s16LclSktId);
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

        send(s16RmtSktId, pu8Buf, u32Sz, 0);
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
        ssize_t s64RecvRst = recv(s16RmtSktId, pu8Buf, RECV_BUF_SIZE, static_cast<int>(eTcpBlkMd));

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

    ssize_t s64RecvRst = recv(s16RmtSktId, au8Buf, 1, static_cast<int>(enTcpBlkMd::Pv));

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
    u16LclPt = htons(12345);
    u32LclIpv4Adr = 0x0100007Fu; /* 127.0.0.1 */
    u32RmtIpv4Adr = 0x0100007Fu; /* 127.0.0.1 */
    s16LclSktId = -1;
    s16RmtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16LclPt = %04X", u16LclPt);
    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("s16LclSktId = %d", s16LclSktId);
    LogInf("s16RmtSktId = %d", s16RmtSktId);
    LogInf("eTcpBlkMd = %d", (int)eTcpBlkMd);

    LogTr("Exit clTcpClt::clTcpClt function.");
}

clTcpClt::clTcpClt(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt)
{
    LogTr("Enter clTcpSer::clTcpSer function.");

    u8AdrFm = AF_INET; // IPV4.
    this->u16LclPt = u16LclPt;
    this->u32LclIpv4Adr = u32LclIpv4Adr;
    this->u32RmtIpv4Adr = u32RmtIpv4Adr;
    s16LclSktId = -1;
    s16RmtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16LclPt = %04X", u16LclPt);
    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("s16LclSktId = %d", s16LclSktId);
    LogInf("s16RmtSktId = %d", s16RmtSktId);
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

err clTcpClt::erSetNetParm(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt)
{
    LogTr("Enter clTcpClt::erSetNetParm function.");

    err erRtn = EC_NOK;

    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("u16LclPt = 0x%08X", u16LclPt);

    this->u16LclPt = u16LclPt;
    this->u32LclIpv4Adr = u32LclIpv4Adr;
    this->u32RmtIpv4Adr = u32RmtIpv4Adr;
    erRtn = EC_OK;

    LogInf("this->u16LclPt = 0x%08X", this->u16LclPt);
    LogInf("this->u32RmtIpv4Adr = 0x%08X", this->u32RmtIpv4Adr);
    LogInf("this->u32RmtIpv4Adr = 0x%08X", this->u32RmtIpv4Adr);

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
    s16LclSktId = socket(AF_INET, SOCK_STREAM, 0);
    LogInf("s16LclSktId = %d", s16LclSktId);

    if(s16LclSktId > 0)
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
        struct sockaddr_in tLclAdr = {0};

        tLclAdr.sin_family = u8AdrFm;
        tLclAdr.sin_addr.s_addr = u32LclIpv4Adr;
        tLclAdr.sin_port = u16LclPt;

        if(connect(s16LclSktId, (struct sockaddr*)&tLclAdr, sizeof(tLclAdr)) >= 0)
        {
            LogScs("Connected successfully.");
        }
        else
        {
            close(s16LclSktId);
            erRtn = EC_NOK;
            LogErr("Connection Failed.");
        }
    }

    LogTr("Exit clTcpClt::erConn function.");

    return erRtn;
}

err clTcpClt::erDisc(void)
{
    LogTr("Enter clTcpClt::erDisc function.");

    err erRtn = EC_NOK;

    shutdown(s16LclSktId, SHUT_RDWR);
    close(s16LclSktId);
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

        send(s16LclSktId, pu8Buf, u32Sz, 0);
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
        ssize_t s64RecvRst = recv(s16LclSktId, pu8Buf, RECV_BUF_SIZE, static_cast<int>(eTcpBlkMd));

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

    ssize_t s64RecvRst = recv(s16LclSktId, au8Buf, 1, static_cast<int>(enTcpBlkMd::Pv) | static_cast<int>(enTcpBlkMd::NonBlk));
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
