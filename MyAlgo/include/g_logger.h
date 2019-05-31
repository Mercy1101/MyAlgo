#ifndef G_LOGGER_H
#define G_LOGGER_H

#include <mutex>
#include "gos_typedef.h"
#include <windows.h>


namespace Lee {
    namespace GosLog {

class Logger
{
public :
    virtual VOID Log(UINT32 ulLogLevel, const CHAR *szFormat, va_list vaLog)=0;
    virtual VOID Log(UINT32 ulLogLevel, const CHAR *szFormat, ...)=0;
    virtual VOID Log(UINT32 ulLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...)=0;
    virtual VOID Log(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...)=0;

    virtual VOID SetLogModule(UINT32 ulLogModule, CHAR *szLogModule)=0;
    virtual VOID SetLogLevel(UINT32 ulLogLevel)=0;
    virtual VOID SetAllModuleLogLevel(UINT32 ulLogLevel)=0;
    virtual VOID SetLogLevel(UINT32 ulLogModule, UINT32 ulLogLevel)=0;
    virtual BOOL SetLogLevel(CHAR *szLogModule, UINT32 ulLogLevel)=0;

    virtual VOID SetLogToStdout(BOOL bEnable)=0;
    virtual VOID SetLogToFile(const BOOL   bEnable,
                              const UINT32 ulMaxFileNum=0,
                              const UINT32 ulMaxFileSize=1024*1024,
                              const UINT32 ulMaxBufTime=300) = 0;

    virtual VOID SetLogFilePrefix(CHAR *szLogFilePrefix)=0;

    virtual VOID GetModuleList(VECTOR<UINT32> &vLogModule) = 0;
    virtual VOID GetModule(UINT32 ulModuleID, CHAR *szModuleName, UINT32 &ulModuleLevel)=0;
};

struct LOG_MODULE_T
{
    UINT32      ulLogLevel;
    CHAR        acLogModule[32];
};

class GLogger : public Logger
{
public :
    GLogger();

    virtual VOID Log(UINT32 ulLogLevel, const CHAR *szFormat, va_list vaLog);
    virtual VOID Log(UINT32 ulLogLevel, const CHAR *szFormat, ...);
    virtual VOID Log(UINT32 ulLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...);
    virtual VOID Log(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...);

    virtual VOID SetLogModule(UINT32 ulLogModule, CHAR *szLogModule);
    virtual VOID SetAllModuleLogLevel(UINT32 ulLogLevel);
    virtual VOID SetLogLevel(UINT32 ulLogLevel);
    virtual VOID SetLogLevel(UINT32 ulLogModule, UINT32 ulLogLevel);
    virtual BOOL SetLogLevel(CHAR *szLogModule, UINT32 ulLogLevel);
    virtual VOID SetLogToStdout(BOOL bEnable);
    virtual VOID SetLogToFile(const BOOL   bEnable,
                              const UINT32 ulMaxFileNum=0,
                              const UINT32 ulMaxFileSize=1024*1024,
                              const UINT32 ulMaxBufTime=300);
    virtual VOID SetLogFilePrefix(CHAR *szLogFilePrefix);

    virtual VOID GetModuleList(VECTOR<UINT32> &vLogModule);
    virtual VOID GetModule(UINT32 ulModuleID, CHAR *szModuleName, UINT32 &ulModuleLevel);

private:
    UINT32                  m_ulLogLevel;
    BOOL                    m_bLogToStdout;
    BOOL                    m_bLogToFile;
    UINT32                  m_ulMaxFileNum;
    UINT32                  m_ulMaxFileSize;
    std::mutex              m_pLogFileMutex;
    UINT32                  m_ulCurrLogFileIndex;
    UINT32                  m_ulCurrLogFileSize;
    FILE                    *m_fpLog;
    BOOL                    m_bLogToRemote;
    UINT8                   m_aucRemoteLoggerAddr[4];
    UINT16                  m_usRemoteLoggerPort;
    CHAR                    m_acLogFilePrefix[256];
    UINT32                  m_ulLastFlushTime;
    UINT32                  m_ulMaxBufTime;
    CHAR                    m_acLogBuf[8*1024];

    MAP<UINT32, LOG_MODULE_T*>    m_mLogModule;

    VOID LogToFile(CHAR *szLog, UINT32 ulLogLen);
    VOID _Log(CHAR *szLogModule, UINT32 LogLevel, const CHAR *szFormat, va_list vaLog);
};

class GLogFactory
{
public:
    static GLogFactory* GetInstance();
    VOID SetLogger(Logger *pLogger);
    Logger* GetLogger();

private:
    Logger      *m_pLogger;

    GLogFactory();
};

static WORD GetLogColor(UINT32 ulLogLevel);

    }; // end of  namespace GosLog 
}; // end of namespace Lee 

#endif