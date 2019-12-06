#include <utility/utility.h>
#include <catch2/catch.hpp>

TEST_CASE("time相关的函数简单测试，只是显示没有判断", "[utility][time]") {
  dbg(Lee::GetTimeString());
  dbg(Lee::GetTimeString(Lee::GetCurrentTimeStamp()));
  dbg(Lee::GetTimeString(Lee::GetCurrentTimeStamp(), "%Y%m%d%H%M%S"));
  dbg(Lee::GetTimeString(Lee::GetCurrentMilliSecondStamp()));
  dbg(Lee::GetTimeString(Lee::GetCurrentMilliSecondStamp(), "%Y%m%d%H%M%S"));
  dbg(Lee::GetTimeString(1571290440));
  dbg(Lee::GetTimeString(1571290440, "%Y%m%d%H%M%S"));
  dbg(Lee::GetTimeString("%Y%m%d%H%M%S"));

  dbg(Lee::GetCompileTimeStamp());
  dbg(Lee::GetTimeString(Lee::GetCompileTimeStamp()));
}