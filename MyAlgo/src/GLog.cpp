#include "GLog.h"
#include "g_logger.h"
#include <stdarg.h>

namespace Lee {
    namespace GosLog {

        BOOL GosInitLog()
        {
            Logger          *pLogger = NULL;
            GLogFactory     *pLogFactory = GLogFactory::GetInstance();

            if (!pLogFactory)
            {
                return FALSE;
            }

            pLogger = pLogFactory->GetLogger();
            if (!pLogger)
            {
                return FALSE;
            }

            pLogger->SetLogToStdout(TRUE);
            pLogger->SetLogToFile(TRUE, 2, 4 * 1024 * 1024, 0);
            pLogger->SetLogLevel(LOG_INFO);

            return TRUE;
        }

        BOOL GosInitLog(PLAT_CFG_T *pstPlatCfg)
        {
            Logger          *pLogger = NULL;
            GLogFactory     *pLogFactory = GLogFactory::GetInstance();

            if (!pLogFactory)
            {
                return FALSE;
            }

            pLogger = pLogFactory->GetLogger();
            if (!pLogger)
            {
                return FALSE;
            }

            pLogger->SetLogToStdout(pstPlatCfg->bLogToStdout);
            pLogger->SetLogToFile(pstPlatCfg->bLogToFile,
                                  pstPlatCfg->ulLogFileNum,
                                  pstPlatCfg->ulLogFileSize,
                                  pstPlatCfg->ulLogBufTime);
                                  
            pLogger->SetLogLevel(pstPlatCfg->ulLogLevel);

            return TRUE;
        }

        VOID GosLog(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...)
        {
            GLogFactory     *pLogFactory;
            Logger          *pLogger;
            va_list         vaLog;
            size_t          ulLen;
            CHAR            acLog[1024] = { 0 };

            pLogFactory = GLogFactory::GetInstance();
            if (!pLogFactory)
            {
                return;
            }

            pLogger = pLogFactory->GetLogger();
            if (!pLogger)
            {
                return;
            }

            va_start(vaLog, szFormat);

            ulLen = vsnprintf(acLog, sizeof(acLog) - 1, szFormat, vaLog);
            acLog[ulLen] = '\0';

            va_end(vaLog);

            pLogger->Log(szLogModule, ulLogLevel, acLog);
        }

        VOID GosLog(UINT32 ulLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...)
        {
            GLogFactory     *pLogFactory;
            Logger          *pLogger;
            va_list         vaLog;
            size_t          ulLen;
            CHAR            acLog[1024] = { 0 };

            pLogFactory = GLogFactory::GetInstance();
            if (!pLogFactory)
            {
                return;
            }

            pLogger = pLogFactory->GetLogger();
            if (!pLogger)
            {
                return;
            }

            va_start(vaLog, szFormat);

            ulLen = vsnprintf(acLog, sizeof(acLog) - 1, szFormat, vaLog);
            acLog[ulLen] = '\0';

            va_end(vaLog);

            pLogger->Log(ulLogModule, ulLogLevel, acLog);
        }

        VOID GosLog(UINT32 ulLogLevel, const CHAR *szFormat, ...)
        {
            GLogFactory     *pLogFactory;
            Logger          *pLogger;
            va_list         vaLog;
            size_t          ulLen;
            CHAR            acLog[1024] = { 0 };

            pLogFactory = GLogFactory::GetInstance();
            if (!pLogFactory)
            {
                return;
            }

            pLogger = pLogFactory->GetLogger();
            if (!pLogger)
            {
                return;
            }

            va_start(vaLog, szFormat);

            ulLen = vsnprintf(acLog, sizeof(acLog) - 1, szFormat, vaLog);
            acLog[ulLen] = '\0';

            va_end(vaLog);

            pLogger->Log(ulLogLevel, acLog);
        }


    }; // end of  namespace GosLog 
}; // end of namespace Lee 

