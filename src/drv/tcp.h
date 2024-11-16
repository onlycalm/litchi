#ifndef TCP_H
#define TCP_H

#include "typ.h"

#define RECV_BUF_SIZE 1024

enum class enTcpBlkMd : uint8_t
{
    Blk,
    NonBlk,
};

class cTcp
{
public:
    cTcp(void);
    cTcp(u32 u32SvrIpv4Adr, u32 u32CltIpv4Adr, u16 u16SvrPt);
    ~cTcp(void);
    Err SetBlkMd(enTcpBlkMd eTcpBlkMd);
    void Lsn(void);
    int GetRecvSz(void);
    int Read(byte* abyDat);
    void Send(byte* pbyMsg, u32 u32Size);

private:
    byte byRecvBuf[RECV_BUF_SIZE] = {0};
    u8 u8AdrFm;
    u16 u16SvrPt;
    u32 u32SvrIpv4Adr;
    u32 u32CltIpv4Adr;
    s16 s16SvrSktId;
    s16 s16CltSktId;
};

#endif // TCP_H
