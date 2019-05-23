#include "gos_get_file_size.h"
#include <sys/stat.h>

INT32 gos_get_file_size(const CHAR * const szFileName)
{
#ifdef _OSWIN32_

#ifdef _BCB_
    struct stat  stStat;
#else
    struct _stat stStat;
#endif

    if (_stat(szFileName, &stStat) != 0)
    {
        return -1;
    }

    return stStat.st_size;

#else
    INT32       fd = open(szFileName, O_RDONLY);
    struct stat sbuf;

    if (fd == -1)
    {
        return -1;
    }

    //获取文件的大小
    if (fstat(fd, &sbuf) < 0)
    {
        close(fd);
        return -1;
    }

    close(fd);
    return (INT32)sbuf.st_size;
#endif
}