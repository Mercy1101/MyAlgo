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

;(4)
CapsLock & j::
SetCapsLockState, AlwaysOff
Send, {Down}
return

;(5)
CapsLock & k::
SetCapsLockState, AlwaysOff
Send, {Up}
return

;(6)
CapsLock & l::
SetCapsLockState, AlwaysOff
Send, {Right}
return

;(7)
CapsLock & h::
SetCapsLockState, AlwaysOff
Send, {Left}
return

;(8)
CapsLock & o::
SetCapsLockState, AlwaysOff
Send, {End}{Enter}
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
Send,^0
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

; 输出/// @note
CapsLock & n::
SetCapsLockState, AlwaysOff
Send,^0
SendInput,/// @note    {space}
return

; 输出///   @retval
CapsLock & r::
SetCapsLockState, AlwaysOff
Send,^0
SendInput,///   @retval{space}
SendInput,{enter}///
Send,{up}{right}{right}{right}{right}{right}{right}{right}{right}{right}{right}{right}
return

; 输出头文件注释
CapsLock & p::
SetCapsLockState, AlwaysOff
Send,^0
;SendInput,/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
;SendInput,{ENTER}/// 
;SendInput,{ENTER}/// Use of this source code is governed by a MIT license
;SendInput,{ENTER}/// that can be found in the License file.
SendInput,///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
SendInput,{ENTER}/// Copyright (c) 2019, FRITT Inc. All rights reserved.
;SendInput,{ENTER}///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
SendInput,{ENTER}/// 
SendInput,{ENTER}/// {@}file{space}{space}{space}FILENAME.h
SendInput,{ENTER}/// {@}brief{space}{space}        
SendInput,{ENTER}/// 
SendInput,{ENTER}/// {@}author{space}lijiancong, pipinstall@163.com
SendInput,{ENTER}/// {@}date{space}{space}{space}            
d = %A_YYYY%-%A_MM%-%A_DD% %A_Hour%:%A_Min%:%A_Sec%
clipboard = %d%
Send ^v
SendInput,{ENTER}
SendInput,///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////{ENTER}
SendInput,{ENTER}
SendInput,{#}ifndef <PROJECT>_<PATH>_<FILE>_H_{ENTER}
SendInput,{#}define <PROJECT>_<PATH>_<FILE>_H_{ENTER}
;SendInput,{#}pragma once{ENTER}
SendInput,{ENTER}
SendInput,{#}endif  // end of <PROJECT>_<PATH>_<FILE>_H_{ENTER}
return

; 输出/// @details  
CapsLock & a::
SetCapsLockState, AlwaysOff
Send,^0
;SendInput,///{enter}
SendInput,/// @details{space}{space}
SendInput,{enter}///
Send,{up}{right}{right}{right}{right}{right}{right}{right}{right}{right}{right}{right}
return

;(11)
!f::
SetCapsLockState, AlwaysOff
Run, F:\PDF
return

;(12)
CapsLock & c::
SetCapsLockState, AlwaysOff
Send,^0
Send, /**{space}{space}*/
Send, {Left}{Left}{Left}
return

;(13)
!1::
Run, D:\github\MyAlgo
return


;(17)
!b::
Run, C:\boost_1_70_0\index.html
return

;(18)
CapsLock & d::
SetCapsLockState, AlwaysOff
Send,^0
Send, /// TODO(lijiancong){space}
return