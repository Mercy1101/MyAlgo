echo off

echo 'start cmake in windows'

::call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

rd /s /q build
mkdir build
cd  build

::cmake .. -G "Visual Studio 16 2019" -A  Win32
::cmake .. -G "Visual Studio 14 2015"
cmake .. -G "Visual Studio 16 2019"

::MSBuild.exe  DongJinAPI.sln -p:Platform=Win32 -t:Rebuild 
::MSBuild.exe DongJinAPI.sln -p:Configuration=Release -p:Platform="Win32" -t:Rebuild
MSBuild.exe MyAlgo.sln -p:Configuration=Release  -t:Rebuild

echo 'end all'