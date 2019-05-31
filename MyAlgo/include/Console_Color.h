#ifndef MYALGO_INCLUDE_CONSOLE_COLOR_H
#define MYALGO_INCLUDE_CONSOLE_COLOR_H

#include <windows.h>
#include "gos_typedef.h"

#define CONSOLE_COLOR       // 设置是否开启颜色

namespace Lee {
    namespace ConsoleColor {
#ifdef _OSWIN32_
#ifdef CONSOLE_COLOR

#define RED         (FOREGROUND_INTENSITY | FOREGROUND_RED)                         // 红色
#define GREEN       (FOREGROUND_INTENSITY | FOREGROUND_GREEN)                       // 绿色
#define BLUE        (FOREGROUND_INTENSITY | FOREGROUND_BLUE)                        // 蓝色
#define YELLOW      (FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN)       // 黄色
#define PINK        (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE)       // 粉色
#define CYAN        (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)     // 青色
#define WHITE       (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)    // 白色
#define NO_COLOR    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)           // 原色（灰色）

#define RED1         (FOREGROUND_RED)                         // 红色
#define GREEN1       (FOREGROUND_GREEN)                       // 绿色
#define BLUE1        (FOREGROUND_BLUE)                        // 蓝色
#define YELLOW1      (FOREGROUND_RED |FOREGROUND_GREEN)       // 黄色
#define PINK1        (FOREGROUND_RED | FOREGROUND_BLUE)       // 粉色
#define CYAN1        (FOREGROUND_GREEN | FOREGROUND_BLUE)     // 青色

#endif
#endif

void SetConsoleColor(WORD Color);

    };  // end of namespace ConsoleColor 
};  // end of namespace Lee 


#endif