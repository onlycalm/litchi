#ifndef TCP_H
#define TCP_H

#include "typ.h"
#include <sys/socket.h>

#define RECV_BUF_SIZE 4096u

enum class enTcpBlkMd : int
{
    Blk    = 0,
    Pv     = MSG_PEEK,
    WaitFl = MSG_WAITALL, // Waiting for receiving bytes to be full.
    NonBlk = MSG_DONTWAIT
};

class clTcpSer
{
public:
    clTcpSer(void);
    clTcpSer(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt);
    ~clTcpSer(void);
    err erSetBlkMd(enTcpBlkMd eTcpBlkMd);
    err erSetNetParm(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt);
    enTcpBlkMd eGetBlkMd(void);
    err erLsn(void);
    err erDisc(void);
    err erSnd(u8* pu8Buf, u32 u32Sz);
    err erRecv(u8* pu8Buf, u32* u32Sz);
    bool bIsConn(void);

private:
    u8 u8RecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16LclPt;
    u16 u16RmtPt;
    u32 u32LclIpv4Adr;
    u32 u32RmtIpv4Adr;
    s16 s16LclSktId;
    s16 s16RmtSktId;
    enTcpBlkMd eTcpBlkMd;
};

class clTcpClt
{
public:
    clTcpClt(void);
    clTcpClt(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16LclPt);
    ~clTcpClt(void);
    err erSetBlkMd(enTcpBlkMd eTcpBlkMd);
    err erSetNetParm(u32 u32LclIpv4Adr, u32 u32RmtIpv4Adr, u16 u16RmtPt,
                     u16 u16LclPt);
    enTcpBlkMd eGetBlkMd(void);
    err erConn(void);
    err erDisc(void);
    err erSnd(u8* pu8Buf, u32 u32Sz);
    err erRecv(u8* pu8Buf, u32* u32Sz);
    bool bIsConn(void);

private:
    u8 u8RecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16LclPt;
    u16 u16RmtPt;
    u32 u32LclIpv4Adr;
    u32 u32RmtIpv4Adr;
    s16 s16LclSktId;
    s16 s16RmtSktId;
    enTcpBlkMd eTcpBlkMd;
};

#endif // TCP_H
