#include "designpattern/Singleton.h"

#include <catch2/catch.hpp>

#include "utility/utility.h"  // for lee::ignore_unsed()

using namespace lee;
using namespace lee::DesignPattern_::Singleton_;

/**
* @name                Singleton.SimpleTest_Int

* @brief               singleton模板的简单测试，主要使用int来实例化一个单例，
                       并使用GetSingletonPtr<int>();函数来获取指向该实例的指针。

* @author              Lijiancong, pipinstall@163.com
* @date                2019-07-17 10:38:10

* @note
*/
TEST_CASE("Singleton简单测试", "[design_pattern][Singleton]") {
  auto m_pSimple = GetSingletonPtr<int>();
  /// 判断实例化的对象中数据是不是已经初始化
  // REQUIRE(m_pSimple->GetData() == 0);

  m_pSimple->SetData(5);
  REQUIRE(m_pSimple->GetData() == 5);

  /// 再次调用GetInstance()，产生的指针仍指向第一次实例化的singleton
  auto pInstance = singleton<int>::GetInstance();
  REQUIRE(pInstance->GetData() == 5);

  pInstance->SetData(6);
  REQUIRE(m_pSimple->GetData() == 6);

  /// 对已经实例化的singleton<int>复制该指针可以成功，
  /// 得到的指针仍指向仍然是已经实例化的singleton<int>
  auto pInstance_another = GetSingletonPtr<int>();
  REQUIRE(pInstance_another->GetData() == 6);
}

/**
* @name                Singleton.ErrorTest

* @brief               这下面写了很多不能编译通过的例子，
                       也是使用单例来避免的一些情况。
                       比如拷贝该实例，继承等不应出现的例子。

* @author              Lijiancong, pipinstall@163.com
* @date                2019-07-17 10:38:10

* @note
*/
TEST_CASE("Singleton无法通过编译的展示", "[design_pattern][Singleton]") {
  /// 构造函数被隐藏为private，所以不能显式定义， 编译不过
  /// E0330	"singleton<int>::singleton<int>()" (已声明 所在行数 : 30)
  /// 不可访问 auto error0 = new singleton<int>; auto error1 =
  /// std::make_shared<singleton<int>>();

  /// 复制构造函数已删除，编译不过
  ///（编译器错误：E1776	无法引用 函数
  ///"singleton<int>::singleton<int>(const singleton<int> &)" (已隐式声明)
  /// --它是已删除的函数）
  /// auto error2(*m_pSimple);

  /// 移动构造函数已删除，编译不过
  ///（编译器错误：E1776	无法引用 函数
  ///"singleton<int>::singleton<int>(const singleton<int> &)" (已隐式声明)
  /// --它是已删除的函数）
  /// auto error3(std::move(*m_pSimple));

  /// 复制构造符已删除，编译不过
  /// （编译器错误：E1776	无法引用 函数
  /// "singleton<int>::singleton<int>(const singleton<int> &)" (已隐式声明)
  /// --它是已删除的函数） auto error4 = *m_pSimple;

  /// 移动构造符已删除，编译不过
  ///（编译器错误：E1776	无法引用 函数
  ///"singleton<int>::singleton<int>(const singleton<int> &)" (已隐式声明)
  /// --它是已删除的函数）
  /// auto error5 = std::move(*m_pSimple);

  /// class ERROR_singleton : public singleton<int> {};
  /// 单例模式的class时不能被继承使用
  /// 编译器错误:E1790	无法引用 "ERROR_singleton" 的默认构造函数 --
  /// 它是已删除的函数 ERROR_singleton error6; auto error7 = new
  /// ERROR_singleton;
}

/**
* @name                Singleton.TemplateTest

* @brief                写了关于double类型来实例化singleton的例子，
                        实验证明double类型也可以正常实现单例。

* @author              Lijiancong, pipinstall@163.com
* @date                2019-07-17 10:40:25

* @note
*/
TEST_CASE("Singleton double类型下的测试", "[design_pattern][Singleton]") {
  auto pInstance_double = GetSingletonPtr<double>();

  REQUIRE(pInstance_double->SetData(0.00000000001) == Approx(0.00000000001));

  for (int i = 0; i < 100; i++) {
    REQUIRE(pInstance_double == GetSingletonPtr<double>());
    REQUIRE(Approx(pInstance_double->GetData()) ==
            GetSingletonPtr<double>()->GetData());

    /// 下面这句话本来是想以指针地址来做比较，但EXPECT_NE这个宏不允许类型转换。
    /// EXPECT_NE(pInstance_double, GetSingletonPtr<int>());
  }

  REQUIRE(GetSingletonPtr<int>()->GetData() == 6);
}

struct DialCfg {
  bool bIsAutoReboot;           //是否自动重启
  unsigned ulAutoRebootHour;    //自动重启开始时点，缺省1
  unsigned ulAutoRebootMinute;  //自动重启开始分钟，缺省0
  unsigned ulNeedAutoAudit;     // 临时拨测是否需要进行自动核查
};

/**
* @name                Singleton.StructTest

* @brief               演示了使用结构体来实例化单例，并能实现单例的功能。

* @author              Lijiancong, pipinstall@163.com
* @date                2019-07-17 10:41:43

* @note
*/
TEST_CASE("结构体测试", "[design_pattern][Singleton]") {
  auto pInstance = GetSingletonPtr<DialCfg>();
  lee::ignore_unused(pInstance);

  DialCfg stTemp = {0};
  stTemp.bIsAutoReboot = true;
  stTemp.ulAutoRebootHour = 2;
  stTemp.ulAutoRebootMinute = 59;
  stTemp.ulNeedAutoAudit = 7;
  GetSingletonPtr<DialCfg>()->SetData(stTemp);
  REQUIRE(GetSingletonPtr<DialCfg>()->GetData().bIsAutoReboot ==
          stTemp.bIsAutoReboot);
  REQUIRE(GetSingletonPtr<DialCfg>()->GetData().ulAutoRebootHour ==
          stTemp.ulAutoRebootHour);
  REQUIRE(GetSingletonPtr<DialCfg>()->GetData().ulAutoRebootMinute ==
          stTemp.ulAutoRebootMinute);
  REQUIRE(GetSingletonPtr<DialCfg>()->GetData().ulNeedAutoAudit ==
          stTemp.ulNeedAutoAudit);

  REQUIRE(GetSingletonPtr<int>()->GetData() == 6);
  REQUIRE(GetSingletonPtr<double>()->SetData(0.00000000001) ==
          Approx(0.00000000001));
}
