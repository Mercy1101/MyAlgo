#ifndef MYALGO_SRC_GOS_ITOA_H
#define MYALGO_SRC_GOS_ITOA_H

#include "gos_typedef.h"

INT32 gos_itoa(UINT32 ulDigit, CHAR *szDigit);

const char* gos_itoa_imporve(const int value,     // [in]
                             char      buf[]      // [out]
                             );

#endif