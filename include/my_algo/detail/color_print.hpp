/**
 * @file            color_print.hpp
 * @brief           颜色输出print的实现
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-06-03 13:23:34
 *
 * @copyright       Copyright (c) 2021 GBCOM
 *
 */

#ifndef DETAIL_LEE_UTILITY_COLOR_PRINT_HPP_
#define DETAIL_LEE_UTILITY_COLOR_PRINT_HPP_

#include <iostream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _BCB_
#define USE_ORIGIN_LOG_COUT  ///< 定义该宏，使用原命令行的输出
#else
#undef USE_ORIGIN_LOG_COUT  ///< 不定义该宏，使用颜色输出
#endif

namespace lee
{
enum color
{
    ORIGIN,
    WHITE,
    BLUE,
    GREEN,
    YELLOW,
    RED,
};

#ifdef _WIN32
enum window_color
{
    ORIGIN_COLOR = FOREGROUND_INTENSITY,                                                       ///< 原色
    WHITE_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,  ///< 白色
    BLUE_COLOR = FOREGROUND_INTENSITY | FOREGROUND_BLUE,                                       ///< 蓝色
    GREEN_COLOR = FOREGROUND_INTENSITY | FOREGROUND_GREEN,                                     ///< 绿色
    YELLOW_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,                   ///< 黄色
    RED_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED,                                         ///< 红色
};
inline WORD GetWindowColor(const color& color)
{
    switch (static_cast<int>(color))
    {
        case static_cast<int>(ORIGIN):
            return static_cast<WORD>(ORIGIN_COLOR);
        case static_cast<int>(WHITE):
            return static_cast<WORD>(WHITE_COLOR);
        case static_cast<int>(BLUE):
            return static_cast<WORD>(BLUE_COLOR);
        case static_cast<int>(GREEN):
            return static_cast<WORD>(GREEN_COLOR);
        case static_cast<int>(YELLOW):
            return static_cast<WORD>(YELLOW_COLOR);
        case static_cast<int>(RED):
            return static_cast<WORD>(RED_COLOR);
        default:
            return static_cast<WORD>(ORIGIN_COLOR);
    }
}
#else
inline const char* GetLinuxColor(const ::lee::color& color)
{
    switch (static_cast<int>(color))
    {
        case static_cast<int>(ORIGIN):
            return "\e[0m";
        case static_cast<int>(WHITE):
            return "\e[1;37m";
        case static_cast<int>(BLUE):
            return "\e[1;34m";
        case static_cast<int>(GREEN):
            return "\e[1;32m";
        case static_cast<int>(YELLOW):
            return "\e[1;33m";
        case static_cast<int>(RED):
            return "\e[1;31m";
        default:
            return "\e[0m";
    }
}
#endif

/// 颜色输出函数
inline void printf_color(const color& Color, const std::string& str)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetWindowColor(Color));
    std::cout << str;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetWindowColor(WHITE));
#else
    std::cout << GetLinuxColor(Color) << str << GetLinuxColor(ORIGIN);
#endif
}

}  // namespace lee

#endif  ///< DETAIL_LEE_UTILITY_COLOR_PRINT_HPP_