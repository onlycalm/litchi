#ifndef TCP_H
#define TCP_H

#include "typ.h"
#include <sys/socket.h>

#define RECV_BUF_SIZE 1024

enum class enTcpBlkMd : int
{
    Blk = 0,
    Pv = MSG_PEEK,
    WaitFl = MSG_WAITALL, // Waiting for receiving bytes to be full.
    NonBlk = MSG_DONTWAIT
};

class cTcpSer
{
public:
    cTcpSer(void);
    cTcpSer(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt);
    ~cTcpSer(void);
    err SetBlkMd(enTcpBlkMd eTcpBlkMd);
    enTcpBlkMd GetBlkMd(void);
    err Lsn(void);
    err Snd(u8* pu8Buf, u32 u32Sz);
    err Recv(u8* pu8Buf, u32* u32Sz);
    bool IsConn(void);

private:
    byte byRecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16LclPt;
    u32 u32LclIpv4Adr;
    u32 u32RmtIpv4Adr;
    s16 s16LclSktId;
    s16 s16RmtSktId;
    enTcpBlkMd eTcpBlkMd;
};

class cTcpClt
{
public:
    cTcpClt(void);
    cTcpClt(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt);
    ~cTcpClt(void);
    err SetBlkMd(enTcpBlkMd eTcpBlkMd);
    enTcpBlkMd GetBlkMd(void);
    err Conn(void);
    err Snd(u8* pu8Buf, u32 u32Sz);
    err Recv(u8* pu8Buf, u32* u32Sz);
    bool IsConn(void);

private:
    byte byRecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16LclPt;
    u32 u32LclIpv4Adr;
    u32 u32RmtIpv4Adr;
    s16 s16LclSktId;
    s16 s16RmtSktId;
    enTcpBlkMd eTcpBlkMd;
};

#endif // TCP_H
