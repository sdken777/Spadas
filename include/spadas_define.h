
#ifndef SPADAS_DEFINE_H
#define SPADAS_DEFINE_H

// Update log / 更新记录: Windows下的system::command使用CreateProcess

// Version definition / 版本定义
#define SPADAS_VERSION_MAJOR 8
#define SPADAS_VERSION_MINOR 9
#define SPADAS_VERSION_BUILD 2

/*! \mainpage
* \~English Spadas is a "write once and compile everywhere" C++ multifunctional class library that supports Windows, Linux and other operating systems. \n
* \~Chinese Spadas是支持Windows、Linux等操作系统的“一次编写到处编译”C++多功能类库。 \n
* \~English This document corresponds to Spadas version: 8.9.2 \n
* \~Chinese 本文档对应Spadas版本：8.9.2 \n
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
* \~English The derived class of Object only needs to implement a constructor (and must implement a parameterless constructor), without implementing a copy constructor, a destructor, an equal operator, etc., to implement reference counting. \n
* \~Chinese 该Object子类只需实现构造函数（且必须实现一个无参数构造函数），而无需实现拷贝构造函数、析构函数、等于号重载等，即可实现引用计数。 \n
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
*   \~English - spadas::DoubleMat : Matrix
*   \~Chinese - spadas::DoubleMat : 矩阵
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
* \~English To implement multi-threading, you need to declare a derived class of spadas::Object and spadas::IWorkflow, and implement the virtual methods defined in IWorkflow (the optional part may not be implemented). \n
* \~Chinese 实现多线程需要声明一个 spadas::Object 和 spadas::IWorkflow 的子类，并实现IWorkflow中定义的接口方法（其中可选部分可不实现）。 \n
* \~English Starting multiple threads in a program requires calling the static function spadas::Threads::start . \n
* \~Chinese 在程序中启动多线程需要调用静态函数 spadas::Threads::start 。 \n
* \~English To stop a thread, you need to call the member function spadas::Threads::stop. If there is no Threads object, you can get it through the static function spadas::Threads::getThreadsOf. \n
* \~Chinese 停止线程需要调用成员函数 spadas::Threads::stop ，若没有Threads对象可通过静态函数 spadas::Threads::getThreadsOf 获得。 \n
* \~English In addition, spadas::Lock is often used in multi-threading to ensure the security of data access between threads; and inter-thread synchronization can be achieved through spadas::Flag or spadas::Barrier. \n
* \~Chinese 另外，多线程中需要经常使用 spadas::Lock 确保线程间数据访问的安全；而线程间同步可通过 spadas::Flag 或 spadas::Barrier 实现。 \n
*
* \~English \subsection f9 9. Other functions
* \~Chinese \subsection f9 9. 其他功能
* \~English Spadas also provides the following features:
* \~Chinese Spadas还提供以下功能：
* \~English - spadas::Time : Date and time, available through spadas::utility::getTime
* \~Chinese - spadas::Time : 日期与时间，可通过 spadas::utility::getTime 获取
* \~English - spadas::Timer : Timer
* \~Chinese - spadas::Timer : 计时器
* \~English - spadas::Tick : Oscillator
* \~Chinese - spadas::Tick : 振荡器
* \~English - spadas::MemoryMap : Memory map
* \~Chinese - spadas::MemoryMap : 内存映射
* \~English - spadas::MemoryMapStream : Memory-map-based data stream
* \~Chinese - spadas::MemoryMapStream : 基于内存映射的数据流
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
*   \~English - General function plugin interface: spadas::IPluginVxxx, corresponding global function definition: spadas::GetPluginVxxx (where xxx indicates the interface version, such as 103, 201, etc., the same below)
*   \~Chinese - 通用功能插件接口: spadas::IPluginVxxx ，对应的全局函数定义: spadas::GetPluginVxxx（其中xxx表示接口版本，如103、201等等，下同）
*   \~English - Native data processing plugin interface: spadas::IProcessorPluginVxxx, corresponding global function definition: spadas::GetProcessorPluginVxxx
*   \~Chinese - 原生数据处理插件接口: spadas::IProcessorPluginVxxx ，对应的全局函数定义: spadas::GetProcessorPluginVxxx
*   \~English - General device plugin interface: spadas::IDevicePluginVxxx, corresponding global function definition: spadas::GetDevicePluginVxxx
*   \~Chinese - 一般设备插件接口: spadas::IDevicePluginVxxx ，对应的全局函数定义: spadas::GetDevicePluginVxxx
*   \~English - Bus device plugin interface: spadas::IBusPluginVxxx , corresponding global function definition: spadas::GetBusPluginVxxx
*   \~Chinese - 总线设备插件接口: spadas::IBusPluginVxxx ，对应的全局函数定义: spadas::GetBusPluginVxxx
*   \~English - Video device plugin interface: spadas::IVideoPluginVxxx , corresponding global function definition: spadas::GetVideoPluginVxxx
*   \~Chinese - 视频设备插件接口: spadas::IVideoPluginVxxx ，对应的全局函数定义: spadas::GetVideoPluginVxxx
*   \~English - File read and write (R/W) plugin interface: spadas::IFilePluginVxxx, corresponding global function definition: spadas::GetFilePluginVxxx
*   \~Chinese - 文件读写插件接口: spadas::IFilePluginVxxx ，对应的全局函数定义: spadas::GetFilePluginVxxx
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
* \~English The following interface functions must be implemented:
* \~Chinese 以下接口函数必须实现：
*   - spadas::IPluginVxxx::getPluginType
*   - spadas::IPluginVxxx::getPluginVersion
* 
* \~English Optional implementation of the following interface functions:
* \~Chinese 以下接口函数可选实现：
*   \~ - spadas::IPluginVxxx::closePlugin
*   \~ - spadas::IPluginVxxx::initLanguage
*   \~ - spadas::IPluginVxxx::useLogger
*   \~ - spadas::IPluginVxxx::setAppFilesPath
*   \~ - spadas::IPluginVxxx::onCrossData
*   \~ - spadas::IPluginVxxx::useCrossTransmitter
*   \~ - spadas::IPluginVxxx::onCrossCall
*   \~ - spadas::IPluginVxxx::useCrossCaller
*   \~ - spadas::IPluginVxxx::onStartOnlineSession
*   \~ - spadas::IPluginVxxx::onStopOnlineSession
*   \~ - spadas::IPluginVxxx::getStandaloneTaskNames
*   \~ - spadas::IPluginVxxx::runStandaloneTask
*   \~ - spadas::IPluginVxxx::getGuestSyncChannelNames
*
* \~English \subsection p5 5. Native data processing plugin interface
* \~Chinese \subsection p5 5. 实现原生数据处理插件接口
* \~English The following interface functions must be implemented:
* \~Chinese 以下接口函数必须实现：
*   \~ - spadas::IProcessorPluginVxxx::setProcessorConfig
*   \~ - spadas::IProcessorPluginVxxx::disableProcessor
*   \~ - spadas::IProcessorPluginVxxx::processData
*
* \~English Optional implementation of the following interface functions:
* \~Chinese 以下接口函数可选实现：
*   \~ - spadas::IProcessorPluginVxxx::isProcessorOnlineOnly
*   \~ - spadas::IProcessorPluginVxxx::isProcessorOfflineOnly
*   \~ - spadas::IProcessorPluginVxxx::useTimeServer
*   \~ - spadas::IProcessorPluginVxxx::useGeneralTransmitter
*   \~ - spadas::IProcessorPluginVxxx::useBusTransmitter
*   \~ - spadas::IProcessorPluginVxxx::useVideoTransmitter
*
* \~English \subsection p6 6. General device plugin interface
* \~Chinese \subsection p6 6. 实现一般设备插件接口
* \~English The following interface functions must be implemented:
* \~Chinese 以下接口函数必须实现：
*   \~ - spadas::IDevicePluginVxxx::setDeviceConnection
*   \~ - spadas::IDevicePluginVxxx::disconnectDevice
*   \~ - spadas::IDevicePluginVxxx::getDeviceStatus
*
* \~English Optional implementation of the following interface functions:
* \~Chinese 以下接口函数可选实现：
*   \~ - spadas::IDevicePluginVxxx::getChildDeviceStatus
*   \~ - spadas::IDevicePluginVxxx::getDeviceNewData
*   \~ - spadas::IDevicePluginVxxx::getTransmittableProtocols
*   \~ - spadas::IDevicePluginVxxx::getScheduledTransmittableProtocols
*   \~ - spadas::IDevicePluginVxxx::transmitGeneralData
*   \~ - spadas::IDevicePluginVxxx::transmitGeneralDataScheduled
*
* \~English \subsection p7 7. Bus device plugin interface
* \~Chinese \subsection p7 7. 实现总线设备插件接口
* \~English The following interface functions must be implemented:
* \~Chinese 以下接口函数必须实现：
*   \~ - spadas::IBusPluginVxxx::getBusDeviceList
*   \~ - spadas::IBusPluginVxxx::openBusDevice
*   \~ - spadas::IBusPluginVxxx::closeBusDevice
*
* \~English Optional implementation of the following interface functions:
* \~Chinese 以下接口函数可选实现：
*   \~ - spadas::IBusPluginVxxx::receiveBusMessage
*   \~ - spadas::IBusPluginVxxx::transmitBusMessage
*   \~ - spadas::IBusPluginVxxx::transmitBusMessageScheduled
*   \~ - spadas::IBusPluginVxxx::setBusExtraConfig
*   \~ - spadas::IBusPluginVxxx::getBusPayload
*
* \~English \subsection p8 8. Video device plugin interface
* \~Chinese \subsection p8 8. 实现视频设备插件接口
* \~English The following interface functions must be implemented:
* \~Chinese 以下接口函数必须实现：
*   \~ - spadas::IVideoPluginVxxx::getVideoDeviceList
*   \~ - spadas::IVideoPluginVxxx::openVideoDevice
*   \~ - spadas::IVideoPluginVxxx::closeVideoDevice
*
* \~English Optional implementation of the following interface functions:
* \~Chinese 以下接口函数可选实现：
*   \~ - spadas::IVideoPluginVxxx::queryVideoFrame
*   \~ - spadas::IVideoPluginVxxx::transmitVideoFrame
*   \~ - spadas::IVideoPluginVxxx::transmitVideoFrameScheduled
*   \~ - spadas::IVideoPluginVxxx::setVideoExtraConfig
*   \~ - spadas::IVideoPluginVxxx::getVideoDeviceNewData
*   \~ - spadas::IVideoPluginVxxx::useVideoPreviewExpress
*   \~ - spadas::IVideoPluginVxxx::getExclusiveKeywords
*
* \~English \subsection p9 9. File R/W plugin interface
* \~Chinese \subsection p9 9. 实现文件读写插件接口
* \~English This kind of plugins may support file reading, file writing, and data picking. \n\n
* \~Chinese 此类插件可支持文件读取、文件写入、以及文件截取。 \n\n
*
* \~English File reading needs to implement the following interface functions:
* \~Chinese 文件读取需要实现以下接口函数：
*   \~ - spadas::IFilePluginVxxx::getFilesDuration
*   \~ - spadas::IFilePluginVxxx::openReadFiles
*   \~ - spadas::IFilePluginVxxx::readFilesData
*   \~ - spadas::IFilePluginVxxx::closeReadFiles
*
* \~English File writing needs to implement the following interface functions:
* \~Chinese 文件写入需要实现以下接口函数：
*   \~ - spadas::IFilePluginVxxx::openWriteFiles
*   \~ - spadas::IFilePluginVxxx::writeFilesData
*   \~ - spadas::IFilePluginVxxx::closeWriteFiles
*
* \~English Data picking needs to implement the following interface functions:
* \~Chinese 文件截取需要实现以下接口函数：
*   \~ - spadas::IFilePluginVxxx::hasDataFiles
*   \~ - spadas::IFilePluginVxxx::pickSession
*
* \~English Optional implementation of the following interface functions:
* \~Chinese 以下接口函数可选实现：
*   \~ - spadas::IFilePluginVxxx::setFileExtraConfig
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

// For debug printings / 用于调试打印
#if defined(SPADAS_DEBUG)
#define SPADAS_DEBUG_MSG(msg) spadas::console::print((spadas::String)"[" + SPADAS_LOCATION + "] " + msg)
#else
#define SPADAS_DEBUG_MSG(msg)
#endif

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

// Spadas symbol I/O / Spadas类库I/O
#if defined(SPADAS_ENV_WINDOWS) && !defined(SPADAS_EXPORTS)
#define SPADAS_API SPADAS_WINDOWS_DLLIMPORT
#else
#define SPADAS_API SPADAS_DEFAULT_API
#endif

// Use variable arguments / 支持可变参数列表
#include <stdarg.h>

// Use replacement new / 支持指定位置构造函数
#include <new>

// Null pointer / 空指针
#ifndef NULL
#define NULL 0
#endif

// Convenience for string conversion / 字符串转换
#define SS (spadas::String)

// Definitions of infinity / 无限值定义
#define FINF spadas::math::finf()
#define NFINF spadas::math::nfinf()
#define DINF spadas::math::dinf()
#define NDINF spadas::math::ndinf()

// Convenience for Vars definition / 方便变量数据定义
#define SPADAS_VARS_DEF(classType, baseVarsType) virtual String getTypeName() override { return classType::TypeName; } virtual ListNode<String> getBaseChain() override { return genBaseChain(baseVarsType::getTypeName(), baseVarsType::getBaseChain()); }

// For debug / 调试用
#if defined(SPADAS_DEBUG)
#define SPADAS_BINARY_DUMMY_BYTES 12
#define SPADAS_STRING_DUMMY_BYTES 16
#endif

// Definition of channel numbers / 通道数量定义
#define BC_NUM 16 // Bus / 总线
#define VC_NUM 24 // Video / 视频

// OpenCV compatibility / OpenCV兼容性
typedef struct _IplImage IplImage;
typedef struct CvMat CvMat;

#endif
