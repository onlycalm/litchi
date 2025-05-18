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
    clTcpSer(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt);
    ~clTcpSer(void);
    err erSetBlkMd(enTcpBlkMd eTcpBlkMd);
    err erSetNetParm(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt);
    enTcpBlkMd eGetBlkMd(void);
    err erLsn(void);
    err erDisc(void);
    err erSnd(u8* pu8Buf, u32 u32Sz);
    err erRecv(u8* pu8Buf, u32* u32Sz);
    bool bIsConn(void);

private:
    u8 u8RecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16SrcPt;
    u16 u16TgtPt;
    u32 u32SrcIpAdr;
    u32 u32TgtIpAdr;
    s16 s16SrcSktId;
    s16 s16TgtSktId;
    enTcpBlkMd eTcpBlkMd;
};

class clTcpClt
{
public:
    clTcpClt(void);
    clTcpClt(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt);
    ~clTcpClt(void);
    err erSetBlkMd(enTcpBlkMd eTcpBlkMd);
    err erSetNetParm(u32 u32SrcIpAdr, u32 u32TgtIpAdr, u16 u16SrcPt,
                     u16 u16TgtPt);
    enTcpBlkMd eGetBlkMd(void);
    err erConn(void);
    err erDisc(void);
    err erSnd(u8* pu8Buf, u32 u32Sz);
    err erRecv(u8* pu8Buf, u32* u32Sz);
    bool bIsConn(void);

private:
    u8 u8RecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16SrcPt;
    u16 u16TgtPt;
    u32 u32SrcIpAdr;
    u32 u32TgtIpAdr;
    s16 s16SrcSktId;
    s16 s16TgtSktId;
    enTcpBlkMd eTcpBlkMd;
};

#endif // TCP_H
