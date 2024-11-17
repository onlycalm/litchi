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

std::string HexArrayToString(const unsigned char* hexArray, size_t length)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase;

    for(size_t i = 0; i < length; ++i)
    {
        ss << static_cast<int>(hexArray[i]);
    }

    return ss.str();
}

cTcpSer::cTcpSer(void)
{
    LogTr("Enter cTcpSer::cTcpSer function.");

    u8AdrFm = AF_INET; // IPV4.
    u16LclPt = htons(12345);
    u32LclIpv4Adr = 0x0100007FU; /* 127.0.0.1 */
    u32RmtIpv4Adr = 0x0100007FU; /* 127.0.0.1 */
    s16LclSktId = -1;
    s16RmtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16LclPt = %04X", u16LclPt);
    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("s16LclSktId = %d", s16LclSktId);
    LogInf("s16RmtSktId = %d", s16RmtSktId);
    LogInf("eTcpBlkMd = %d", eTcpBlkMd);

    LogTr("Exit cTcpSer::cTcpSer function.");
}

cTcpSer::cTcpSer(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt)
{
    LogTr("Enter cTcpSer::cTcpSer function.");

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
    LogInf("eTcpBlkMd = %d", eTcpBlkMd);

    LogTr("Exit cTcpSer::cTcpSer function.");
}

cTcpSer::~cTcpSer(void)
{
    LogTr("Enter cTcpSer::~cTcpSer function.");

    LogTr("Exit cTcpSer::~cTcpSer function.");
}

err cTcpSer::SetBlkMd(enTcpBlkMd eTcpBlkMd)
{
    LogTr("Enter cTcpSer::SetBlkMd function.");

    err erRtn = EC_NOK;

    this->eTcpBlkMd = eTcpBlkMd;
    LogInf("this->eTcpBlkMd = %d", eTcpBlkMd);

    return erRtn;

    LogTr("Exit cTcpSer::SetBlkMd function.");
}

enTcpBlkMd cTcpSer::GetBlkMd(void)
{
    LogTr("Enter cTcpSer::GetBlkMd function.");

    return eTcpBlkMd;

    LogTr("Exit cTcpSer::GetBlkMd function.");
}

err cTcpSer::Lsn(void)
{
    LogTr("Enter cTcpSer::Lsn function.");

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

    LogTr("Exit cTcpSer::Lsn function.");
}

err cTcpSer::Snd(u8* pu8Buf, u32 u32Sz)
{
    LogTr("Enter cTcpSer::Snd function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) && (u32Sz > 0U))
    {
        LogInf("Send data: 0x%s", HexArrayToString(pu8Buf, u32Sz).c_str());

        send(s16RmtSktId, pu8Buf, u32Sz, 0);
        erRtn = EC_OK;
    }
    else
    {
        erRtn = EC_NOK;
    }

    LogTr("Exit cTcpSer::Snd function.");

    return erRtn;
}

err cTcpSer::Recv(u8* pu8Buf, u32* pu32Sz)
{
    LogTr("Enter cTcpSer::Recv function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) && (pu32Sz != NULL))
    {
        ssize_t s64RecvRst = recv(s16RmtSktId, pu8Buf, RECV_BUF_SIZE, static_cast<int>(eTcpBlkMd));

        if(s64RecvRst > 0)
        {
            LogInf("Recv data: 0x%s", HexArrayToString(pu8Buf, s64RecvRst).c_str());

            *pu32Sz = s64RecvRst;
            erRtn = EC_OK;
        }
        else if(s64RecvRst == 0)
        {
            *pu32Sz = 0U;
            erRtn = EC_NOK;
            LogWrn("Recv disconnected.");
        }
        else if(s64RecvRst == -1)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                // Receive buffer is empty.
                *pu32Sz = 0U;
                erRtn = EC_OK;
            }
            else
            {
                *pu32Sz = 0U;
                erRtn = EC_NOK;
                LogErr("Recv failed 0x%08X.", errno);
            }
        }
        else
        {
            *pu32Sz = 0U;
            erRtn = EC_NOK;
            LogErr("Recv failed.");
        }
    }
    else
    {
        *pu32Sz = 0U;
        erRtn = EC_NOK;
        LogErr("NULL point.");
    }

    LogTr("Exit cTcpSer::Recv function.");

    return erRtn;
}

bool cTcpSer::IsConn(void)
{
    LogTr("Enter cTcpSer::IsConn function.");

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

    LogTr("Exit cTcpSer::IsConn function.");

    return bConn;
}

cTcpClt::cTcpClt(void)
{
    LogTr("Enter cTcpClt::cTcpClt function.");

    u8AdrFm = AF_INET; // IPV4.
    u16LclPt = htons(12345);
    u32LclIpv4Adr = 0x0100007FU; /* 127.0.0.1 */
    u32RmtIpv4Adr = 0x0100007FU; /* 127.0.0.1 */
    s16LclSktId = -1;
    s16RmtSktId = -1;
    eTcpBlkMd = enTcpBlkMd::Blk;

    LogInf("u8AdrFm = %d", u8AdrFm);
    LogInf("u16LclPt = %04X", u16LclPt);
    LogInf("u32LclIpv4Adr = 0x%08X", u32LclIpv4Adr);
    LogInf("u32RmtIpv4Adr = 0x%08X", u32RmtIpv4Adr);
    LogInf("s16LclSktId = %d", s16LclSktId);
    LogInf("s16RmtSktId = %d", s16RmtSktId);
    LogInf("eTcpBlkMd = %d", eTcpBlkMd);

    LogTr("Exit cTcpClt::cTcpClt function.");
}

cTcpClt::cTcpClt(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt)
{
    LogTr("Enter cTcpSer::cTcpSer function.");

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
    LogInf("eTcpBlkMd = %d", eTcpBlkMd);

    LogTr("Exit cTcpSer::cTcpSer function.");
}

cTcpClt::~cTcpClt(void)
{
    LogTr("Enter cTcpClt::~cTcpClt function.");

    LogTr("Exit cTcpClt::~cTcpClt function.");
}

err cTcpClt::SetBlkMd(enTcpBlkMd eTcpBlkMd)
{
    LogTr("Enter cTcpClt::SetBlkMd function.");

    err erRtn = EC_NOK;

    this->eTcpBlkMd = eTcpBlkMd;
    LogInf("this->eTcpBlkMd = %d", eTcpBlkMd);

    return erRtn;

    LogTr("Exit cTcpClt::SetBlkMd function.");
}

enTcpBlkMd cTcpClt::GetBlkMd(void)
{
    LogTr("Enter cTcpClt::GetBlkMd function.");

    return eTcpBlkMd;

    LogTr("Exit cTcpClt::GetBlkMd function.");
}

err cTcpClt::Conn(void)
{
    LogTr("Enter cTcpClt::Conn function.");

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

    LogTr("Exit cTcpClt::Conn function.");

    return erRtn;
}

err cTcpClt::Snd(u8* pu8Buf, u32 u32Sz)
{
    LogTr("Enter cTcpClt::Snd function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) && (u32Sz > 0U))
    {
        LogInf("Send data: 0x%s", HexArrayToString(pu8Buf, u32Sz).c_str());

        send(s16LclSktId, pu8Buf, u32Sz, 0);
        erRtn = EC_OK;
    }
    else
    {
        erRtn = EC_NOK;
    }

    LogTr("Exit cTcpClt::Snd function.");

    return erRtn;
}

err cTcpClt::Recv(u8* pu8Buf, u32* pu32Sz)
{
    LogTr("Enter cTcpClt::Recv function.");

    err erRtn = EC_NOK;

    if((pu8Buf != NULL) && (pu32Sz != NULL))
    {
        ssize_t s64RecvRst = recv(s16LclSktId, pu8Buf, RECV_BUF_SIZE, static_cast<int>(eTcpBlkMd));

        if(s64RecvRst > 0)
        {
            LogInf("Recv data: 0x%s", HexArrayToString(pu8Buf, s64RecvRst).c_str());

            *pu32Sz = s64RecvRst;
            erRtn = EC_OK;
        }
        else if(s64RecvRst == 0)
        {
            *pu32Sz = 0U;
            erRtn = EC_NOK;
            LogWrn("Recv disconnected.");
        }
        else if(s64RecvRst == -1)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                // Receive buffer is empty.
                *pu32Sz = 0U;
                erRtn = EC_OK;
            }
            else
            {
                *pu32Sz = 0U;
                erRtn = EC_NOK;
                LogErr("Recv failed.");
            }
        }
        else
        {
            *pu32Sz = 0U;
            erRtn = EC_NOK;
            LogErr("Recv failed.");
        }
    }
    else
    {
        *pu32Sz = 0U;
        erRtn = EC_NOK;
        LogErr("NULL point.");
    }

    LogTr("Exit cTcpClt::Recv function.");

    return erRtn;
}

bool cTcpClt::IsConn(void)
{
    LogTr("Enter cTcpClt::IsConn function.");

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

    LogTr("Exit cTcpClt::IsConn function.");

    return bConn;
}

// Tcp server.
// TcpSer.Lsn();
// TcpSer.SetBlkMd(enTcpBlkMd::NonBlk);
// u8 au8Buf[1024] = {0U};
// u32 u32RecvSz = 0;
//
// do
// {
//     TcpSer.Recv(au8Buf, &u32RecvSz);
//
//     if(u32RecvSz > 0)
//     {
//         au8Buf[0] = 0x11;
//         au8Buf[1] = 0x22;
//         u32RecvSz = 2;
//         TcpSer.Snd(au8Buf, u32RecvSz);
//     }
// } while(TcpSer.IsConn() == true);

// Tcp client.
// TcpClt.Conn();
// TcpClt.SetBlkMd(enTcpBlkMd::NonBlk);
// u8 au8Buf[1024] = {0U};
// u32 u32RecvSz = 0;
// au8Buf[0] = 0x55U;
// au8Buf[1] = 0xAAU;
// TcpClt.Snd(au8Buf, 2);
//
// do
// {
//     TcpClt.Recv(au8Buf, &u32RecvSz);
//
//     if(u32RecvSz > 0)
//     {
//         au8Buf[0] = 0x55U;
//         au8Buf[1] = 0xAAU;
//         TcpClt.Snd(au8Buf, 2);
//     }
// } while(TcpClt.IsConn() == true);

#endif // TCP_H
