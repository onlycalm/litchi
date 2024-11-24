#ifndef DOIP_H
#define DOIP_H

#include "tcp.h"
#include "typ.h"
#include <cstdio>

// ISO 13400-2-2012.

#define PLD_MSG_SIZE 4096u
#define EID_SIZE     6u
#define VIN_SIZE     17u
#define GID_SIZE     6u

enum class enNdTypCd : u8
{
    // Mandatory.
    DoipGw = 0x00u,
    DoipNd = 0x01u,
    // Reserved by this part of ISO 13400. 0x02 to 0xFF.
};

enum class enDiagPwrMdInfRespCd : u8
{
    // Mandatory.
    NRdy = 0x00u,
    Rdy = 0x01u,
    NSpt = 0x02u,
    // Reserved by this part of ISO 13400. 0x03 to 0xFF.
};

enum class enDiagMsgNackCd : u8
{
    // Reserved by this part of ISO 13400. 0x00 to 0x01.
    // Mandatory.
    InvSrcAdr    = 0x02u,
    unknTgtAdr   = 0x03u,
    DiagMsgTooLg = 0x04u,
    OutOfMsg     = 0x05u,
    // Optional.
    TgtUnrch     = 0x06u,
    UnknNet      = 0x07u,
    TransProtEr  = 0x08u,
    // Reserved by this part of ISO 13400. 0x09 to 0xFF.
};

enum class enDiagMsgPosAckCd : u8
{
    // Mandatory.
    RtgCfmAckMsg = 0x00u,
    // Reserved by this part of ISO 13400. 0x01 to 0xFF.
};

enum class enRtgActRespCd : u8
{
    // Mandatory.
    RtgActDndUnknSrcAdr = 0x00u,
    RtgActDndSktReg     = 0x01u,
    RtgActDndSrcAdrDif  = 0x02u,
    RtgActDndSrcAdrReg  = 0x03u,
    // Optional.
    RtgActDndMsAuth     = 0x04u,
    RtgActDndRejCfm     = 0x05u,
    // Mandatory.
    RtgActDndUnsptTyp   = 0x06u,
    // Reserved by this part of ISO 13400. 0x07 to 0x0F.
    RtgScsAct           = 0x10u,
    // Optional.
    RtgWlAct            = 0x11u,
    // Reserved by this part of ISO 13400. 0x12 to 0xDF.
    // Vehicle-manufacturer specific. 0xE0 to 0xFE.
    // Reserved by this part of ISO 13400. 0xFF.
};

enum class enRtgActReqTyp : u8
{
    // Mandatory.
    Dflt     = 0x00u,
    WwhObd   = 0x01u,
    // ISO/SAE reserved. 0x02 to 0xDF.
    // Optional.
    CntrlSec = 0xE0u,
    // Available for additional OEM-specific use. 0xE1 to 0xFF.
};

enum class enVinGidSyncStsCd : u8
{
    // Mandatory.
    VinGidSync  = 0x00u,
    // Reserved by this part of ISO 13400. 0x01 to 0x0F.
    VinGidNSync = 0x10u,
    // Reserved by this part of ISO 13400. 0x11 to 0xFF.
};

enum class enFthrActCd : u8
{
    // Mandatory.
    NoFthrActReq         = 0x00u,
    // Reserved by this part of ISO 13400. 0x01 to 0x0F.
    // Optional.
    RtActReqInitCntrlSec = 0x10u,
    // Available for additional OEM-specific use.
};

enum class enDoipHdrNack : u8
{
    // Mandatory.
    IncorPatnFmt = 0x00u,
    UnknPldTyp   = 0x01u,
    MsgTooLg     = 0x02u,
    OutOfMsg     = 0x03u,
    InvldPldLen  = 0x04u,
    // Reserved by this part of ISO 13400. 0x05 to 0xFF. Mandatory.
};

enum class enPldTyp : u16
{
    // Mandatory.
    DoipHdrNack       = 0x0000u,
    VehIdReqMsg       = 0x0001u,
    // Optional.
    VehIdReqMsgEid    = 0x0002u,
    // Mandatory.
    VehIdReqMsgVin    = 0x0003u,
    VehIdRespMsg      = 0x0004u,
    RtgActReq         = 0x0005u,
    RtgActResp        = 0x0006u,
    AlvChkReq         = 0x0007u,
    AlvChkResp        = 0x0008u,
    // Reserve. 0x0009 to 0x4000.
    // Optional.
    DoipEntyStsReq    = 0x4001u,
    DoipEntyStsResp   = 0x4002u,
    // Mandatory.
    DiagPwrMdInfoReq  = 0x4003u,
    DiagPwrMdInfoResp = 0x4004u,
    // Reserve. 0x4005 to 0x8000.
    // Mandatory.
    DiagMsg           = 0x8001u,
    DiagMsgPosAck     = 0x8002u,
    DiagMesNegAck     = 0x8003u,
    // Reserve. 0x8004 to 0xEFFF.
    // Optional.
    // Reserve for manufacturer-specificuse. 0xF000 to 0xFFFF.
};

enum class enProtVer : u8
{
    // Reserve. 0x00.
    Iso2010     = 0x01u,
    Iso2012     = 0x02u,
    // Reserved by this part of ISO 13400. 0x03 to 0xFE.
    VehIdReqMsg = 0xFFu, // Default.
};

typedef struct
{
    // Mandatory.
    u8 u8NdTyp;
    u8 u8MaxCncrtSkt;
    u8 u8CrtOpnSkt;
    // Optional.
    u8 u8MaxDatSz;
} stDoipEntyStsResp;

typedef struct
{
    u8 u8DiagPwrMd;
} stDiagPwrMdInfResp;

typedef struct
{
    // Mandatory.
    u16 u16SrcAdr;
} stPldTypAlvChkResp;

#pragma pack(push, 1)
typedef struct
{
    // Mandatory.
    u16 u16SrcAdr;
    u16 u16TgtAdr;
    u8 u8Nack;
    // Optional.
    // Previous diagnostic message data.
} stPldTypDiagMsgNack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    // Mandatory.
    u16 u16SrcAdr;
    u16 u16TgtAdr;
    u8 u8AckCd;
    // Optional.
    // Previous diagnostic message data.
} stPldTypDiagMsgPosAck;
#pragma pack(pop)

typedef struct
{
    // Mandatory.
    u16 u16SrcAdr;
    u16 u16TgtAdr;
    // User data.
} stPldTypDiagMsg;

#pragma pack(push, 1)
typedef struct
{
    // Mandatory.
    u16 u16LgcAdrExtTstEqpt;
    u16 u16LgcAdrDoipEnty;
    u8 u8RtgActRespCd;
    u32 u32Rsv;
    // Optional.
    u32 u32RsvOem;
} stPldTypRtgActResp;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    // Mandatory.
    u16 u16SrcAdr;
    u8 u8ActTyp;
    u32 u32Rsv;
    // Optional.
    u32 u32RsvOem;
} stPldTypRtgActReq;
#pragma pack(pop)

typedef struct
{
    u8 au8Eid[EID_SIZE];
} stVehIdReqMsgEid;

typedef struct
{
    u8 au8Vin[VIN_SIZE];
} stVehIdReqMsgVin;

#pragma pack(push, 1)
typedef struct
{
    // Mandatory.
    u8 au8Vin[VIN_SIZE];
    u16 u16LgcAdr;
    u8 au8Eid[EID_SIZE];
    u8 au8Gid[GID_SIZE];
    u8 u8FthrActReq;

    // Optional.
    u8 u8VinGidSyncSts;
} stVehIdRespMsg;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    u8 u8ProtVer;
    u8 u8InvProtVer; //Equals the u8ProtVer XOR 0xFF.
    u16 u16PldTyp;
    u32 u32PldLen;
} stHdr;
#pragma pack(pop)

typedef struct
{
    stHdr tHdr;
    u8 au8PlgMsg[PLD_MSG_SIZE];
} stPkt;

class clDoipSer
{
public:
    clDoipSer(void);
    ~clDoipSer(void);
    err erRespVehId(void);

private:
    u8 au8Eid[EID_SIZE];
    u8 au8Vin[VIN_SIZE];
    u8 au8Gid[GID_SIZE];
    u8 u8ProtVer;
    u8 u8InvProtVer;
    u16 u16LgcAdr;
    u16 u16SrcAdr;
    u16 u16TgtAdr;
    clTcpSer cTcpSer;
};

class clDoipClt
{
public:
    clDoipClt(void);
    ~clDoipClt(void);
    err erConn(void);
    err erDisc(void);
    err erReqRtgAct(void);
    err erReqDiag(void);

private:
    u8 au8Eid[EID_SIZE];
    u8 au8Vin[VIN_SIZE];
    u8 au8Gid[GID_SIZE];
    u8 u8ProtVer;
    u8 u8InvProtVer;
    u16 u16LgcAdr;
    u16 u16SrcAdr;
    u16 u16TgtAdr;
    clTcpClt cTcpClt;
};

#endif // DOIP_H
