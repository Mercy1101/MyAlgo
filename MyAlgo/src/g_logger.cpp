#include "gos_typedef.h"
#include "g_logger.h"
#include <io.h>
#include <sys/stat.h>
#include "Console_Color.h"
#include <string.h>     // for strcat_s()

namespace Lee {
    namespace GosLog {

BOOL gos_file_exist(CHAR* szFile)
{
    INT32   iRet;

    if (!szFile || *szFile == '\0')
    {
        return FALSE;
    }

    /* 0 == F_OK, 判断文件/目录是否存在 */
    iRet = _access(szFile, 0);

    if (iRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL gos_create_dir_ex(CHAR* szDir, UINT32 dwMode)
{
    return CreateDirectory(szDir, NULL);
}

BOOL gos_create_redir_ex(CHAR* szDir, UINT32 dwMode)
{
    CHAR    *szTempDir = NULL;
    CHAR    *szCurrDir = NULL;
    UINT32  ulLen;
    CHAR    *szFirstDir;
    UINT32  i;
    UINT32  ulStartPos = 0;
    UINT32  ulCurrLen = 0;

    if (szDir == NULL)
    {
        goto err;
    }

    ulLen = strlen(szDir);
    if (ulLen == 0)
    {
        goto err;
    }

    szTempDir = reinterpret_cast<CHAR *>(malloc(2 * (ulLen + 2)));
    if (NULL == szTempDir)
    {
        goto err;
    }
    szCurrDir = szTempDir + (ulLen + 2);

    /* 复制目录并添加目录结束符 */
    memcpy(szTempDir, szDir, ulLen);
    if (!IS_DIR_SPLIT(szTempDir[ulLen - 1]))
    {
        szTempDir[ulLen] = '/';
        ulLen++;    /* ulLen为有目录结束符的字符串长度 */
    }

    szTempDir[ulLen] = '\0';
    szFirstDir = szCurrDir;
    /* 判断开头的三个字符是否形如X:\ */
    if (ulLen < 3 || szDir[1] != ':' || !IS_DIR_SPLIT(szDir[2]))
    {
        goto err;
    }
    ulStartPos = 3;
    szFirstDir[0] = szDir[0];
    szFirstDir[1] = ':';
    szFirstDir[2] = DIR_SEPARATOR;


    /* 获取第一个目录 */
    for (i = ulStartPos; ; i++)
    {
        if (szDir[i] == '\0' || IS_DIR_SPLIT(szDir[i]))
        {
            ulCurrLen = i + 1;
            szFirstDir[ulCurrLen - 1] = '\0';
            break;
        }

        if (i == ulLen)
        {
            goto err;
        }
        szFirstDir[i] = szDir[i];
    }

    /* 创建第一个目录 */
    if (!gos_file_exist(szFirstDir))
    {
        if (!gos_create_dir_ex(szFirstDir, dwMode))
        {
            goto err;
        }
    }

    /* 循环创建子目录 */
    while (TRUE)
    {
        if (ulCurrLen >= ulLen)
        {
            break;
        }

        szCurrDir[ulCurrLen - 1] = DIR_SEPARATOR;
        for (i = ulCurrLen; ; i++)
        {
            if (IS_DIR_SPLIT(szTempDir[i]))
            {
                ulCurrLen = i + 1;
                szCurrDir[ulCurrLen - 1] = '\0';
                break;
            }
            szCurrDir[i] = szTempDir[i];
        }

        if (!gos_file_exist(szCurrDir))
        {
            if (!gos_create_dir_ex(szCurrDir, dwMode))
            {
                goto err;
            }
        }
    }

    /* 最后一次判断目录是否存在 */
    if (!gos_file_exist(szTempDir)) //也许应该用szCurrDir更好
    {
        goto err;
    }

    if (szTempDir)
    {
        free(szTempDir);
    }

    return TRUE;

err:
    if (szTempDir)
    {
        free(szTempDir);
    }

    return FALSE;
}

BOOL gos_create_redir(CHAR* szDir)
{
#ifdef _OSWIN32_
    UINT32  dwMode = 0;
#else
    UINT32  dwMode = S_IRWXU | S_IRWXG | S_IRWXO;   // FILE_ACCESS_MODE_ALL
#endif

    return gos_create_redir_ex(szDir, dwMode);
}

BOOL gos_get_file_time(CHAR* szFileName, UINT32 *pulCreateTime, UINT32 *pulModifyTime)
{
#ifdef _BCB_
    struct stat  stStat;
#else
    struct _stat stStat;
#endif
    *pulCreateTime = 0;
    *pulModifyTime = 0;

    if (_stat(szFileName, &stStat) != 0)
    {
        return FALSE;
    }

    *pulCreateTime = stStat.st_ctime;
    *pulModifyTime = stStat.st_mtime;

    return TRUE;
}

CHAR* gos_format_path(CHAR *szFile)
{
    CHAR    *szTmp;
    CHAR    *szStart;
    CHAR    acToken[32];

    // format \\ ->/
    for (szTmp = szFile; *szTmp; szTmp++)
    {
        if (*szTmp == '\\' || *szTmp == '/')
        {
            *szTmp = DIR_SEPARATOR;
        }
    }

    /* remove /./  \\.\\ */
    strcpy(acToken, "\\.\\");

    while (1)
    {
        szTmp = strstr(szFile, acToken);
        if (!szTmp)
        {
            break;
        }

        memmove(szTmp, szTmp + 2, strlen(szTmp + 2) + 1);
    }

    /* replace /../  \\..\\ */
    strcpy(acToken, "\\..\\");

    while (1)
    {
        szTmp = strstr(szFile, acToken);
        if (!szTmp)
        {
            break;
        }

        // szTmp is (/xxx)/../yyy
        for (szStart = szTmp - 1; szStart >= szFile; szStart--)
        {
            // szStart is /xxx/../yyy
            if (*szStart == DIR_SEPARATOR)
            {
                memmove(szStart, szTmp + 3, strlen(szTmp + 3) + 1);
                break;
            }
        }
    }
    return szFile;
}

UINT32 gos_get_uptime(VOID)
{
#ifdef _OSWIN32_
    static UINT32  ulStartTicks = 0;

    if (0 == ulStartTicks)
    {
        ulStartTicks = GetTickCount();
        return 0;
    }

    return (UINT32)((GetTickCount() - ulStartTicks) / 1000);
#endif
}

BOOL gos_get_proc_name(CHAR *szProcName, BOOL bResvPostfix)
{
#ifdef _OSWIN32_
    CHAR    acPath[256] = {0};
    INT32   iLen = 0;

    if (GetModuleFileName(NULL, acPath, sizeof(acPath) - 1) == 0)
    {
        return FALSE;
    }

    iLen = strlen(acPath);
    --iLen;
    for (; iLen > 0; iLen--)
    {
        if (acPath[iLen] == '\\' || acPath[iLen] == '/')
        {
            break;
        }
    }

    if (iLen <= 0)
    {
        return FALSE;
    }

    strcpy(szProcName, &acPath[iLen + 1]);
    if (!bResvPostfix)
    {
        iLen = strlen(szProcName);
        --iLen;
        for (; iLen > 0; iLen--)
        {
            if (szProcName[iLen] == '.')
            {
                szProcName[iLen] = '\0';
                break;
            }
        }
    }

    return TRUE;
#endif
}

CHAR* gos_get_root_path(VOID)
{
    static BOOL     bGetPath = FALSE;
    static CHAR     acPath[512];
    INT             iPathLen = 0;

    if (!bGetPath)
    {
        memset(acPath, 0, sizeof(acPath));

        if (GetModuleFileName(NULL, acPath, sizeof(acPath)-1) == 0)
        {
            acPath[0] = '\0';
        }

        iPathLen = strlen(acPath);
        // 去除可执行文件名后缀
        --iPathLen;
        for ( ; iPathLen>0; iPathLen--)
        {
            if (acPath[iPathLen] == '\\' ||
                acPath[iPathLen] == '/' )
            {
                acPath[iPathLen] = '\0';
                break;
            }
        }

        gos_format_path(acPath);
        bGetPath = TRUE;
    }

    return acPath;
}

static CHAR* GetLogLevel(UINT32 ulLogLevel)
{
    if (ulLogLevel == LOG_DETAIL)
    {
        return (CHAR*)"DETAIL";
    }
    else if (ulLogLevel == LOG_INFO)
    {
        return (CHAR*)"INFO";
    }
    else if (ulLogLevel == LOG_WARN)
    {
        return (CHAR*)"WARN";
    }
    else if (ulLogLevel == LOG_ERROR)
    {
        return (CHAR*)"ERROR";
    }
    else if (ulLogLevel == LOG_FAIL)
    {
        return (CHAR*)"FAIL";
    }
    else if (ulLogLevel == LOG_FATAL)
    {
        return (CHAR*)"FATAL";
    }
    else if (ulLogLevel == LOG_PROFILER)
    {
        return (CHAR*)"PROFILER";
    }

    return (CHAR*)"UNKNOWN";
}

static WORD GetLogColor(UINT32 ulLogLevel)
{
    using namespace Lee::ConsoleColor;
    switch(ulLogLevel)
    {
        case LOG_DETAIL:
            return NO_COLOR;
            break;

        case LOG_INFO:
            return WHITE;
            break;

        case LOG_WARN:
            return YELLOW;
            break;

        case LOG_ERROR:
            return RED;
            break;

        case LOG_FAIL:
            return RED;
            break;

        case LOG_FATAL:
            return RED;
            break;

        case LOG_PROFILER:
            return CYAN;
            break;

        default:
            return NO_COLOR;
            break;
    }
}

static Logger* GetDefaultLogger()
{
    static GLogger *pLogger = NULL;

    if (!pLogger)
    {
        
        pLogger = new GLogger();
    }

    return pLogger;
}

GLogFactory::GLogFactory()
{
    m_pLogger = GetDefaultLogger();
}

GLogFactory* GLogFactory::GetInstance()
{
    static GLogFactory* pLogFactory = NULL;

    if (!pLogFactory)
    {
        pLogFactory = new GLogFactory();
    }

    return pLogFactory;
}

VOID GLogFactory::SetLogger(Logger *pLogger)
{
    m_pLogger = pLogger;
}

Logger* GLogFactory::GetLogger()
{
    return m_pLogger;
}

/* GLogger */
GLogger::GLogger()
{
    m_mLogModule.clear();

    SetLogModule(MODULE_PLAT, (CHAR*)MODULE_PLAT_NAME);

    m_ulLogLevel = LOG_ERROR;
    m_bLogToStdout = FALSE;
    m_bLogToFile = TRUE;

    m_ulMaxFileNum = 2;
    m_ulMaxFileSize = 1024 * 1024;

    m_ulCurrLogFileIndex = 0xffffffff;
    m_ulCurrLogFileSize = 0;
    m_fpLog = NULL;

    m_ulLastFlushTime = 0;
    m_ulMaxBufTime = 5 * 60;

    m_acLogFilePrefix[0] = '\0';
    gos_get_proc_name(m_acLogFilePrefix, FALSE);
}

static UINT32 GetLogFileIndex(CHAR *szLogDir, CHAR *szLogFilePrefix, UINT32 ulMaxLogFileNum)
{
    UINT32      ulIndex = 0;
    UINT32      ulFisrtFileTime = 0;
    UINT32      ulFileCreateTime;
    UINT32      ulFileModifyTime;
    UINT32      i;
    CHAR        acFile[260];

    for (i = 0; i < ulMaxLogFileNum; i++)
    {
        sprintf_s(acFile, sizeof(acFile), "%s/%s_%u.log", szLogDir, szLogFilePrefix, i);

        if (!gos_file_exist(acFile))
        {
            return i;
        }

        gos_get_file_time(acFile, &ulFileCreateTime, &ulFileModifyTime);

        if (0 == i || ulFileModifyTime < ulFisrtFileTime)
        {
            ulFisrtFileTime = ulFileModifyTime;
            ulIndex = i;
        }
    }

    return ulIndex;
}

VOID GLogger::SetLogFilePrefix(CHAR *szLogFilePrefix)
{
    strncpy(m_acLogFilePrefix, szLogFilePrefix, sizeof(m_acLogFilePrefix) - 1);
    m_acLogFilePrefix[sizeof(m_acLogFilePrefix) - 1] = '\0';
}

VOID GLogger::LogToFile(CHAR *szLog, UINT32 ulLogLen)
{
    std::lock_guard<std::mutex> lock(m_pLogFileMutex);

    if (m_ulCurrLogFileSize > m_ulMaxFileSize)
    {
        fclose(m_fpLog);
        m_fpLog = NULL;
        m_ulCurrLogFileIndex = MOD(m_ulCurrLogFileIndex + 1, m_ulMaxFileNum);
        m_ulCurrLogFileSize = 0;
    }

    if (!m_fpLog)
    {
        CHAR    acCurrLogFile[512] = {0};

        sprintf_s(acCurrLogFile, sizeof(acCurrLogFile), "%s/log", gos_get_root_path());
        gos_format_path(acCurrLogFile);

        if (!gos_file_exist(acCurrLogFile))
        {
            gos_create_redir(acCurrLogFile);
        }

        if (m_ulCurrLogFileIndex == 0xffffffff)
        {
            m_ulCurrLogFileIndex = GetLogFileIndex(acCurrLogFile, m_acLogFilePrefix, m_ulMaxFileNum);
        }

        sprintf(acCurrLogFile, "%s/log/%s_%u.log", gos_get_root_path(), m_acLogFilePrefix, m_ulCurrLogFileIndex);
        gos_format_path(acCurrLogFile);

        m_fpLog = fopen(acCurrLogFile, "w+t");
        m_ulLastFlushTime = gos_get_uptime();

        if (m_fpLog)
        {
            setvbuf(m_fpLog, m_acLogBuf, _IOFBF, sizeof(m_acLogBuf));
        }
    }

    if (m_fpLog)
    {
        m_ulCurrLogFileSize += fprintf(m_fpLog, "%s\n", szLog);
#ifdef WIN32
        m_ulCurrLogFileSize++;
#endif

        UINT32  ulCurrTime = gos_get_uptime();

        if ((UINT32)(ulCurrTime - m_ulLastFlushTime) >= m_ulMaxBufTime)
        {
            fflush(m_fpLog);
            m_ulLastFlushTime = ulCurrTime;
        }
    }
}

VOID GLogger::Log(UINT32 ulLogLevel, const CHAR *szFormat, va_list vaLog)
{
    _Log(reinterpret_cast<CHAR*>(""), ulLogLevel, szFormat, vaLog);
}

VOID GLogger::Log(UINT32 ulLogLevel, const CHAR *szFormat, ...)
{
    va_list vaLog;

    if (ulLogLevel == LOG_NULL || ulLogLevel < m_ulLogLevel)
    {
        return;
    }

    va_start(vaLog, szFormat);

    _Log("", ulLogLevel, szFormat, vaLog);

    va_end(vaLog);
}

VOID GLogger::Log(UINT32 ulLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...)
{
    va_list         vaLog;
    CHAR            acLogModule[32] = {0};
    LOG_MODULE_T    *pLogModule = NULL;

    if (ulLogLevel == LOG_NULL || ulLogLevel < m_ulLogLevel)
    {
        return;
    }

    if (m_mLogModule.find(ulLogModule) == m_mLogModule.end())
    {
        pLogModule = nullptr;
    }
    else
    {
        pLogModule = m_mLogModule[ulLogModule];
    }

    if (!pLogModule)
    {
        sprintf_s(acLogModule, sizeof(acLogModule), "UNKNOWN");
    }
    else
    {
        if (pLogModule->ulLogLevel != LOG_DEFAULT &&
            ulLogLevel < pLogModule->ulLogLevel)
        {
            return;
        }

        sprintf_s(acLogModule, sizeof(acLogModule), pLogModule->acLogModule);
    }

    va_start(vaLog, szFormat);

    _Log(acLogModule, ulLogLevel, (CHAR*)szFormat, vaLog);

    va_end(vaLog);
}

VOID GLogger::Log(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, ...)
{
    va_list vaLog;

    if (ulLogLevel == LOG_NULL || ulLogLevel < m_ulLogLevel)
    {
        return;
    }

    if (IS_NULL_STRING(szLogModule))
    {
        szLogModule = (CHAR*)"";
    }

    va_start(vaLog, szFormat);

    _Log(szLogModule, ulLogLevel, szFormat, vaLog);

    va_end(vaLog);
}

VOID GLogger::SetLogModule(UINT32 ulLogModule, CHAR *szLogModule)
{
    LOG_MODULE_T    *pLogModule = NULL;

    if (m_mLogModule.find(ulLogModule) == m_mLogModule.end())
    {
        // 异常退出
        return;
    }
    else
    {
        pLogModule = m_mLogModule.find(ulLogModule)->second;
    }

    pLogModule = (LOG_MODULE_T*)calloc(1, sizeof(LOG_MODULE_T));
    if (!pLogModule)
    {
        return;
    }

    pLogModule->ulLogLevel = LOG_DEFAULT;
    strncpy(pLogModule->acLogModule, szLogModule, sizeof(pLogModule->acLogModule) - 1);

    m_mLogModule[ulLogModule] = pLogModule;
}

VOID GLogger::SetLogLevel(UINT32 ulLogLevel)
{
    if (ulLogLevel > LOG_FATAL)
    {
        return;
    }

    m_ulLogLevel = ulLogLevel;
}

VOID GLogger::SetAllModuleLogLevel(UINT32 ulLogLevel)
{
    if (ulLogLevel == LOG_DEFAULT)
    {
        ulLogLevel = m_ulLogLevel;
    }
    else
    {
        if (ulLogLevel > LOG_FATAL)
        {
            return;
        }

        m_ulLogLevel = ulLogLevel;
    }

    for (MAP<UINT32, LOG_MODULE_T*>::iterator it = m_mLogModule.begin(); it != m_mLogModule.end(); ++it)
    {
        it->second->ulLogLevel = ulLogLevel;
    }
}

VOID GLogger::SetLogLevel(UINT32 ulLogModule, UINT32 ulLogLevel)
{
    LOG_MODULE_T    *pLogModule = NULL;

    if (ulLogLevel > LOG_DEFAULT)
    {
        return;
    }

    if (m_mLogModule.find(ulLogModule) == m_mLogModule.end())
    {
        return;
    }
    else
    {
        pLogModule = m_mLogModule[ulLogModule];
    }

    if (pLogModule)
    {
        pLogModule->ulLogLevel = ulLogLevel;
    }
}

BOOL GLogger::SetLogLevel(CHAR *szLogModule, UINT32 ulLogLevel)
{
    LOG_MODULE_T        *pLogModule = NULL;
    BOOL                bRet = FALSE;

    if (ulLogLevel > LOG_DEFAULT)
    {
        return bRet;
    }

    for (MAP<UINT32, LOG_MODULE_T*>::iterator it = m_mLogModule.begin(); it != m_mLogModule.end(); ++it)
    {
        if (0 == strcmp(it->second->acLogModule, szLogModule))
        {
            it->second->ulLogLevel = ulLogLevel;
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}

VOID GLogger::SetLogToStdout(BOOL bEnable)
{
    m_bLogToStdout = bEnable;
}

/*
* @name               : SetLogToFile
* @function           : 设置这个log写入文件的配置
* @class              : GLogger
* @globalparam        : NONE
* @classparam         : m_bLogToFile、m_ulMaxFileNum、m_ulMaxFileSize、m_ulMaxBufTime
* @return             : NONE
* @author             : Lijiancong, 316, lijiancong@fritt.com.cn
* @createtime         : 2019-05-29 14:27:13
* @modifytime         : 2019-05-29 14:27:13
* @note               : 
*/
VOID GLogger::SetLogToFile(const BOOL   bEnable,                // [in]
                           const UINT32 ulMaxFileNum,     // [in]
                           const UINT32 ulMaxFileSize,    // [in]
                           const UINT32 ulMaxBufTime      // [in]
                           )
{
    if (ulMaxFileNum < 2)
    {
        m_ulMaxFileNum = 2;
    }

    if (ulMaxFileSize < 1024 * 1024)
    {
        m_ulMaxFileSize = 1024 * 1024;
    }

    m_bLogToFile = bEnable;
    m_ulMaxBufTime = ulMaxBufTime;
}

VOID GLogger::GetModuleList(VECTOR<UINT32>& vLogModule)
{
    for (MAP<UINT32, LOG_MODULE_T*>::const_iterator it = m_mLogModule.cbegin(); it != m_mLogModule.cend(); ++it)
    {
        vLogModule.push_back(it->first);
    }
}

VOID GLogger::GetModule(UINT32 ulModuleID, CHAR *szModuleName, UINT32 &ulModuleLevel)
{
    LOG_MODULE_T    *pLogModule = nullptr;

    if (m_mLogModule.find(ulModuleID) == m_mLogModule.end())
    {
        return;
    }
    else
    {
        pLogModule = m_mLogModule[ulModuleID];
    }

    if (pLogModule)
    {
        strcpy(szModuleName, pLogModule->acLogModule);
        ulModuleLevel = pLogModule->ulLogLevel;
    }
}

VOID GLogger::_Log(CHAR *szLogModule, UINT32 ulLogLevel, const CHAR *szFormat, va_list vaLog)
{
    UINT32  ulLen;
    CHAR    acTime[32];
    CHAR    acLog[1024*2] = {0};
    CHAR    acLogTemp[1024] = {0};
    CHAR    *szLogLevel = GetLogLevel(ulLogLevel);

    const time_t  ulTime = time(NULL);
    tm stTimer = {0};
    localtime_s(&stTimer, &ulTime);
    sprintf_s(acTime, sizeof(acTime), "%04d-%02d-%02d %02d:%02d:%02d",
                                      1900+stTimer.tm_year,
                                      1+stTimer.tm_mon,
                                      stTimer.tm_mday,
                                      stTimer.tm_hour,
                                      stTimer.tm_min,
                                      stTimer.tm_sec);

    if (IS_NULL_STRING(szLogModule))
    {
        ulLen = sprintf_s(acLog, sizeof(acLog), "[%s] %s >", acTime, szLogLevel);
    }
    else
    {
        ulLen = sprintf_s(acLog, sizeof(acLog), "[%s] %s : %s >", acTime, szLogModule, szLogLevel);
    }

    sprintf_s(acLogTemp, sizeof(acLogTemp), szFormat, vaLog);
    strcat_s(acLog, sizeof(acLog), acLogTemp);

    va_end(vaLog);

    /* log */
    if (m_bLogToStdout)
    {
        // printf("%s\n", acLog);
        if (IS_NULL_STRING(szLogModule))
        {
            printf("[%s] ", acTime);
        }
        else
        {
            printf("[%s] %s :", acTime, szLogModule);
        }
        Lee::ConsoleColor::SetConsoleColor(GetLogColor(ulLogLevel));
        printf("%s ",szLogLevel);
        Lee::ConsoleColor::SetConsoleColor(NO_COLOR);
        printf(">%s\n", acLogTemp);
    }

    if (m_bLogToFile)
    {
        LogToFile(acLog, ulLen);
    }
}

    }; // end of  namespace GosLog 
}; // end of namespace Lee 