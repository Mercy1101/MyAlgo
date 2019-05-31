#ifndef MYALGO_INCLUDE_CONSOLE_COLOR_H
#define MYALGO_INCLUDE_CONSOLE_COLOR_H

#include <windows.h>
#include "gos_typedef.h"

#define CONSOLE_COLOR       // �����Ƿ�����ɫ

namespace Lee {
    namespace ConsoleColor {
#ifdef _OSWIN32_
#ifdef CONSOLE_COLOR

#define RED         (FOREGROUND_INTENSITY | FOREGROUND_RED)                         // ��ɫ
#define GREEN       (FOREGROUND_INTENSITY | FOREGROUND_GREEN)                       // ��ɫ
#define BLUE        (FOREGROUND_INTENSITY | FOREGROUND_BLUE)                        // ��ɫ
#define YELLOW      (FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN)       // ��ɫ
#define PINK        (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE)       // ��ɫ
#define CYAN        (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)     // ��ɫ
#define WHITE       (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)    // ��ɫ
#define NO_COLOR    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)           // ԭɫ����ɫ��

#define RED1         (FOREGROUND_RED)                         // ��ɫ
#define GREEN1       (FOREGROUND_GREEN)                       // ��ɫ
#define BLUE1        (FOREGROUND_BLUE)                        // ��ɫ
#define YELLOW1      (FOREGROUND_RED |FOREGROUND_GREEN)       // ��ɫ
#define PINK1        (FOREGROUND_RED | FOREGROUND_BLUE)       // ��ɫ
#define CYAN1        (FOREGROUND_GREEN | FOREGROUND_BLUE)     // ��ɫ

#endif
#endif

void SetConsoleColor(WORD Color);

    };  // end of namespace ConsoleColor 
};  // end of namespace Lee 


#endif