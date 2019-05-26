#ifndef MYALGO_INCLUDE_GOS_TYPEDEF_H
#define MYALGO_INCLUDE_GOS_TYPEDEF_H

#include <stdio.h>
#include "profiler.h"

#define _OSWIN32_       // windows下32bit的应用程序定义该宏

#define BOOL    int
#define TRUE    1
#define FALSE   0

#define UINT64          unsigned long long
#define INT64           long long

#define UINT32  unsigned int
#define INT32   int

#define UINT16  short
#define UINT8   unsigned char

#define CHAR    char
#define VOID    void

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)   sizeof(x)/sizeof((x)[0])
#endif

#define MOD(a, b)       ((a) % (b))

#define GET_INT(ptr)            (*((UINT32*)(ptr)))
#define SET_INT(ptr, value)     (*((UINT32*)(ptr))) = (UINT32)(value)

#define GET_INT64(ptr)          (*((INT64*)(ptr)))
#define SET_INT64(ptr, value)   (*((INT64*)(ptr))) = (INT64)(value)

#define GET_SHORT(ptr)          (*((UINT16*)(ptr)))
#define SET_SHORT(ptr, value)   (*((UINT16*)(ptr))) = (UINT16)(value)

#endif