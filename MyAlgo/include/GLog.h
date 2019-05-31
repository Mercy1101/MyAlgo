#ifndef MYALGO_INCLUDE_GLOG_H
#define MYALGO_INCLUDE_GLOG_H

#include "gos_typedef.h"

namespace Lee {
    namespace GosLog {

        struct IP_ADDR_T
        {
            UINT8               aucIP[4];               /* IP  */
            UINT16              usPort;                 /* �˿� */
        };

        struct PEER_ADDR_T
        {
            UINT8               aucIP[4];           /* IP  */
            UINT16              usPort;             /* �˿� */
            UINT8               ucTCPMode;
        };

        struct PLAT_CFG_T
        {
            UINT16              usInstID;           /* ����ƽ̨��ʶ */
            BOOL                bMultiInst;         /* �Ƿ������ʵ�����У�ȱʡ�� */
            UINT32              ulMaxUniqueMsgNum;  /* ��Ϣ������󻺴���Ϣ����*/
            UINT32              ulMsgMaxWaitTime;   /* socket�첽��Ϣ���ȴ�ʱ�� */
            BOOL                bServerMode;
            IP_ADDR_T           stSelfAddr;         /* ����������ַ */
            UINT32              ulPeerNum;
            PEER_ADDR_T         astPeerAddr[256];   /* �Զ˵�ַ */
            UINT32              ulLogLevel;
            BOOL                bLogToStdout;
            BOOL                bLogToFile;
            UINT32              ulLogFileNum;
            UINT32              ulLogFileSize;
            UINT32              ulLogBufTime;       /* ��־����ʱ�䣬��λ�� */
            UINT32              ulMaxSysLogNum;     /* �����־������ */
        };

        BOOL GosInitLog();
        BOOL GosInitLog(PLAT_CFG_T *pstPlatCfg);
        VOID GosLog(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...);
        VOID GosLog(UINT32 ulLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...);
        VOID GosLog(UINT32 ulLogLevel, const CHAR *szFormat, ...);
    }; // end of  namespace GosLog 
}; // end of namespace Lee 

#endif