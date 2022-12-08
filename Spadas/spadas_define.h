
#ifndef SPADAS_DEFINE_H
#define SPADAS_DEFINE_H

// 版本定义 // linux/macos版原子操作加内存屏障
#define SPADAS_VERSION_MAJOR 8
#define SPADAS_VERSION_MINOR 3
#define SPADAS_VERSION_BUILD 14

/*! \mainpage
* Spadas是支持Windows、Linux等操作系统的“一次编写到处编译”C++多功能类库。\n
* 本文档对应Spadas版本：8.3.14\n
*
* \n
* \section top1 基本功能概述
* \subsection f1 1. 引用计数
* 实现一个引用计数类需要先声明一个 spadas::Vars 的子类用于定义私有成员变量，所有变量都定义在public域内。\n
* 然后声明一个以该Vars子类作为模板类型的 spadas::Object 子类，用于定义功能函数。\n
* 该Object子类只需实现构造函数（且必须实现一个无参数构造函数），而无需实现拷贝构造函数、析构函数、等于号重载等，即可实现引用计数。\n
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
*   - spadas::DoubleMat : 矩阵
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
* 实现多线程需要声明一个 spadas::Object 和 spadas::IWorkflow 的子类，并实现IWorkflow中定义的接口方法（其中可选部分可不实现）。\n
* 在程序中启动多线程需要调用静态函数 spadas::Threads::start 。\n
* 停止线程需要调用成员函数 spadas::Threads::stop ，若没有Threads对象可通过静态函数 spadas::Threads::getThreadsOf 获得。\n
* 另外，多线程中需要经常使用 spadas::Lock 确保线程间数据访问的安全；而线程间同步可通过 spadas::Flag 或 spadas::Barrier 实现。\n
*
* \subsection f9 9. 其他功能
* Spadas还提供以下功能：
* - spadas::Time : 日期与时间，可通过 spadas::utility::getTime 获取
* - spadas::Timer : 计时器
* - spadas::Tick : 振荡器
* - spadas::MemoryMap : 内存映射
* - spadas::MemoryMapStream : 基于内存映射的数据流
*
* \n
* \section top2 插件框架概述
* \subsection p1 1. 一般原生插件实现方式
*   - 一般原生插件的库文件名需要以"native_"或"libnative_"开头
*   - 实现一个类继承 spadas::IPluginV101
*   - 实现一个全局函数 spadas::GetPluginV101 ，返回上述类对象（定义该全局函数需要前缀SPADAS_DEFAULT_API）
*
* \subsection p2 2. 插件通用定义
* 全局函数格式为 spadas::GetPluginV101 \n
* 需要实现以下接口函数：
*   - spadas::IPluginV101::getPluginType
*   - spadas::IPluginV101::getPluginVersion
* 
* 以下接口函数可根据实际需要选择是否实现：
*   - spadas::IPluginV101::closePlugin
*   - spadas::IPluginV101::onCrossData
*   - spadas::IPluginV101::useCrossTransmitter
*   - spadas::IPluginV101::onCrossCall
*   - spadas::IPluginV101::useCrossCaller
*
* \subsection p3 3. 扩展原生插件实现方式
*   - 扩展原生插件的库文件名根据插件类型而不同（详见后续小节）
*   - 实现一个类同时继承 spadas::IPluginV101 和扩展的插件接口
*   - 实例化一个上述类的对象
*   - 分别实现全局函数 spadas::GetPluginV101 和扩展插件接口对应的全局函数，都返回上述对象（定义该全局函数需要前缀SPADAS_DEFAULT_API）
*
* \subsection p4 4. 数据处理插件定义
* 数据处理插件的库文件名需要以"proc_"或"libproc_"开头。\n
* 数据处理插件对应的全局函数格式为 spadas::GetProcessorPluginV601 \n
* 此类插件可支持数据流模式、独立任务模式、或两种同时支持。\n\n
*
* 数据流模式下，所有数据处理都在session中进行，需要实现以下接口函数：
*   - spadas::IProcessorPluginV601::isDataStreamModeSupported （返回TRUE）
*   - spadas::IProcessorPluginV601::isUsingSetProcessorConfigX
*   - spadas::IProcessorPluginV601::setProcessorConfig 或 spadas::IProcessorPluginV601::setProcessorConfigX
*   - spadas::IProcessorPluginV601::disableProcessor
*   - spadas::IProcessorPluginV601::processData
*
* 数据流模式下，以下接口函数可根据实际需要选择是否实现：
*   - spadas::IProcessorPluginV601::updateStartTimeLocal
*   - spadas::IProcessorPluginV601::updateStartTimeUTC
*   - spadas::IProcessorPluginV601::useBusTransmitter
*
* 独立任务模式下，数据处理在非session时段进行，需要实现以下接口函数：
*   - spadas::IProcessorPluginV601::isStandaloneTaskModeSupported （返回TRUE）
*   - spadas::IProcessorPluginV601::runStandaloneTask
*
* \subsection p5 5. 通用设备插件定义
* 通用设备插件的库文件名需要以"dev_"或"libdev_"开头。\n
* 通用设备插件对应的全局函数格式为 spadas::GetDevicePluginV201 \n
* 通用设备插件需要实现以下接口函数：
*   - spadas::IDevicePluginV201::setDeviceConnection
*   - spadas::IDevicePluginV201::disconnectDevice
*   - spadas::IDevicePluginV201::getDeviceStatus
*   - spadas::IDevicePluginV201::resetDeviceDataStream
*   - spadas::IDevicePluginV201::closeDeviceDataStream
*   - spadas::IDevicePluginV201::getDeviceNewData
*
* 以下接口函数可根据实际需要选择是否实现：
*   - spadas::IDevicePluginV201::getChildDeviceStatus
*   - spadas::IDevicePluginV201::useBusTransmitter
*   - spadas::IDevicePluginV201::getBufferFilesLatestTime
*   - spadas::IDevicePluginV201::clearBufferFiles
*   - spadas::IDevicePluginV201::pickEvent
*
* \subsection p6 6. 总线设备插件定义
* 总线设备插件的库文件名需要以"bus_"或"libbus_"开头。\n
* 总线设备插件对应的全局函数格式为 spadas::GetBusPluginV200 \n
* 总线设备插件需要实现以下接口函数：
*   - spadas::IBusPluginV200::getBusDeviceList
*   - spadas::IBusPluginV200::openBusDevice
*   - spadas::IBusPluginV200::closeBusDevice
*   - spadas::IBusPluginV200::receiveBusMessage
*
* 以下接口函数可根据实际需要选择是否实现：
*   - spadas::IBusPluginV200::transmitBusMessage
*   - spadas::IBusPluginV200::setBusExtraConfig
*   - spadas::IBusPluginV200::getBusPayload
*
* \subsection p7 7. 视频设备插件定义
* 视频设备插件的库文件名需要以"video_"或"libvideo_"开头。\n
* 视频设备插件对应的全局函数格式为 spadas::GetVideoPluginV401 \n
* 视频设备插件需要实现以下接口函数：
*   - spadas::IVideoPluginV401::getVideoDeviceList
*   - spadas::IVideoPluginV401::openVideoDevice
*   - spadas::IVideoPluginV401::closeVideoDevice
*   - spadas::IVideoPluginV401::queryVideoFrame
*
* 以下接口函数可根据实际需要选择是否实现：
*   - spadas::IVideoPluginV401::setVideoExtraConfig
*   - spadas::IVideoPluginV401::getVideoDeviceNewData
*   - spadas::IVideoPluginV401::useVideoPreviewExpress
*   - spadas::IVideoPluginV401::getExclusiveKeywords
*
* \subsection p8 8. 文件读写插件定义
* 文件读写插件的库文件名需要以"file_"或"libfile_"开头。\n
* 文件读写插件对应的全局函数格式为 spadas::GetFilePluginV102 \n
* 此类插件可支持文件读取、文件写入、以及文件截取。\n\n
*
* 文件读取需要实现以下接口函数：
*   - spadas::IFilePluginV102::getFilesDuration
*   - spadas::IFilePluginV102::openReadFiles
*   - spadas::IFilePluginV102::readFilesData
*   - spadas::IFilePluginV102::closeReadFiles
*
* 文件写入需要实现以下接口函数：
*   - spadas::IFilePluginV102::openWriteFiles
*   - spadas::IFilePluginV102::writeFilesData
*   - spadas::IFilePluginV102::closeWriteFiles
*
* 文件截取需要实现以下接口函数：
*   - spadas::IFilePluginV102::hasDataFiles
*   - spadas::IFilePluginV102::pickSession
*
* 以下接口函数可根据实际需要选择是否实现：
*   - spadas::IFilePluginV102::setFileExtraConfig
*   - spadas::IFilePluginV102::updateStartTimeLocal
*   - spadas::IFilePluginV102::updateStartTimeUTC
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
#elif
#error Unsupported CPU architecture.
#endif

// 用于类库I/O
#if defined(SPADAS_ENV_WINDOWS)
#define SPADAS_DEFAULT_API __declspec(dllexport)
#define SPADAS_WINDOWS_DLLIMPORT __declspec(dllimport)
#endif
#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
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

// 用于调试打印
#if defined(SPADAS_DEBUG)
#define SPADAS_DEBUG_MSG(msg) spadas::console::print((spadas::String)"[" + SPADAS_LOCATION + "] " + msg)
#else
#define SPADAS_DEBUG_MSG(msg)
#endif

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
#include <stdarg.h>

// 支持指定位置构造函数
#include <new>

// 空指针
#ifndef NULL
#define NULL 0
#endif

// 字符串转换
#define SS (spadas::String)

// 无限值定义
#define FINF spadas::math::finf()
#define NFINF spadas::math::nfinf()
#define DINF spadas::math::dinf()
#define NDINF spadas::math::ndinf()

// 方便变量数据定义
#define SPADAS_VARS_DEF(classType, baseVarsType) virtual String getTypeName() override { return classType::TypeName; } virtual ListNode<String> getBaseChain() override { return genBaseChain(baseVarsType::getTypeName(), baseVarsType::getBaseChain()); }

// 调试用
#if defined(SPADAS_DEBUG)
#define SPADAS_BINARY_DUMMY_BYTES 12
#define SPADAS_STRING_DUMMY_BYTES 16
#endif

// OpenCV兼容性
typedef struct _IplImage IplImage;
typedef struct CvMat CvMat;

#endif
