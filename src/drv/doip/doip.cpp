#include <iomanip>
#include "typ.h"
#include "com.h"
#include "err.h"
#include "log.h"
#include "tcp.h"
#include "doip.h"

#ifdef DOIP_H

std::string strHexToStr(const uint8_t* array, size_t length)
{
    std::stringstream ss;
    for(size_t i = 0; i < length; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(array[i]);
    }

    return ss.str();
}

clDoipSer::clDoipSer(void)
{
    LogTr("Enter clDoipSer::clDoipSer function.");

    memset(au8Eid, 0, sizeof(au8Eid));
    memset(au8Vin, 0, sizeof(au8Vin));
    memset(au8Gid, 0, sizeof(au8Gid));
    memset(au8Gid, 0, sizeof(au8Gid));
    u16LgcAdr = 0x0000u;

    LogTr("Exit clDoipSer::clDoipSer function.");
}

clDoipSer::~clDoipSer(void)
{
    LogTr("Enter clDoipSer::~clDoipSer function.");

    LogTr("Exit clDoipSer::~clDoipSer function.");
}

err clDoipSer::erRespVehId(void)
{
    LogTr("Enter clDoipSer::erRespVehId function.");

    stHdr tHdr = {0u};
    stPkt tPkt = {0u};
    stVehIdRespMsg tVehIdRespMsg = {0u};
    err erRtn = EC_NOK;

    tHdr.u8ProtVer = u8ProtVer;
    LogInf("tHdr.u8ProtVer = 0x%02X", tHdr.u8ProtVer);
    tHdr.u8InvProtVer = ~tHdr.u8ProtVer;
    LogInf("tHdr.u8InvProtVer = 0x%02X", tHdr.u8InvProtVer);
    tHdr.u16PldTyp = u16CvtEndn(static_cast<u16>(enPldTyp::VehIdRespMsg));
    LogInf("tHdr.u16PldTyp = 0x%04X", tHdr.u16PldTyp);
    tHdr.u32PldLen = u32CvtEndn((u32)sizeof(tVehIdRespMsg));
    LogInf("tHdr.u32PldLen = 0x%08X", tHdr.u32PldLen);
    tPkt.tHdr = tHdr;
    LogInf("tPkt.tHdr = 0x%s", strHexToStr((unsigned char*)&tPkt.tHdr, sizeof(tPkt.tHdr)).c_str());
    memcpy(tVehIdRespMsg.au8Vin, au8Vin, sizeof(tVehIdRespMsg.au8Vin));
    LogInf("tVehIdRespMsg.au8Vin = 0x%s", strHexToStr(tVehIdRespMsg.au8Vin, sizeof(tVehIdRespMsg.au8Vin)).c_str());
    tVehIdRespMsg.u16LgcAdr = u16LgcAdr;
    LogInf("tVehIdRespMsg.u16LgcAdr = 0x%04X", tVehIdRespMsg.u16LgcAdr);
    memcpy(tVehIdRespMsg.au8Eid, au8Eid, sizeof(tVehIdRespMsg.au8Eid));
    LogInf("tVehIdRespMsg.au8Eid = 0x%s", strHexToStr(tVehIdRespMsg.au8Eid, sizeof(tVehIdRespMsg.au8Eid)).c_str());
    memcpy(tVehIdRespMsg.au8Gid, au8Gid, sizeof(tVehIdRespMsg.au8Gid));
    LogInf("tVehIdRespMsg.au8Gid = 0x%s", strHexToStr(tVehIdRespMsg.au8Gid, sizeof(tVehIdRespMsg.au8Gid)).c_str());
    tVehIdRespMsg.u8FthrActReq = static_cast<u8>(enFthrActCd::NoFthrActReq);
    LogInf("tVehIdRespMsg.u8FthrActReq = 0x%02X", tVehIdRespMsg.u8FthrActReq);
    tVehIdRespMsg.u8VinGidSyncSts = static_cast<u8>(enVinGidSyncStsCd::VinGidNSync);
    LogInf("tVehIdRespMsg.u8VinGidSyncSts = 0x%02X", tVehIdRespMsg.u8VinGidSyncSts);
    memcpy(tPkt.au8PlgMsg, &tVehIdRespMsg, sizeof(tVehIdRespMsg));
    LogInf("tPkt.au8PlgMsg = 0x%s", strHexToStr(tPkt.au8PlgMsg, sizeof(tVehIdRespMsg)).c_str());

    if(cTcpSer.erSnd((u8*)&tPkt, sizeof(tHdr) + sizeof(tVehIdRespMsg)) == EC_OK)
    {
        LogScs("Vehicle declaration message successfully sent.");

        erRtn = EC_OK;
    }
    else
    {
        LogErr("Vehicle declaration message sending failed.");

        erRtn = EC_NOK;
    }

    LogTr("Exit clDoipSer::erRespVehId function.");

    return erRtn;
}

clDoipClt::clDoipClt(void)
{
    LogTr("Enter clDoipClt::clDoipClt function.");

    u8ProtVer = static_cast<u8>(enProtVer::Iso2012);
    u8InvProtVer = ~u8ProtVer;
    memset(au8Eid, 0, sizeof(au8Eid));
    memset(au8Vin, 0, sizeof(au8Vin));
    memset(au8Gid, 0, sizeof(au8Gid));
    memset(au8Gid, 0, sizeof(au8Gid));
    u16LgcAdr = 0x0000u;
    u16SrcAdr = 0x0000u;
    u16TgtAdr = 0x0000u;

    LogTr("Exit clDoipClt::clDoipClt function.");
}

clDoipClt::~clDoipClt(void)
{
    LogTr("Enter clDoipClt::~clDoipClt function.");

    LogTr("Exit clDoipClt::~clDoipClt function.");
}

err clDoipClt::erConn(void)
{
    LogTr("Enter clDoipClt::erConn function.");

    err erRtn = EC_NOK;

    cTcpClt.erSetNetParm(0x0100007Fu, 0x0100007Fu, 0x5834u, 0x5934u);

    if(cTcpClt.erConn() == EC_OK)
    {
        LogScs("TCP successfully connected.");

        cTcpClt.erSetBlkMd(enTcpBlkMd::Blk);
        erRtn = EC_OK;
    }
    else
    {
        LogErr("TCP connection failed.");

        erRtn = EC_NOK;
    }

    LogTr("Exit clDoipClt::erConn function.");

    return erRtn;
}

err clDoipClt::erReqRtgAct(void)
{
    LogTr("Enter clDoipClt::erReqRtgAct function.");

    err erRtn = EC_NOK;

    stHdr tHdr = {0u};
    stPkt tPkt = {0u};
    stPldTypRtgActReq tPldTypRtgActReq = {0u};
    tHdr.u8ProtVer = u8ProtVer;
    LogInf("tHdr.u8ProtVer = 0x%02X", tHdr.u8ProtVer);
    tHdr.u8InvProtVer = ~tHdr.u8ProtVer;
    LogInf("tHdr.u8InvProtVer = 0x%02X", tHdr.u8InvProtVer);
    tHdr.u16PldTyp = u16CvtEndn(static_cast<u16>(enPldTyp::RtgActReq));
    LogInf("tHdr.u16PldTyp = 0x%04X", tHdr.u16PldTyp);
    tHdr.u32PldLen = u32CvtEndn((u32)sizeof(stPldTypRtgActReq));
    LogInf("tHdr.u32PldLen = 0x%08X", tHdr.u32PldLen);
    tPkt.tHdr = tHdr;
    LogInf("tPkt.tHdr = 0x%s", strHexToStr((unsigned char*)&tPkt.tHdr,
           sizeof(tPkt.tHdr)).c_str());
    tPldTypRtgActReq.u16SrcAdr = u16SrcAdr;
    LogInf("tPldTypRtgActReq.u16SrcAdr = 0x%04X", tPldTypRtgActReq.u16SrcAdr);
    tPldTypRtgActReq.u8ActTyp = static_cast<u8>(enRtgActReqTyp::Dflt);
    LogTr("tPldTypRtgActReq.u8AckCd = 0x%02X", tPldTypRtgActReq.u8ActTyp);
    tPldTypRtgActReq.u32Rsv = 0x00000000ul;
    LogTr("tPldTypRtgActReq.u32Rsv = 0x%08X", tPldTypRtgActReq.u32Rsv);
    tPldTypRtgActReq.u32RsvOem = 0x00000000ul;
    LogTr("tPldTypRtgActReq.u32RsvOem = 0x%08X", tPldTypRtgActReq.u32RsvOem);

    if(cTcpClt.erSnd((u8*)&tPkt, sizeof(tHdr) + sizeof(tPldTypRtgActReq)) ==
       EC_OK)
    {
        LogScs("Request route activation successful.");

        erRtn = EC_OK;
    }
    else
    {
        LogErr("Request for route activation failed.");

        erRtn = EC_NOK;
    }

    LogTr("Exit clDoipClt::erReqRtgAct function.");

    return erRtn;
}

err clDoipClt::erDisc(void)
{
    LogTr("Enter clDoipClt::erDisc function.");

    err erRtn = EC_NOK;

    if(cTcpClt.erDisc() == EC_OK)
    {
        LogScs("TCP disconnected successfully.");

        erRtn = EC_OK;
    }
    else
    {
        LogErr("TCP disconnection failed.");

        erRtn = EC_NOK;
    }

    LogTr("Exit clDoipClt::erDisc function.");

    return erRtn;
}

err clDoipClt::erReqDiag(void)
{
    LogTr("Enter clDoipClt::erReqDiag function.");

    stHdr tHdr = {0u};
    stPkt tPkt = {0u};
    err erRtn = EC_NOK;

    // tHdr.u8ProtVer = u8ProtVer;
    // LogInf("tHdr.u8ProtVer = 0x%02X", tHdr.u8ProtVer);
    // tHdr.u8InvProtVer = u8InvProtVer;
    // LogInf("tHdr.u8InvProtVer = 0x%02X", tHdr.u8InvProtVer);
    // tHdr.u16PldTyp = u16CvtEndn(static_cast<u16>(enPldTyp::DiagMsg));
    // LogInf("tHdr.u16PldTyp = 0x%04X", tHdr.u16PldTyp);
    // tHdr.u32PldLen = u32CvtEndn((u32)sizeof());
    // LogInf("tHdr.u32PldLen = 0x%08X", tHdr.u32PldLen);
    // tPkt.tHdr = tHdr;

    LogTr("Exit clDoipClt::erReqDiag function.");

    return erRtn;
}

#endif // DOIP_H
