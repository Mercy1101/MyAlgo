/** 
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * 
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

/**
* @file                 Singleton.h
* @brief                单例模式对象的实现，被设计用于我们全局变量的管理。

* @details              实现上使用了以下知识：
                        1. static变量的初始化，
                        2. 类的五个构造函数的自动生成与如何屏蔽自动生成
                        3. 单例设计模式
                        4. 使用call_once函数来实现支持多线程实例化

* @author               Lijiancong, pipinstall@163.com
* @date                 2019-08-07 15:22:34


* @note
1. 设计思路：
    以单例模式为核心，支持多线程和模板。一个模板类型只会被实现一次。

2. 扩展阅读：
    1> 这篇文章以打印机驱动为例子，简单明了的介绍了最简单的单例也是最原始的单例模式是怎么实现的。
       Scott Meyers 《More Effective C++》条款26：限制某个class所能产生的对象数量(Limiting the number of objects of a class)

    2> 这篇文章同样是Scott Meyers写的关于多线程实例的实现，本文件中的实现大部分摘自该文章。
       Scott Meyers and Andrei Alexanders, September 2004 《C++ and the Perils of Double-Checked Locking》
       文章下载地址：https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf

    3> 这篇博客介绍了最简单的单例模式的实现，不了解单例模式推荐阅读这篇文章。
       《单例模式(Singleton)及其C++实现》https://blog.csdn.net/hit0803107/article/details/54411180

    4> 这篇博客介绍了使用继承来生成多个不同的单例对象，推荐阅读。
      《设计模式C++实现（4）——单例模式》https://blog.csdn.net/wuzhekai1985/article/details/6665869

    5> 这篇博客介绍了使用虚继承和友元来定义可被继承和不可被继承的单例对象。总结的很详细，但我（李建聪）没看懂。
      《《剑指offer》:[48]不能被继承的类-单例模式》https://blog.csdn.net/gogokongyin/article/details/51757835

    6> 国外最权威的软件问答社区，关于单例模式的实现排名第一的问题。讨论区中提供了一个线程安全版的最简单的单例模式实现。
       https://stackoverflow.com/questions/1008019/c-singleton-design-pattern?r=SearchResults
    
    7> 这篇博客介绍了单例类模板怎么被其他类调用(使用友元)
       https://www.cnblogs.com/lifexy/p/8810877.html

    8> 这篇博客很好的解释了DCLP的原理，然后用了现代C++的知识(内存栅栏)实现了线程安全的单例
       https://www.jianshu.com/p/69eef7651667

* @example

使用下面函数就可以得到一个指向该实例的指针
auto g_pDialCfg = Lee::GetSingletonPtr<typename>();

g_pDialCfg->GetData();  /// 可以获取该单例里存储的数据
g_pDialCfg->SetData();  /// 可以改写该单例里存储的数据

*/

#ifndef TOOLS_DESIGN_PATTERN_SINGLETON_H
#define TOOLS_DESIGN_PATTERN_SINGLETON_H

#include <thread>
#include <mutex>
#include <string>         // for memset_s()
#include "utility/utility.h"    // for Lee::non_transferable

namespace Lee { namespace DesignPattern_ { namespace Singleton_{

    template <typename T>
    class singleton : public Lee::non_transferable
    {
    public:
        /**
        * @name                GetData
        * @brief               获取该对象存储的数据
        * @param               NONE
        * @return              存储数据的常量引用。
        * @author              Lijiancong, pipinstall@163.com
        * @date                2019-08-07 15:31:27

        * @note
        */
        const T GetData()
        {
            // std::shared_lock<std::shared_mutex> lock(m_ReadWriteMutex);
            std::lock_guard<std::mutex> Lock(m_ReadWriteMutex);
            return m_data;
        }

        /**
        * @name                SetData
        * @brief               改变该对象存储数据的值
        * 
        * @param               Data [in]    要改变为什么值
        * 
        * @return              改变后的类内数据的常量引用
        * 
        * @author              Lijiancong, pipinstall@163.com
        * @date                2019-08-07 15:31:27

        * @note
        */
        const T SetData(const T& Data)
        {
            // std::unique_lock<std::shared_mutex> lock(m_ReadWriteMutex);
            std::lock_guard<std::mutex> Lock(m_ReadWriteMutex);
            m_data = Data;
            return m_data;
        }
        
    private:
        /// 该对象存储的数据
        T m_data;

        /**
            定义该类的构造函数，但不实现。
            以防止其他类继承这个类导致实例化多次的情况
         */
        explicit singleton<T>() { memset(&m_data, 0, sizeof(m_data)); }

        /// 读写锁用于保存数据的时候独占，读取数据的时候可以多次读取。
        // std::shared_mutex m_ReadWriteMutex;
        std::mutex m_ReadWriteMutex;

    public:
        /**
        * @name                GetInstance
        * @brief               单例对象的实例化函数，class singleton的核心技术部分。
        * @param               NONE
        * @return              返回一个实例化好的对象
        * @author              Lijiancong, pipinstall@163.com
        * @date                2019-08-07 15:31:27

        * @note
        */
        static singleton<T>* GetInstance()
        {
            static std::once_flag InstanceFlag;
            static singleton<T>* m_pInstance;
            std::call_once(InstanceFlag, [&]() { m_pInstance = new singleton<T>; });

            /** 返回实例化后的指向singleton_call_once对象的指针 */
            return m_pInstance;
        }
    };
} // end of Singleton_
} // end of namespace DesignPattern_

/**
* @name                GetSingletonCallOncePtr
* @brief               返回指向某个特例化单例模板的指针
* @param               NONE
* @return
* @author              Lijiancong, pipinstall@163.com
* @date                2019-08-07 15:31:27

* @note
*/
template <typename T>
Lee::DesignPattern_::Singleton_::singleton<T>* const GetSingletonPtr()
{
    return Lee::DesignPattern_::Singleton_::singleton<T>::GetInstance();
}

} // end of namespace Lee

#endif // end of TOOLS_DESIGN_PATTERN_SINGLETON_H

