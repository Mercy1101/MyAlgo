;(1)
!g::
Run, D:\github
return

;(2)
!w::
Run, D:\1WorkStation
return

;(3)
!c::
Run, https://zh.cppreference.com/
return

;(9)
CapsLock & t::
d = %A_YYYY%-%A_MM%-%A_DD% %A_Hour%:%A_Min%:%A_Sec%
;获得系统时间比如今天的时间：2011-09-16。如果需要“年”的话请替换上面的“-”。
clipboard = %d%
;把 d 的值发送到剪贴板，变量是不用声明的，想引用变量的值，就在变量的前后加“%”。第二行的变量是 AHK 自带的变量。
Send ^v
return

; 输出函数头注释
;(10)
CapsLock & q::
d = %A_YYYY%-%A_MM%-%A_DD% %A_Hour%:%A_Min%:%A_Sec%
clipboard = %d%
SetCapsLockState, AlwaysOff
SendInput,/// @name     {Enter}
SendInput,/// @brief    {Enter}
SendInput,///{Enter}
SendInput,/// @param    {Enter}
SendInput,///{Enter}
SendInput,/// @return   {Enter}
SendInput,///{Enter}
SendInput,/// @author   Lijiancong, pipinstall@163.com{Enter}
SendInput,/// @date    {space} 
Send ^v
SendInput,{Enter}/// @warning  线程不安全
return