::获取最近一次的label
for /F %%i in ('git describe --tags') do ( set tag=%%i)

::tag自增
set /a nexttag = %tag:~0,3%+1

::git创建tag
git tag %nexttag%

::改写配置文件中的tag
set marco=#define CURRENT_TAGS_NUMBER 
echo,%marco%%nexttag%> VersionConfig.h

::pause