#ifndef MYALGO_INCLUDE_GLOG_H
#define MYALGO_INCLUDE_GLOG_H

#include "gos_typedef.h"

namespace Lee {
    namespace GosLog {

        struct IP_ADDR_T
        {
            UINT8               aucIP[4];               /* IP  */
            UINT16              usPort;                 /* 端口 */
        };

        struct PEER_ADDR_T
        {
            UINT8               aucIP[4];           /* IP  */
            UINT16              usPort;             /* 端口 */
            UINT8               ucTCPMode;
        };

        struct PLAT_CFG_T
        {
            UINT16              usInstID;           /* 本地平台标识 */
            BOOL                bMultiInst;         /* 是否允许多实例运行，缺省否 */
            UINT32              ulMaxUniqueMsgNum;  /* 消息查重最大缓存消息个数*/
            UINT32              ulMsgMaxWaitTime;   /* socket异步消息最大等待时间 */
            BOOL                bServerMode;
            IP_ADDR_T           stSelfAddr;         /* 本地侦听地址 */
            UINT32              ulPeerNum;
            PEER_ADDR_T         astPeerAddr[256];   /* 对端地址 */
            UINT32              ulLogLevel;
            BOOL                bLogToStdout;
            BOOL                bLogToFile;
            UINT32              ulLogFileNum;
            UINT32              ulLogFileSize;
            UINT32              ulLogBufTime;       /* 日志缓存时间，单位秒 */
            UINT32              ulMaxSysLogNum;     /* 最大日志缓存数 */
        };

        BOOL GosInitLog();
        BOOL GosInitLog(PLAT_CFG_T *pstPlatCfg);
        VOID GosLog(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...);
        VOID GosLog(UINT32 ulLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...);
        VOID GosLog(UINT32 ulLogLevel, const CHAR *szFormat, ...);
    }; // end of  namespace GosLog 
}; // end of namespace Lee 

#endif