
#include "spadas.h"

using namespace spadas;

// 插件版本接口
SPADAS_DEFAULT_API void get_native_plugin_api_version(UInt& major, UInt& minor)
{
	major = 2;
	minor = 0;
}

SPADAS_DEFAULT_API void get_bus_plugin_api_version(UInt& major, UInt& minor)
{
	major = 3;
	minor = 0;
}

SPADAS_DEFAULT_API void get_video_plugin_api_version(UInt& major, UInt& minor)
{
	major = 5;
	minor = 0;
}

SPADAS_DEFAULT_API void get_dev_plugin_api_version(UInt& major, UInt& minor)
{
	major = 3;
	minor = 0;
}

SPADAS_DEFAULT_API void get_proc_plugin_api_version(UInt& major, UInt& minor)
{
	major = 7;
	minor = 0;
}

SPADAS_DEFAULT_API void get_file_plugin_api_version(UInt& major, UInt& minor)
{
	major = 2;
	minor = 0;
}

// 一般原生插件接口兼容性
// class CompatiblePluginVars : public Vars
// {
// public:
// 	UInt version;
// 	Interface<IPluginV101> i101;
// 	Interface<IPluginV102> i102;
// 	Interface<IPluginV103> i103;
// };

// class CompatiblePlugin : public Object<CompatiblePluginVars>, public IPlugin
// {
// public:
// 	CompatiblePlugin()
// 	{}
// 	CompatiblePlugin(Interface<IPluginV101> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
// 	{
// 		vars->version = 101;
// 		vars->i101 = i;
// 	}
// 	CompatiblePlugin(Interface<IPluginV102> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
// 	{
// 		vars->version = 102;
// 		vars->i102 = i;
// 	}
// 	CompatiblePlugin(Interface<IPluginV103> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
// 	{
// 		vars->version = 103;
// 		vars->i103 = i;
// 	}
// 	String getPluginType() override
// 	{
// 		switch (vars->version)
// 		{
// 		case 101:
// 			return vars->i101->getPluginType();
// 		case 102:
// 			return vars->i102->getPluginType();
// 		case 103:
// 			return vars->i103->getPluginType();
// 		default:
// 			return String();
// 		}
// 	}
// 	String getPluginVersion() override
// 	{
// 		switch (vars->version)
// 		{
// 		case 101:
// 			return vars->i101->getPluginVersion();
// 		case 102:
// 			return vars->i102->getPluginVersion();
// 		case 103:
// 			return vars->i103->getPluginVersion();
// 		default:
// 			return String();
// 		}
// 	}
// 	void closePlugin() override
// 	{
// 		switch (vars->version)
// 		{
// 		case 101:
// 			vars->i101->closePlugin();
// 			break;
// 		case 102:
// 			vars->i102->closePlugin();
// 			break;
// 		case 103:
// 			vars->i103->closePlugin();
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// };

SPADAS_DEFAULT_API Interface<IPluginV200> get_compatible_plugin(Pointer func, UInt minor)
{
	if (!func) return Interface<IPluginV200>();
	switch (minor)
	{
	case 0:
	{
		GetPluginV200 getPlugin = (GetPluginV200)func;
		return getPlugin();
	}
	// case 1:
	// {
	// 	GetPluginV101 getPlugin = (GetPluginV101)func;
	// 	auto i = getPlugin();
	// 	if (i.isValid()) return CompatiblePlugin(i);
	// 	else return Interface<IPlugin>();
	// }
	// case 2:
	// {
	// 	GetPluginV102 getPlugin = (GetPluginV102)func;
	// 	auto i = getPlugin();
	// 	if (i.isValid()) return CompatiblePlugin(i);
	// 	else return Interface<IPlugin>();
	// }
	// case 3:
	// {
	// 	GetPluginV103 getPlugin = (GetPluginV103)func;
	// 	auto i = getPlugin();
	// 	if (i.isValid()) return CompatiblePlugin(i);
	// 	else return Interface<IPlugin>();
	// }
	default:
		return Interface<IPluginV200>();
	}
}

// 通用功能插件接口 2.0
String IPluginV200::getPluginType()
{
	return String();
}

String IPluginV200::getPluginVersion()
{
	return String();
}

void IPluginV200::closePlugin()
{}

void IPluginV200::initLanguage(String languageCode)
{}

void IPluginV200::useLogger(Interface<ILogger> logger)
{}

void IPluginV200::setAppFilesPath(Path path)
{}

void IPluginV200::onCrossData(String id, Binary data)
{}

void IPluginV200::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV200::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV200::useCrossCaller(Interface<ICrossCaller> caller)
{}

void IPluginV200::onStartOnlineSession(SessionIdentifier session, ULong startCPUTick)
{}

void IPluginV200::onStopOnlineSession()
{}

Array<String> IPluginV200::getStandaloneTaskNames()
{
	return Array<String>();
}

void IPluginV200::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

Array<String> IPluginV200::getGuestSyncChannelNames()
{
	return Array<String>();
}

// 一般设备插件接口 3.0
void IDevicePluginV300::setDeviceConnection(String config)
{}

void IDevicePluginV300::disconnectDevice()
{}

Enum<GeneralDeviceStatus> IDevicePluginV300::getDeviceStatus(String& info)
{
	return GeneralDeviceStatus::Value::NotConnect;
}

Array<Enum<GeneralDeviceStatus> > IDevicePluginV300::getChildDeviceStatus()
{
	return Array<Enum<GeneralDeviceStatus> >();
}

Array<GeneralDeviceData> IDevicePluginV300::getDeviceNewData()
{
	return Array<GeneralDeviceData>();
}

Array<String> IDevicePluginV300::getTransmittableProtocols()
{
	return Array<String>();
}

Array<String> IDevicePluginV300::getScheduledTransmittableProtocols()
{
	return Array<String>();
}

Bool IDevicePluginV300::transmitGeneralData(String protocol, Array<Double> vector, Binary binary)
{
	return FALSE;
}

Bool IDevicePluginV300::transmitGeneralDataScheduled(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

// 总线设备插件接口 3.0
Array<BusDeviceInfo> IBusPluginV300::getBusDeviceList()
{
	return Array<BusDeviceInfo>();
}

Bool IBusPluginV300::openBusDevice(Array<BusDeviceConfig> configs)
{
	return FALSE;
}

void IBusPluginV300::closeBusDevice()
{}

Bool IBusPluginV300::receiveBusMessage(BusDeviceData& rxData)
{
	return FALSE;
}

Bool IBusPluginV300::transmitBusMessage(UInt channel, UInt id, Binary binary)
{
	return FALSE;
}

Bool IBusPluginV300::transmitBusMessageScheduled(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

void IBusPluginV300::setBusExtraConfig(String extra)
{}

Array<BusChannelPayload> IBusPluginV300::getBusPayload()
{
	return Array<BusChannelPayload>();
}

// 视频设备插件接口 5.0
Array<VideoDeviceInfo> IVideoPluginV500::getVideoDeviceList()
{
	return Array<VideoDeviceInfo>();
}

Bool IVideoPluginV500::openVideoDevice(Array<VideoDeviceConfig> configs)
{
	return FALSE;
}

void IVideoPluginV500::closeVideoDevice()
{}

Bool IVideoPluginV500::queryVideoFrame(VideoDeviceData& frame)
{
	return FALSE;
}

Bool IVideoPluginV500::transmitVideoFrame(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data)
{
	return FALSE;
}

Bool IVideoPluginV500::transmitVideoFrameScheduled(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

void IVideoPluginV500::setVideoExtraConfig(String extra)
{}

Array<GeneralDeviceData> IVideoPluginV500::getVideoDeviceNewData()
{
	return Array<GeneralDeviceData>();
}

void IVideoPluginV500::useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress)
{}

Array<String> IVideoPluginV500::getExclusiveKeywords()
{
	return Array<String>();
}

// 原生数据处理插件接口 7.0
Bool IProcessorPluginV700::isProcessorOnlineOnly()
{
	return FALSE;
}

Bool IProcessorPluginV700::isProcessorOfflineOnly()
{
	return FALSE;
}

void IProcessorPluginV700::setProcessorConfig(String config, Bool onlineMode)
{}

void IProcessorPluginV700::disableProcessor()
{}

void IProcessorPluginV700::processData(InputTables inputs, SessionSampleBufferTable sampleBuffers, OutputTables outputs)
{}

void IProcessorPluginV700::useTimeServer(Interface<ITimeServer> timeServer)
{}

void IProcessorPluginV700::useGeneralTransmitter(Interface<IGeneralDataTransmitter> generalTransmitter)
{}

void IProcessorPluginV700::useBusTransmitter(Interface<IBusMessageTransmitter> busTransmitter)
{}

void IProcessorPluginV700::useVideoTransmitter(Interface<IVideoFrameTransmitter> videoTransmitter)
{}

// 文件读写插件接口 2.0
Double IFilePluginV200::getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo, Interface<ITimeServer> timeServer)
{
	return 0;
}

Bool IFilePluginV200::openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double jumpOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo, Interface<ITimeServer> timeServer)
{
	return FALSE;
}

Bool IFilePluginV200::readFilesData(String readerName, InputTables inputs, Double targetOffset, Flag shouldEnd)
{
	return FALSE;
}

void IFilePluginV200::closeReadFiles(String readerName)
{}

Bool IFilePluginV200::openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo, Interface<ITimeServer> timeServer)
{
	return FALSE;
}

void IFilePluginV200::writeFilesData(String writerName, InputTables inputs, Array<SessionBusRawData> busMessages, Flag shouldEnd)
{}

void IFilePluginV200::closeWriteFiles(String writerName)
{}

Bool IFilePluginV200::hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo)
{
	return FALSE;
}

void IFilePluginV200::pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback, Interface<ITimeServer> timeServer)
{}

void IFilePluginV200::setFileExtraConfig(String extra)
{}