
#ifndef SPADAS_DEFINE_H
#define SPADAS_DEFINE_H

// 版本定义 // 新增File::appendXXX
#define SPADAS_VERSION_MAJOR 9
#define SPADAS_VERSION_MINOR 0
#define SPADAS_VERSION_BUILD 10

/*! \mainpage
* Spadas是支持Windows、Linux等操作系统的“一次编写到处编译”C++多功能类库。\n
* 本文档对应Spadas版本：9.0.10\n
*
* \n
* \section top1 基本功能概述
* \subsection f1 1. 引用计数
* 实现一个引用计数类需要先声明一个 spadas::Vars 的子类用于定义私有成员变量，所有变量都定义在public域内。\n
* 然后声明一个以该Vars子类作为模板类型的 spadas::Object 子类，用于定义功能函数。\n
* 该Object子类即可实现引用计数，且该对象的拷贝构造函数、析构函数都为多线程安全操作。\n
* 如需要重定向操作的多线程安全(包括等于号赋值操作， spadas::Object::setVars 等)，应使用 spadas::Safe 。\n
* 功能函数内部可通过vars->的方式访问私有成员变量。\n
*
* \subsection f2 2. 对象容器
* 对象容器包括以下类型：
*   - spadas::Interface : 接口容器
*   - spadas::Optional : 可空对象容器，可为任意数据类型增加有效性判断
*
* \subsection f3 3. 基本数据结构
* 基本数据结构包括以下类型：
*   - spadas::Array : 数组
*   - spadas::ArrayX : 可扩展数组
*   - spadas::List : 链表
*   - spadas::Stream : 队列
*   - spadas::Map : 映射表
*   - spadas::Dictionary : 字典(通过字符串索引)
*   - spadas::Binary : 二进制数据块
*   - spadas::String : 字符串
*   - spadas::XML : XML
*   - spadas::IntMat spadas::FloatMat spadas::DoubleMat : 矩阵
*
* \subsection f4 4. 控制台
* 控制台功能包括文字打印、文字和按键获取、弹出信息窗口等。具体请参考命名空间 spadas::console \n
*
* \subsection f5 5. 文件路径与文件IO
* 具体请参考 spadas::Path 和 spadas::File \n
*
* \subsection f6 6. 图像
* 仅考虑图像数据存储可使用 spadas::ImagePointer ；需要更复杂功能可使用 spadas::Image \n
*
* \subsection f7 7. 数学函数
* 具体请参考命名空间 spadas::math \n
*
* \subsection f8 8. 多线程
* 多线程可实现以下两类场景：\n
* - 启动多个子线程或以循环为主的场景，可实现 spadas::IWorkflow ，通过 spadas::Threads::start 开始，需要结束时调用 spadas::Threads::stop
* - 启动单个子线程执行任务的场景，可实现 spadas::ITask ，通过 spadas::TaskManager::start 开始，需要中断时调用 spadas::TaskManager::stop
* 多线程中需要经常使用 spadas::Lock 或 spadas::SpinLocked 确保线程间操作和数据访问的安全。
* 另外，线程间同步可通过 spadas::Flag 或 spadas::Barrier 实现。\n
*
* \subsection f9 9. 其他功能
* Spadas还提供以下功能：
* - spadas::Time spadas::TimeWithMS : 日期与时间
* - spadas::Timer : 计时器
* - spadas::Tick : 振荡器
* - spadas::MemoryMapStream : 基于内存映射的数据流
* - spadas::LibraryLoader : 动态加载库文件
* 以及 spadas::system 和 spadas::utility 命名空间下的功能。
*
* \subsection f10 10. 注释标签
* 在类和函数等的注释前可能有以下几类标签，以方括号表示：
* - 非安全: 涉及到指针或引用等，如操作不当可能导致程序崩溃
* - 多线程安全: 在不进行重定向操作的前提下，该对象的所有方法都为多线程安全操作。如有重定向，仍需使用 spadas::Safe 。另外该类的 spadas::Vars::isSpinLockManaged 应返回TRUE
* - 可选: 接口中非必须实现的方法
*
* \n
* \section top2 插件框架概述
* \subsection p1 1. 插件实现方式
*   - 插件库的文件名需要以指定关键字开头，并在库内提供一个或多个全局函数，返回对应的接口实现
*   - 定义这些全局函数需要前缀SPADAS_DEFAULT_API
*
* \subsection p2 2. 插件接口种类
*   - 通用功能插件接口: IPluginVxxx ，对应的全局函数定义: GetPluginVxxx（其中xxx表示接口版本，如103、201等等，下同）
*   - 原生数据处理插件接口: IProcessorPluginVxxx ，对应的全局函数定义: GetProcessorPluginVxxx
*   - 一般设备插件接口: IDevicePluginVxxx ，对应的全局函数定义: GetDevicePluginVxxx
*   - 总线设备插件接口: IBusPluginVxxx ，对应的全局函数定义: GetBusPluginVxxx
*   - 视频设备插件接口: IVideoPluginVxxx ，对应的全局函数定义: GetVideoPluginVxxx
*   - 文件读写插件接口: IFilePluginVxxx ，对应的全局函数定义: GetFilePluginVxxx
*
* \subsection p3 3. 插件库种类
*   - 通用功能库: 关键字"native_"或"libnative_"，支持通用功能插件接口
*   - 原生数据处理库: 关键字"proc_"或"libproc_"，支持通用功能插件接口、原生数据处理插件接口
*   - 一般设备库: 关键字"dev_"或"libdev_"，支持通用功能插件接口、一般设备插件接口
*   - 总线设备库: 关键字"bus_"或"libbus_"，支持通用功能插件接口、总线设备插件接口
*   - 视频设备库: 关键字"video_"或"libvideo_"，支持通用功能插件接口、视频设备插件接口
*   - 文件读写库: 关键字"file_"或"libfile_"，支持通用功能插件接口、文件读写插件接口
*   - 混合设备库: 关键字"mix_"或"libmix_"，支持通用功能插件接口、一般设备插件接口、总线设备插件接口、视频设备插件接口
*
* \subsection p4 4. 实现通用功能插件接口
* spadas::IPluginV200 中必须实现的方法包括：
*   - getPluginType
*   - getPluginVersion
*
* \subsection p5 5. 实现原生数据处理插件接口
* spadas::IProcessorPluginV700 中必须实现的方法包括：
*   - setProcessorConfig
*   - disableProcessor
*   - processData
*
* \subsection p6 6. 实现一般设备插件接口
* spadas::IDevicePluginV300 中必须实现的方法包括：
*   - setDeviceConnection
*   - disconnectDevice
*   - getDeviceStatus
*
* \subsection p7 7. 实现总线设备插件接口
* spadas::IBusPluginV300 中必须实现的方法包括：
*   - getBusDeviceList
*   - openBusDevice
*   - closeBusDevice
*
* \subsection p8 8. 实现视频设备插件接口
* spadas::IVideoPluginV500 中必须实现的方法包括：
*   - getVideoDeviceList
*   - openVideoDevice
*   - closeVideoDevice
*
* \subsection p9 9. 实现文件读写插件接口
* spadas::IFilePluginV200 可实现文件读取、文件写入、以及文件截取。\n\n
*
* 文件读取需要实现的方法包括：
*   - getFilesDuration
*   - openReadFiles
*   - readFilesData
*   - closeReadFiles
*
* 文件写入需要实现的方法包括：
*   - openWriteFiles
*   - writeFilesData
*   - closeWriteFiles
*
* 文件截取需要实现的方法包括：
*   - hasDataFiles
*   - pickSession
*/

// CPU架构检查
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

// 用于类库I/O
#if defined(SPADAS_ENV_WINDOWS)
#define SPADAS_DEFAULT_API __declspec(dllexport)
#define SPADAS_WINDOWS_DLLIMPORT __declspec(dllimport)
#endif
#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)
#define SPADAS_DEFAULT_API __attribute__ ((visibility("default")))
#endif

// 代码信息
#define SPADAS_LOCATION spadas::String(__PRETTY_FUNCTION__).replace("spadas::", "")

// 用于错误提示
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

// 用于警告提示
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

// 避免宏定义冲突
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

// Spadas类库I/O
#if defined(SPADAS_ENV_WINDOWS) && !defined(SPADAS_EXPORTS)
#define SPADAS_API SPADAS_WINDOWS_DLLIMPORT
#else
#define SPADAS_API SPADAS_DEFAULT_API
#endif

// 支持可变参数列表
#include <cstdarg>

// 支持指定位置构造函数
#include <new>

// 支持lambda表达式变量
#include <functional>
#define Func typename std::function

// 空指针
#ifndef NULL
#define NULL 0
#endif

// 方便转字符串
#define SS (spadas::String)

// 方便实现枚举值转字符串
#define ES(val) case Value::val: return #val

// 方便调试
#define PV(var) spadas::console::print((spadas::String) #var + " = " + var)
#define PV2(var1, var2) spadas::console::print((spadas::String) #var1 + " = " + var1 + ", " + #var2 + " = " + var2)
#define PV3(var1, var2, var3) spadas::console::print((spadas::String) #var1 + " = " + var1 + ", " + #var2 + " = " + var2 + ", " + #var3 + " = " + var3)
#define PV4(var1, var2, var3, var4) spadas::console::print((spadas::String) #var1 + " = " + var1 + ", " + #var2 + " = " + var2 + ", " + #var3 + " = " + var3 + ", " + #var4 + " = " + var4)
#define PA(arr) spadas::console::print((spadas::String) #arr + " = [ " + spadas::String::merge(arr) + " ]")

// 无限值定义
#define FINF spadas::math::finf()
#define NFINF spadas::math::nfinf()
#define DINF spadas::math::dinf()
#define NDINF spadas::math::ndinf()

// 方便变量数据定义
#define SPADAS_VARS_DEF(classType, baseVarsType) virtual spadas::String getTypeName() override { return classType::TypeName; } \
virtual spadas::Bool isType(spadas::ULong typeID) override { return typeID == classType::TypeName.getID() || baseVarsType::isType(typeID); } \
virtual spadas::Bool isType(spadas::String typeName) override { return typeName == classType::TypeName || baseVarsType::isType(typeName); }

// 通道数量定义
#define BC_NUM 16 // 总线
#define VC_NUM 24 // 视频

#endif
