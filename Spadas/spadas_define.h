
#ifndef SPADAS_DEFINE_H
#define SPADAS_DEFINE_H

// Update log / 更新记录: 修正部分时区夏令时问题

// Version definition / 版本定义
#define SPADAS_VERSION_MAJOR 9
#define SPADAS_VERSION_MINOR 0
#define SPADAS_VERSION_BUILD 62

/*! \mainpage
* \~English Spadas is a "write once and compile everywhere" C++ multifunctional class library that supports Windows, Linux and other operating systems. \n
* \~Chinese Spadas是支持Windows、Linux等操作系统的“一次编写到处编译”C++多功能类库。 \n
* \~English This document corresponds to Spadas version: 9.0.62 \n
* \~Chinese 本文档对应Spadas版本：9.0.62 \n
* \~English The source code repository is: https://gitee.com/ken777/Spadas \n
* \~Chinese 源码仓库位于： https://gitee.com/ken777/Spadas \n
*
* \n
* \~English \section top1 Overview of basic functions
* \~Chinese \section top1 基本功能概述
* \~English \subsection f1 1. Reference counting
* \~Chinese \subsection f1 1. 引用计数
* \~English To implement a reference counting class, you need to declare a derived class of spadas::Vars to define private member variables, and all variables are defined in the public domain. \n
* \~Chinese 实现一个引用计数类需要先声明一个 spadas::Vars 的子类用于定义私有成员变量，所有变量都定义在public域内。 \n
* \~English Then declare a derived class of spadas::Object that uses the derived class of Vars as the template type to define the methods. \n
* \~Chinese 然后声明一个以该Vars子类作为模板类型的 spadas::Object 子类，用于定义功能函数。 \n
* \~English This derived class of Object is a reference counting class, and the copy constructor and destructor of this object are all multithread safe operations. \n
* \~Chinese 该Object子类即可实现引用计数，且该对象的拷贝构造函数、析构函数都为多线程安全操作。 \n
* \~English If you need multithread safety for redirection operations (including equal operator, spadas::Object::setVars ), you should use spadas::Safe. \n
* \~Chinese 如需要重定向操作的多线程安全(包括等于号赋值操作， spadas::Object::setVars )，应使用 spadas::Safe 。 \n
* \~English Inside the methods, private member variables can be accessed through "vars->". \n
* \~Chinese 功能函数内部可通过vars->的方式访问私有成员变量。 \n
*
* \~English \subsection f2 2. Object container
* \~Chinese \subsection f2 2. 对象容器
* \~English Object containers include the following types:
* \~Chinese 对象容器包括以下类型：
*   \~English - spadas::Interface : Interface container
*   \~Chinese - spadas::Interface : 接口容器
*   \~English - spadas::Optional : Nullable object container, which can add validity judgment for any data type
*   \~Chinese - spadas::Optional : 可空对象容器，可为任意数据类型增加有效性判断
*
* \~English \subsection f3 3. Basic data structure
* \~Chinese \subsection f3 3. 基本数据结构
* \~English Basic data structures include the following types:
* \~Chinese 基本数据结构包括以下类型：
*   \~English - spadas::Array : Array
*   \~Chinese - spadas::Array : 数组
*   \~English - spadas::ArrayX : Scalable array
*   \~Chinese - spadas::ArrayX : 可扩展数组
*   \~English - spadas::List : Linked list
*   \~Chinese - spadas::List : 链表
*   \~English - spadas::Stream : Queue
*   \~Chinese - spadas::Stream : 队列
*   \~English - spadas::Map : Mapping table
*   \~Chinese - spadas::Map : 映射表
*   \~English - spadas::Dictionary : Dictionary (indexed by string)
*   \~Chinese - spadas::Dictionary : 字典(通过字符串索引)
*   \~English - spadas::Binary : Block of binary data
*   \~Chinese - spadas::Binary : 二进制数据块
*   \~English - spadas::String : String
*   \~Chinese - spadas::String : 字符串
*   \~ - spadas::XML : XML
*   \~English - spadas::IntMat spadas::FloatMat spadas::DoubleMat : Matrix
*   \~Chinese - spadas::IntMat spadas::FloatMat spadas::DoubleMat : 矩阵
*
* \~English \subsection f4 4. Console
* \~Chinese \subsection f4 4. 控制台
* \~English Console functions include text printing, text and key acquisition, pop-up information window, etc. For details, please refer to the namespace spadas::console \n
* \~Chinese 控制台功能包括文字打印、文字和按键获取、弹出信息窗口等。具体请参考命名空间 spadas::console \n
*
* \~English \subsection f5 5. File path and file IO
* \~Chinese \subsection f5 5. 文件路径与文件IO
* \~English For details, please refer to spadas::Path and spadas::File \n
* \~Chinese 具体请参考 spadas::Path 和 spadas::File \n
*
* \~English \subsection f6 6. Image
* \~Chinese \subsection f6 6. 图像
* \~English You can use spadas::ImagePointer while only considering image data storage, or spadas::Image if more complex functions needed \n
* \~Chinese 仅考虑图像数据存储可使用 spadas::ImagePointer ；需要更复杂功能可使用 spadas::Image \n
*
* \~English \subsection f7 7. Mathematical functions
* \~Chinese \subsection f7 7. 数学函数
* \~English For details, please refer to the namespace spadas::math \n
* \~Chinese 具体请参考命名空间 spadas::math \n
*
* \~English \subsection f8 8. Multithreading
* \~Chinese \subsection f8 8. 多线程
* \~English You can realize the following two types of multithread scenarios: \n
* \~Chinese 多线程可实现以下两类场景： \n
* \~English - For starting multiple sub-threads or using loop-based sub-thread, you can implement spadas::IWorkflow, start with spadas::Threads::start, and call spadas::Threads::stop when it needs to end
* \~Chinese - 启动多个子线程或以循环为主的场景，可实现 spadas::IWorkflow ，通过 spadas::Threads::start 开始，需要结束时调用 spadas::Threads::stop
* \~English - For using a single sub-thread to execute a task, you can implement spadas::ITask, start with spadas::TaskManager::start, and call spadas::TaskManager::stop when it needs to be interrupted
* \~Chinese - 启动单个子线程执行任务的场景，可实现 spadas::ITask ，通过 spadas::TaskManager::start 开始，需要中断时调用 spadas::TaskManager::stop
* \~English In multithread programming, it is often necessary to use spadas::Lock or spadas::SpinLocked to ensure the safety of inter-thread operations and data access. \n
* \~Chinese 多线程中需要经常使用 spadas::Lock 或 spadas::SpinLocked 确保线程间操作和数据访问的安全。 \n
* \~English In addition, inter-thread synchronization can be achieved through spadas::Flag or spadas::Barrier.
* \~Chinese 另外，线程间同步可通过 spadas::Flag 或 spadas::Barrier 实现。
*
* \~English \subsection f9 9. Other functions
* \~Chinese \subsection f9 9. 其他功能
* \~English Spadas also provides the following features:
* \~Chinese Spadas还提供以下功能：
* \~English - spadas::Time spadas::TimeWithMS : Date and time, available through spadas::utility::getTime
* \~Chinese - spadas::Time spadas::TimeWithMS : 日期与时间，可通过 spadas::utility::getTime 获取
* \~English - spadas::Timer : Timer
* \~Chinese - spadas::Timer : 计时器
* \~English - spadas::Tick : Oscillator
* \~Chinese - spadas::Tick : 振荡器
* \~English - spadas::MemoryMapStream : Memory-map-based data stream
* \~Chinese - spadas::MemoryMapStream : 基于内存映射的数据流
* \~English - spadas::LibraryLoader : Dynamically load library files
* \~Chinese - spadas::LibraryLoader : 动态加载库文件
* \~English Also see functions under the spadas::system and spadas::utility namespaces.
* \~Chinese 以及 spadas::system 和 spadas::utility 命名空间下的功能。
*
* \~English \subsection f10 10. Annotation tag
* \~Chinese \subsection f10 10. 注释标签
* \~English There may be the following types of tags before the comments of classes and functions, which are represented by square brackets:
* \~Chinese 在类和函数等的注释前可能有以下几类标签，以方括号表示：
* \~English - Unsafe: Involving pointers or references. Improper operation may cause the program to crash
* \~Chinese - 非安全: 涉及到指针或引用等，如操作不当可能导致程序崩溃
* \~English - Multithread safe: Besides redirection, all methods of this object are multithread safe. In case of redirection, spadas::Safe is still required. Also spadas::Vars::isSpinLockManaged for this class should return TRUE
* \~Chinese - 多线程安全: 在不进行重定向操作的前提下，该对象的所有方法都为多线程安全操作。如有重定向，仍需使用 spadas::Safe 。另外该类的 spadas::Vars::isSpinLockManaged 应返回TRUE
* \~English - Optional: methods that are not required to be implemented in the interface
* \~Chinese - 可选: 接口中非必须实现的方法
*
* \n
* \~English \section top2 Overview of the plugin framework
* \~Chinese \section top2 插件框架概述
* \~English \subsection p1 1. Implementation of plugin
* \~Chinese \subsection p1 1. 插件实现方式
*   \~English - The file name of the plugin library needs to start with the specified keyword, and provide one or more global functions in the library to return the corresponding interface implementation
*   \~Chinese - 插件库的文件名需要以指定关键字开头，并在库内提供一个或多个全局函数，返回对应的接口实现
*   \~English - Defining these global functions requires the prefix SPADAS_DEFAULT_API
*   \~Chinese - 定义这些全局函数需要前缀SPADAS_DEFAULT_API
*
* \~English \subsection p2 2. Types of plugin interfaces
* \~Chinese \subsection p2 2. 插件接口种类
*   \~English - General function plugin interface: IPluginVxxx, corresponding global function definition: GetPluginVxxx (where xxx indicates the interface version, such as 103, 201, etc., the same below)
*   \~Chinese - 通用功能插件接口: IPluginVxxx ，对应的全局函数定义: GetPluginVxxx（其中xxx表示接口版本，如103、201等等，下同）
*   \~English - Native data processing plugin interface: spadas::IProcessorPluginVxxx, corresponding global function definition: GetProcessorPluginVxxx
*   \~Chinese - 原生数据处理插件接口: IProcessorPluginVxxx ，对应的全局函数定义: GetProcessorPluginVxxx
*   \~English - General device plugin interface: spadas::IDevicePluginVxxx, corresponding global function definition: GetDevicePluginVxxx
*   \~Chinese - 一般设备插件接口: IDevicePluginVxxx ，对应的全局函数定义: GetDevicePluginVxxx
*   \~English - Bus device plugin interface: IBusPluginVxxx , corresponding global function definition: GetBusPluginVxxx
*   \~Chinese - 总线设备插件接口: IBusPluginVxxx ，对应的全局函数定义: GetBusPluginVxxx
*   \~English - Video device plugin interface: IVideoPluginVxxx , corresponding global function definition: GetVideoPluginVxxx
*   \~Chinese - 视频设备插件接口: IVideoPluginVxxx ，对应的全局函数定义: GetVideoPluginVxxx
*   \~English - File read and write (R/W) plugin interface: IFilePluginVxxx, corresponding global function definition: GetFilePluginVxxx
*   \~Chinese - 文件读写插件接口: IFilePluginVxxx ，对应的全局函数定义: GetFilePluginVxxx
*
* \~English \subsection p3 3. Types of plugin libraries
* \~Chinese \subsection p3 3. 插件库种类
*   \~English - General function library: keyword "native_" or "libnative_", supports general function plugin interface
*   \~Chinese - 通用功能库: 关键字"native_"或"libnative_"，支持通用功能插件接口
*   \~English - Native data processing library: keyword "proc_" or "libproc_", supports general function and native data processing plugin interface
*   \~Chinese - 原生数据处理库: 关键字"proc_"或"libproc_"，支持通用功能插件接口、原生数据处理插件接口
*   \~English - General device library: keyword "dev_" or "libdev_", supports general function  and general device plugin interface
*   \~Chinese - 一般设备库: 关键字"dev_"或"libdev_"，支持通用功能插件接口、一般设备插件接口
*   \~English - Bus device library: keyword "bus_" or "libbus_", supports general function and bus device plugin interface
*   \~Chinese - 总线设备库: 关键字"bus_"或"libbus_"，支持通用功能插件接口、总线设备插件接口
*   \~English - Video device library: keyword "video_" or "libvideo_", supports general function and video device plugin interface
*   \~Chinese - 视频设备库: 关键字"video_"或"libvideo_"，支持通用功能插件接口、视频设备插件接口
*   \~English - File R/W library: keyword "file_" or "libfile_", supports general function and file R/W plugin interface
*   \~Chinese - 文件读写库: 关键字"file_"或"libfile_"，支持通用功能插件接口、文件读写插件接口
*   \~English - Mixed device library: keyword "mix_" or "libmix_", supports general function , general device, bus device and video device plugin interface
*   \~Chinese - 混合设备库: 关键字"mix_"或"libmix_"，支持通用功能插件接口、一般设备插件接口、总线设备插件接口、视频设备插件接口
*
* \~English \subsection p4 4. General function plugin interface
* \~Chinese \subsection p4 4. 实现通用功能插件接口
* \~English Methods that must be implemented in spadas::IPluginV200 include:
* \~Chinese spadas::IPluginV200 中必须实现的方法包括：
*   \~ - getPluginType
*   \~ - getPluginVersion
*
* \~English \subsection p5 5. Native data processing plugin interface
* \~Chinese \subsection p5 5. 实现原生数据处理插件接口
* \~English Methods that must be implemented in spadas::IProcessorPluginV700 include:
* \~Chinese spadas::IProcessorPluginV700 中必须实现的方法包括：
*   \~ - setProcessorConfig
*   \~ - disableProcessor
*   \~ - processData
*
* \~English \subsection p6 6. General device plugin interface
* \~Chinese \subsection p6 6. 实现一般设备插件接口
* \~English Methods that must be implemented in spadas::IDevicePluginV300 include:
* \~Chinese spadas::IDevicePluginV300 中必须实现的方法包括：
*   \~ - setDeviceConnection
*   \~ - disconnectDevice
*   \~ - getDeviceStatus
*
* \~English \subsection p7 7. Bus device plugin interface
* \~Chinese \subsection p7 7. 实现总线设备插件接口
* \~English Methods that must be implemented in spadas::IBusPluginV300 include:
* \~Chinese spadas::IBusPluginV300 中必须实现的方法包括：
*   \~ - getBusDeviceList
*   \~ - openBusDevice
*   \~ - closeBusDevice
*
* \~English \subsection p8 8. Video device plugin interface
* \~Chinese \subsection p8 8. 实现视频设备插件接口
* \~English Methods that must be implemented in spadas::IVideoPluginV500 include:
* \~Chinese spadas::IVideoPluginV500 中必须实现的方法包括：
*   \~ - getVideoDeviceList
*   \~ - openVideoDevice
*   \~ - closeVideoDevice
*
* \~English \subsection p9 9. File R/W plugin interface
* \~Chinese \subsection p9 9. 实现文件读写插件接口
* \~English spadas::IFilePluginV200 can realize file reading, file writing, and data picking. \n\n
* \~Chinese spadas::IFilePluginV200 可实现文件读取、文件写入、以及文件截取。 \n\n
*
* \~English The methods that need to be implemented for file reading include:
* \~Chinese 文件读取需要实现的方法包括：
*   \~ - getFilesDuration
*   \~ - openReadFiles
*   \~ - readFilesData
*   \~ - closeReadFiles
*
* \~English The methods that need to be implemented for file writing include:
* \~Chinese 文件写入需要实现的方法包括：
*   \~ - openWriteFiles
*   \~ - writeFilesData
*   \~ - closeWriteFiles
*
* \~English The methods that need to be implemented for data picking include:
* \~Chinese 文件截取需要实现的方法包括：
*   \~ - hasDataFiles
*   \~ - pickSession
*/

// CPU architecture validation / CPU架构检查
#if defined(__x86_64__) || defined(_M_X64)
#if !defined(SPADAS_ARCH_X86) && defined(SPADAS_ARCH_ARM)
#error Not for X86 architecture.
#endif
#elif defined(__aarch64__)
#if !defined(SPADAS_ARCH_ARM)
#error Not for Arm architecture.
#endif
#else
#error Unsupported CPU architecture.
#endif

// For symbol I/O / 用于类库I/O
#if defined(SPADAS_ENV_WINDOWS)
#define SPADAS_DEFAULT_API __declspec(dllexport)
#define SPADAS_WINDOWS_DLLIMPORT __declspec(dllimport)
#endif
#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
#define SPADAS_DEFAULT_API __attribute__ ((visibility("default")))
#endif

// Code location / 代码信息
#define SPADAS_LOCATION spadas::String(__PRETTY_FUNCTION__).replace("spadas::", "")

// For error printings / 用于错误提示
#define SPADAS_ERROR_MSG(msg) spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] " + msg)
#define SPADAS_ERROR_PASS(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition); }
#define SPADAS_ERROR_BREAK(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition); break; }
#define SPADAS_ERROR_CONTINUE(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition); continue; }
#define SPADAS_ERROR_RETURN(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition); return; }
#define SPADAS_ERROR_RETURNVAL(condition, returnVal) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition); return (returnVal); }
#define SPADAS_ERROR_PASS_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); }
#define SPADAS_ERROR_BREAK_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); break; }
#define SPADAS_ERROR_CONTINUE_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); continue; }
#define SPADAS_ERROR_RETURN_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); return; }
#define SPADAS_ERROR_RETURNVAL_MSG(condition, msg, returnVal) if (condition) { spadas::console::print((spadas::String)"[SPADAS ERROR @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); return (returnVal); }

// For warning printings / 用于警告提示
#define SPADAS_WARNING_MSG(msg) spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] " + msg)
#define SPADAS_WARNING_PASS(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition); }
#define SPADAS_WARNING_BREAK(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition); break; }
#define SPADAS_WARNING_CONTINUE(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition); continue; }
#define SPADAS_WARNING_RETURN(condition) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition); return; }
#define SPADAS_WARNING_RETURNVAL(condition, returnVal) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition); return (returnVal); }
#define SPADAS_WARNING_PASS_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); }
#define SPADAS_WARNING_BREAK_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); break; }
#define SPADAS_WARNING_CONTINUE_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); continue; }
#define SPADAS_WARNING_RETURN_MSG(condition, msg) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); return; }
#define SPADAS_WARNING_RETURNVAL_MSG(condition, msg, returnVal) if (condition) { spadas::console::print((spadas::String)"[SPADAS WARNING @ " + SPADAS_LOCATION + "] "#condition + ". " + msg); return (returnVal); }

// Avoid macro conflicting / 避免宏定义冲突
#if defined(TRUE)
#undef TRUE
#endif
#if defined(FALSE)
#undef FALSE
#endif
#if defined(abs)
#undef abs
#endif
#if defined(max)
#undef max
#endif
#if defined(min)
#undef min
#endif
#if defined(check)
#undef check
#endif

// For comma in macro / 在宏定义中使用逗号
#define COMMA ,

// Spadas symbol I/O / Spadas类库I/O
#if defined(SPADAS_ENV_WINDOWS) && !defined(SPADAS_EXPORTS)
#define SPADAS_API SPADAS_WINDOWS_DLLIMPORT
#else
#define SPADAS_API SPADAS_DEFAULT_API
#endif

// Use variable arguments / 支持可变参数列表
#include <cstdarg>

// Use replacement new / 支持指定位置构造函数
#include <new>

// Support for lambda expression variables / 支持lambda表达式变量
#include <functional>
#define Func typename std::function

// Null pointer / 空指针
#ifndef NULL
#define NULL 0
#endif

// Recursive macro / 可递归宏
#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...)  EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_END(...)
#define MAP_OUT
#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0(test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP1)(f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP0)(f, peek, __VA_ARGS__)
#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

// Convenience for converting enumeration value to string / 方便实现枚举值转字符串
#define MACRO_ENUM_VALUE(val) case Value::val: return #val;
#define SPADAS_ENUM_VALUES(...) static const spadas::Char* toString(Value val) { switch (val) { MAP(MACRO_ENUM_VALUE, __VA_ARGS__) default: return 0; } }

// Convenience for debugging / 方便调试
#define MACRO_SEE(var) seeVarStringList.append((spadas::String) #var + " = " + (var));
#define see(...) { spadas::ArrayX<spadas::String> seeVarStringList; MAP(MACRO_SEE, __VA_ARGS__) spadas::console::print(spadas::String::merge(seeVarStringList.toArray())); }
#define seeArray(arr) spadas::console::print((spadas::String) #arr + " = [ " + spadas::String::merge(arr) + " ]")

// Convenience for string concatenation / 方便字符串拼接
#define cat +(spadas::String)
#define MACRO_CAT_ALL(var) (var) +
#define catAll(...) (spadas::String) MAP(MACRO_CAT_ALL, __VA_ARGS__) ""

// Definitions of infinity / 无限值定义
#define FINF spadas::math::finf()
#define NFINF spadas::math::nfinf()
#define DINF spadas::math::dinf()
#define NDINF spadas::math::ndinf()

// Convenience for type conversion definitions / 方便用于类型转换的定义
#define SPADAS_TYPE(typeNameString) static spadas::String typeName() { static spadas::String name = typeNameString; return name; }
#define SPADAS_VARS(classType, baseVarsType) virtual spadas::String getTypeName() override { return classType::typeName(); } \
virtual spadas::Bool isType(spadas::ULong id) override { return id == classType::typeName().getID() || baseVarsType::isType(id); } \
virtual spadas::Bool isType(spadas::String name) override { return name == classType::typeName() || baseVarsType::isType(name); }

// Definition of channel numbers / 通道数量定义
#define BC_NUM 16 // Bus / 总线
#define VC_NUM 24 // Video / 视频

#endif
