
#include "spadas.h"

using namespace spadas;

// 插件版本接口
SPADAS_DEFAULT_API void get_native_plugin_api_version(UInt& major, UInt& minor)
{
	major = 1;
	minor = 6;
}

SPADAS_DEFAULT_API void get_bus_plugin_api_version(UInt& major, UInt& minor)
{
	major = 2;
	minor = 1;
}

SPADAS_DEFAULT_API void get_video_plugin_api_version(UInt& major, UInt& minor)
{
	major = 4;
	minor = 3;
}

SPADAS_DEFAULT_API void get_dev_plugin_api_version(UInt& major, UInt& minor)
{
	major = 2;
	minor = 2;
}

SPADAS_DEFAULT_API void get_proc_plugin_api_version(UInt& major, UInt& minor)
{
	major = 6;
	minor = 3;
}

SPADAS_DEFAULT_API void get_file_plugin_api_version(UInt& major, UInt& minor)
{
	major = 1;
	minor = 3;
}

// 一般原生插件接口兼容性
class CompatiblePluginVars : public Vars
{
public:
	UInt version;
	Interface<IPluginV101> i101;
	Interface<IPluginV102> i102;
	Interface<IPluginV103> i103;
	Interface<IPluginV104> i104;
	Interface<IPluginV105> i105;
	Interface<IPluginV106> i106;
};

class CompatiblePlugin : public Object<CompatiblePluginVars>, public IPlugin
{
public:
	CompatiblePlugin()
	{}
	CompatiblePlugin(Interface<IPluginV101> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
	{
		vars->version = 101;
		vars->i101 = i;
	}
	CompatiblePlugin(Interface<IPluginV102> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
	{
		vars->version = 102;
		vars->i102 = i;
	}
	CompatiblePlugin(Interface<IPluginV103> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
	{
		vars->version = 103;
		vars->i103 = i;
	}
	CompatiblePlugin(Interface<IPluginV104> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
	{
		vars->version = 104;
		vars->i104 = i;
	}
	CompatiblePlugin(Interface<IPluginV105> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
	{
		vars->version = 105;
		vars->i105 = i;
	}
	CompatiblePlugin(Interface<IPluginV106> i) : Object<CompatiblePluginVars>(new CompatiblePluginVars, TRUE)
	{
		vars->version = 106;
		vars->i106 = i;
	}
	String getPluginType() override
	{
		switch (vars->version)
		{
		case 101:
			return vars->i101->getPluginType();
		case 102:
			return vars->i102->getPluginType();
		case 103:
			return vars->i103->getPluginType();
		case 104:
			return vars->i104->getPluginType();
		case 105:
			return vars->i105->getPluginType();
		case 106:
			return vars->i106->getPluginType();
		default:
			return String();
		}
	}
	String getPluginVersion() override
	{
		switch (vars->version)
		{
		case 101:
			return vars->i101->getPluginVersion();
		case 102:
			return vars->i102->getPluginVersion();
		case 103:
			return vars->i103->getPluginVersion();
		case 104:
			return vars->i104->getPluginVersion();
		case 105:
			return vars->i105->getPluginVersion();
		case 106:
			return vars->i106->getPluginVersion();
		default:
			return String();
		}
	}
	void closePlugin() override
	{
		switch (vars->version)
		{
		case 101:
			vars->i101->closePlugin();
			break;
		case 102:
			vars->i102->closePlugin();
			break;
		case 103:
			vars->i103->closePlugin();
			break;
		case 104:
			vars->i104->closePlugin();
			break;
		case 105:
			vars->i105->closePlugin();
			break;
		case 106:
			vars->i106->closePlugin();
			break;
		default:
			break;
		}
	}
};

SPADAS_DEFAULT_API Interface<IPlugin> get_compatible_plugin(Pointer func, UInt minor)
{
	if (!func) return Interface<IPlugin>();
	switch (minor)
	{
	case 0:
	{
		GetPlugin getPluginFunc = (GetPlugin)func;
		return getPluginFunc();
	}
	case 1:
	{
		GetPluginV101 getPluginFunc = (GetPluginV101)func;
		auto i = getPluginFunc();
		if (i.isValid()) return CompatiblePlugin(i);
		else return Interface<IPlugin>();
	}
	case 2:
	{
		GetPluginV102 getPluginFunc = (GetPluginV102)func;
		auto i = getPluginFunc();
		if (i.isValid()) return CompatiblePlugin(i);
		else return Interface<IPlugin>();
	}
	case 3:
	{
		GetPluginV103 getPluginFunc = (GetPluginV103)func;
		auto i = getPluginFunc();
		if (i.isValid()) return CompatiblePlugin(i);
		else return Interface<IPlugin>();
	}
	case 4:
	{
		GetPluginV104 getPluginFunc = (GetPluginV104)func;
		auto i = getPluginFunc();
		if (i.isValid()) return CompatiblePlugin(i);
		else return Interface<IPlugin>();
	}
	case 5:
	{
		GetPluginV105 getPluginFunc = (GetPluginV105)func;
		auto i = getPluginFunc();
		if (i.isValid()) return CompatiblePlugin(i);
		else return Interface<IPlugin>();
	}
	case 6:
	{
		GetPluginV106 getPluginFunc = (GetPluginV106)func;
		auto i = getPluginFunc();
		if (i.isValid()) return CompatiblePlugin(i);
		else return Interface<IPlugin>();
	}
	default:
		return Interface<IPlugin>();
	}
}

// 一般原生插件API 1.0
String IPlugin::getPluginType()
{
	return String();
}

String IPlugin::getPluginVersion()
{
	return String();
}

void IPlugin::closePlugin()
{}

// 通用功能插件接口 1.1
String IPluginV101::getPluginType()
{
	return String();
}

String IPluginV101::getPluginVersion()
{
	return String();
}

void IPluginV101::closePlugin()
{}

void IPluginV101::onCrossData(String id, Binary data)
{}

void IPluginV101::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV101::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV101::useCrossCaller(Interface<ICrossCaller> caller)
{}

// 通用功能插件接口 1.2
String IPluginV102::getPluginType()
{
	return String();
}

String IPluginV102::getPluginVersion()
{
	return String();
}

void IPluginV102::closePlugin()
{}

void IPluginV102::onCrossData(String id, Binary data)
{}

void IPluginV102::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV102::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV102::useCrossCaller(Interface<ICrossCaller> caller)
{}

void IPluginV102::onStartOnlineSession(SessionIdentifier session, ULong startCPUTick)
{}

void IPluginV102::onStopOnlineSession()
{}

Array<String> IPluginV102::getStandaloneTaskNames()
{
	return Array<String>();
}

void IPluginV102::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

// 通用功能插件接口 1.3
String IPluginV103::getPluginType()
{
	return String();
}

String IPluginV103::getPluginVersion()
{
	return String();
}

void IPluginV103::closePlugin()
{}

void IPluginV103::initLanguage(String languageCode)
{}

void IPluginV103::onCrossData(String id, Binary data)
{}

void IPluginV103::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV103::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV103::useCrossCaller(Interface<ICrossCaller> caller)
{}

void IPluginV103::onStartOnlineSession(SessionIdentifier session, ULong startCPUTick)
{}

void IPluginV103::onStopOnlineSession()
{}

Array<String> IPluginV103::getStandaloneTaskNames()
{
	return Array<String>();
}

void IPluginV103::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

// 通用功能插件接口 1.4
String IPluginV104::getPluginType()
{
	return String();
}

String IPluginV104::getPluginVersion()
{
	return String();
}

void IPluginV104::closePlugin()
{}

void IPluginV104::initLanguage(String languageCode)
{}

void IPluginV104::useLogger(Interface<ILogger> logger)
{}

void IPluginV104::onCrossData(String id, Binary data)
{}

void IPluginV104::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV104::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV104::useCrossCaller(Interface<ICrossCaller> caller)
{}

void IPluginV104::onStartOnlineSession(SessionIdentifier session, ULong startCPUTick)
{}

void IPluginV104::onStopOnlineSession()
{}

Array<String> IPluginV104::getStandaloneTaskNames()
{
	return Array<String>();
}

void IPluginV104::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

// 通用功能插件接口 1.5
String IPluginV105::getPluginType()
{
	return String();
}

String IPluginV105::getPluginVersion()
{
	return String();
}

void IPluginV105::closePlugin()
{}

void IPluginV105::initLanguage(String languageCode)
{}

void IPluginV105::useLogger(Interface<ILogger> logger)
{}

void IPluginV105::setAppFilesPath(Path path)
{}

void IPluginV105::onCrossData(String id, Binary data)
{}

void IPluginV105::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV105::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV105::useCrossCaller(Interface<ICrossCaller> caller)
{}

void IPluginV105::onStartOnlineSession(SessionIdentifier session, ULong startCPUTick)
{}

void IPluginV105::onStopOnlineSession()
{}

Array<String> IPluginV105::getStandaloneTaskNames()
{
	return Array<String>();
}

void IPluginV105::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

Array<String> IPluginV105::getGuestSyncChannelNames()
{
	return Array<String>();
}

// 通用功能插件接口 1.6
String IPluginV106::getPluginType()
{
	return String();
}

String IPluginV106::getPluginVersion()
{
	return String();
}

void IPluginV106::closePlugin()
{}

Dictionary<String> IPluginV106::getThirdPartyNotices()
{
	return Dictionary<String>();
}

void IPluginV106::initLocaleInfo(String languageCode, Bool preferPRCWeb)
{}

void IPluginV106::useLogger(Interface<ILogger> logger)
{}

void IPluginV106::setAppFilesPath(Path path)
{}

void IPluginV106::onCrossData(String id, Binary data)
{}

void IPluginV106::useCrossTransmitter(Interface<ICrossTransmitter> transmitter)
{}

Bool IPluginV106::onCrossCall(String id, BaseObject context)
{
	return FALSE;
}

void IPluginV106::useCrossCaller(Interface<ICrossCaller> caller)
{}

void IPluginV106::onStartOnlineSession(SessionIdentifier session, ULong startCPUTick)
{}

void IPluginV106::onStopOnlineSession()
{}

Array<String> IPluginV106::getStandaloneTaskNames()
{
	return Array<String>();
}

void IPluginV106::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

Array<String> IPluginV106::getGuestSyncChannelNames()
{
	return Array<String>();
}

// 一般设备插件接口 2.0
void IDevicePluginV200::setDeviceConnection(String config)
{}

void IDevicePluginV200::disconnectDevice()
{}

GeneralDeviceStatus IDevicePluginV200::getDeviceStatus(String& info)
{
	return GeneralDeviceStatus::NotConnect;
}

Array<GeneralDeviceStatus> IDevicePluginV200::getChildDeviceStatus()
{
	return Array<GeneralDeviceStatus>();
}

void IDevicePluginV200::resetDeviceDataStream(SessionID session, Timer sync, String etcRoot)
{}

void IDevicePluginV200::closeDeviceDataStream()
{}

RawDataTable IDevicePluginV200::getDeviceNewData()
{
	return RawDataTable();
}

void IDevicePluginV200::useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter)
{}

// 一般设备插件接口 2.1
void IDevicePluginV201::setDeviceConnection(String config)
{}

void IDevicePluginV201::disconnectDevice()
{}

GeneralDeviceStatus IDevicePluginV201::getDeviceStatus(String& info)
{
	return GeneralDeviceStatus::NotConnect;
}

Array<GeneralDeviceStatus> IDevicePluginV201::getChildDeviceStatus()
{
	return Array<GeneralDeviceStatus>();
}

void IDevicePluginV201::resetDeviceDataStream(SessionID session, Timer sync, Path inputRoot, Bool eventMode)
{}

void IDevicePluginV201::closeDeviceDataStream()
{}

RawDataTable IDevicePluginV201::getDeviceNewData()
{
	return RawDataTable();
}

void IDevicePluginV201::useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter)
{}

OptionalDouble IDevicePluginV201::getBufferFilesLatestTime(SessionID session)
{
	return 0;
}

void IDevicePluginV201::clearBufferFiles(SessionID session, Double freeTime)
{}

void IDevicePluginV201::pickEvent(SessionID srcSession, PickConfig pick, Interface<IStandaloneTaskCallback> callback)
{}

// 一般设备插件接口 2.2
void IDevicePluginV202::setDeviceConnection(String config)
{}

void IDevicePluginV202::disconnectDevice()
{}

GeneralDeviceStatus IDevicePluginV202::getDeviceStatus(String& info)
{
	return GeneralDeviceStatus::NotConnect;
}

Array<GeneralDeviceStatus> IDevicePluginV202::getChildDeviceStatus()
{
	return Array<GeneralDeviceStatus>();
}

Array<GeneralDeviceData> IDevicePluginV202::getDeviceNewData()
{
	return Array<GeneralDeviceData>();
}

Array<String> IDevicePluginV202::getTransmittableProtocols()
{
	return Array<String>();
}

Array<String> IDevicePluginV202::getScheduledTransmittableProtocols()
{
	return Array<String>();
}

Bool IDevicePluginV202::transmitGeneralData(String protocol, Array<Double> vector, Binary binary)
{
	return FALSE;
}

Bool IDevicePluginV202::transmitGeneralDataScheduled(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

// 总线设备插件接口 2.0
Array<BusDeviceInfo> IBusPluginV200::getBusDeviceList()
{
	return Array<BusDeviceInfo>();
}

Bool IBusPluginV200::openBusDevice(Array<BusDeviceConfig> configs, Timer sync)
{
	return FALSE;
}

void IBusPluginV200::closeBusDevice()
{}

Bool IBusPluginV200::receiveBusMessage(BusRawData& rxData)
{
	return FALSE;
}

Bool IBusPluginV200::transmitBusMessage(BusRawData txData)
{
	return FALSE;
}

void IBusPluginV200::setBusExtraConfig(String extra)
{}

Array<BusChannelPayload> IBusPluginV200::getBusPayload()
{
	return Array<BusChannelPayload>();
}

// 总线设备插件接口 2.1
Array<BusDeviceInfoX> IBusPluginV201::getBusDeviceList()
{
	return Array<BusDeviceInfoX>();
}

Bool IBusPluginV201::openBusDevice(Array<BusDeviceConfig> configs)
{
	return FALSE;
}

void IBusPluginV201::closeBusDevice()
{}

Bool IBusPluginV201::receiveBusMessage(BusDeviceData& rxData)
{
	return FALSE;
}

Bool IBusPluginV201::transmitBusMessage(UInt channel, UInt id, Binary binary)
{
	return FALSE;
}

Bool IBusPluginV201::transmitBusMessageScheduled(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

void IBusPluginV201::setBusExtraConfig(String extra)
{}

Array<BusChannelPayload> IBusPluginV201::getBusPayload()
{
	return Array<BusChannelPayload>();
}

// 视频设备插件接口 4.0
Array<VideoDeviceInfo> IVideoPluginV400::getVideoDeviceList()
{
	return Array<VideoDeviceInfo>();
}

Bool IVideoPluginV400::openVideoDevice(Array<VideoDeviceConfig> configs, Timer sync)
{
	return FALSE;
}

void IVideoPluginV400::closeVideoDevice()
{}

Bool IVideoPluginV400::queryVideoFrame(VideoRawData& frame)
{
	return FALSE;
}

void IVideoPluginV400::setVideoExtraConfig(String extra)
{}

RawDataTable IVideoPluginV400::getVideoDeviceNewData(SessionID session)
{
	return RawDataTable();
}

void IVideoPluginV400::useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress)
{}

// 视频设备插件接口 4.1
Array<VideoDeviceInfo> IVideoPluginV401::getVideoDeviceList()
{
	return Array<VideoDeviceInfo>();
}

Bool IVideoPluginV401::openVideoDevice(Array<VideoDeviceConfig> configs, Timer sync)
{
	return FALSE;
}

void IVideoPluginV401::closeVideoDevice()
{}

Bool IVideoPluginV401::queryVideoFrame(VideoRawData& frame)
{
	return FALSE;
}

void IVideoPluginV401::setVideoExtraConfig(String extra)
{}

RawDataTable IVideoPluginV401::getVideoDeviceNewData(SessionID session)
{
	return RawDataTable();
}

void IVideoPluginV401::useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress)
{}

Array<String> IVideoPluginV401::getExclusiveKeywords()
{
	return Array<String>();
}

// 视频设备插件接口 4.2
Array<VideoDeviceInfoX> IVideoPluginV402::getVideoDeviceList()
{
	return Array<VideoDeviceInfoX>();
}

Bool IVideoPluginV402::openVideoDevice(Array<VideoDeviceConfigX> configs)
{
	return FALSE;
}

void IVideoPluginV402::closeVideoDevice()
{}

Bool IVideoPluginV402::queryVideoFrame(VideoDeviceData& frame)
{
	return FALSE;
}

Bool IVideoPluginV402::transmitVideoFrame(UInt channel, VideoDataCodec codec, Size2D size, Binary data)
{
	return FALSE;
}

Bool IVideoPluginV402::transmitVideoFrameScheduled(UInt channel, VideoDataCodec codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

void IVideoPluginV402::setVideoExtraConfig(String extra)
{}

Array<GeneralDeviceData> IVideoPluginV402::getVideoDeviceNewData()
{
	return Array<GeneralDeviceData>();
}

void IVideoPluginV402::useVideoPreviewExpress(Interface<IVideoPreviewExpressX> previewExpress)
{}

Array<String> IVideoPluginV402::getExclusiveKeywords()
{
	return Array<String>();
}

// 视频设备插件接口 4.3
Array<VideoDeviceInfoX2> IVideoPluginV403::getVideoDeviceList()
{
	return Array<VideoDeviceInfoX2>();
}

Bool IVideoPluginV403::openVideoDevice(Array<VideoDeviceConfigX> configs)
{
	return FALSE;
}

void IVideoPluginV403::closeVideoDevice()
{}

Bool IVideoPluginV403::queryVideoFrame(VideoDeviceData& frame)
{
	return FALSE;
}

Bool IVideoPluginV403::transmitVideoFrame(UInt channel, VideoDataCodec codec, Size2D size, Binary data)
{
	return FALSE;
}

Bool IVideoPluginV403::transmitVideoFrameScheduled(UInt channel, VideoDataCodec codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance)
{
	return FALSE;
}

void IVideoPluginV403::setVideoExtraConfig(String extra)
{}

Array<GeneralDeviceData> IVideoPluginV403::getVideoDeviceNewData()
{
	return Array<GeneralDeviceData>();
}

void IVideoPluginV403::useVideoPreviewExpress(Interface<IVideoPreviewExpressX> previewExpress)
{}

Array<String> IVideoPluginV403::getExclusiveKeywords()
{
	return Array<String>();
}

// 原生数据处理插件接口 6.0
Bool IProcessorPluginV600::isDataStreamModeSupported()
{
	return FALSE;
}

Bool IProcessorPluginV600::isUsingSetProcessorConfigX()
{
	return FALSE;
}

Bool IProcessorPluginV600::isProcessorOnlineOnly()
{
	return FALSE;
}

Bool IProcessorPluginV600::isProcessorOfflineOnly()
{
	return FALSE;
}

void IProcessorPluginV600::setProcessorConfig(String config)
{}

void IProcessorPluginV600::setProcessorConfigX(String config, Bool onlineMode, Bool recordMode, Dictionary<Path> etcRoots)
{}

void IProcessorPluginV600::disableProcessor()
{}

void IProcessorPluginV600::processData(InputTables inputs, SampleBufferTable sampleBuffers, OutputTables outputs)
{}

void IProcessorPluginV600::useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter)
{}

void IProcessorPluginV600::updateStartTimeLocal(SessionID session, ULong posixTime, Double timeRatio)
{}

void IProcessorPluginV600::updateStartTimeUTC(SessionID session, ULong posixTime, Double timeRatio)
{}

Bool IProcessorPluginV600::isStandaloneTaskModeSupported()
{
	return FALSE;
}

void IProcessorPluginV600::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

// 原生数据处理插件接口 6.1
Bool IProcessorPluginV601::isDataStreamModeSupported()
{
	return FALSE;
}

Bool IProcessorPluginV601::isUsingSetProcessorConfigX()
{
	return FALSE;
}

Bool IProcessorPluginV601::isProcessorOnlineOnly()
{
	return FALSE;
}

Bool IProcessorPluginV601::isProcessorOfflineOnly()
{
	return FALSE;
}

void IProcessorPluginV601::setProcessorConfig(String config)
{}

void IProcessorPluginV601::setProcessorConfigX(String config, Bool onlineMode, Bool recordMode, Map<SessionID, Path> inputRoots)
{}

void IProcessorPluginV601::disableProcessor()
{}

void IProcessorPluginV601::processData(InputTables inputs, SampleBufferTable sampleBuffers, OutputTables outputs)
{}

void IProcessorPluginV601::useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter)
{}

void IProcessorPluginV601::updateStartTimeLocal(SessionID session, ULong posixTime, Double timeRatio)
{}

void IProcessorPluginV601::updateStartTimeUTC(SessionID session, ULong posixTime, Double timeRatio)
{}

Bool IProcessorPluginV601::isStandaloneTaskModeSupported()
{
	return FALSE;
}

void IProcessorPluginV601::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

// 原生数据处理插件接口 6.2
Bool IProcessorPluginV602::isProcessorOnlineOnly()
{
	return FALSE;
}

Bool IProcessorPluginV602::isProcessorOfflineOnly()
{
	return FALSE;
}

void IProcessorPluginV602::setProcessorConfig(String config, Bool onlineMode)
{}

void IProcessorPluginV602::disableProcessor()
{}

void IProcessorPluginV602::processData(InputTablesX inputs, SessionSampleBufferTable sampleBuffers, OutputTablesX outputs)
{}

void IProcessorPluginV602::useTimeServer(Interface<ITimeServer> timeServer)
{}

void IProcessorPluginV602::useGeneralTransmitter(Interface<IGeneralDataTransmitter> generalTransmitter)
{}

void IProcessorPluginV602::useBusTransmitter(Interface<IBusMessageTransmitter> busTransmitter)
{}

void IProcessorPluginV602::useVideoTransmitter(Interface<IVideoFrameTransmitter> videoTransmitter)
{}

// 原生数据处理插件接口 6.3
Bool IProcessorPluginV603::isProcessorOnlineOnly()
{
	return FALSE;
}

Bool IProcessorPluginV603::isProcessorOfflineOnly()
{
	return FALSE;
}

void IProcessorPluginV603::setProcessorConfig(String config, Bool onlineMode)
{}

void IProcessorPluginV603::disableProcessor()
{}

void IProcessorPluginV603::processData(InputTablesX inputs, SessionSampleBufferTable sampleBuffers, OutputTablesX outputs)
{}

void IProcessorPluginV603::useTimeServer(Interface<ITimeServer> timeServer)
{}

void IProcessorPluginV603::useGeneralTransmitter(Interface<IGeneralDataTransmitterX> generalTransmitter)
{}

void IProcessorPluginV603::useBusTransmitter(Interface<IBusMessageTransmitterX> busTransmitter)
{}

void IProcessorPluginV603::useVideoTransmitter(Interface<IVideoFrameTransmitterX> videoTransmitter)
{}

// 文件读写插件接口 1.0
Double IFilePluginV100::getFilesDuration(String readerName, Path inputRoot, Array<Path> generationRoots, FileIOBasicInfo basicInfo)
{
	return 0;
}

Bool IFilePluginV100::openReadFiles(String readerName, Path inputRoot, Path generationRoot, Double timeOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo)
{
	return FALSE;
}

Bool IFilePluginV100::readFilesData(String readerName, InputTables inputs, Double targetTime)
{
	return FALSE;
}

void IFilePluginV100::closeReadFiles(String readerName)
{}

Bool IFilePluginV100::openWriteFiles(String writerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo)
{
	return FALSE;
}

void IFilePluginV100::writeFilesData(String writerName, InputTables inputs, Array<BusRawData> busMessages)
{}

void IFilePluginV100::closeWriteFiles(String writerName)
{}

Bool IFilePluginV100::hasDataFiles(String pickerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo)
{
	return FALSE;
}

void IFilePluginV100::pickSession(String pickerName, Path inputRoot, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

void IFilePluginV100::setFileExtraConfig(String extra)
{}

void IFilePluginV100::updateStartTimeLocal(ULong posixTime, Double timeRatio)
{}

void IFilePluginV100::updateStartTimeUTC(ULong posixTime, Double timeRatio)
{}

// 文件读写插件接口 1.1
Double IFilePluginV101::getFilesDuration(String readerName, Path inputRoot, Array<Path> generationRoots, FileIOBasicInfo basicInfo)
{
	return 0;
}

Bool IFilePluginV101::openReadFiles(String readerName, Path inputRoot, Path generationRoot, Double timeOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo)
{
	return FALSE;
}

Bool IFilePluginV101::readFilesData(String readerName, InputTables inputs, Double targetTime, Flag shouldEnd)
{
	return FALSE;
}

void IFilePluginV101::closeReadFiles(String readerName)
{}

Bool IFilePluginV101::openWriteFiles(String writerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo)
{
	return FALSE;
}

void IFilePluginV101::writeFilesData(String writerName, InputTables inputs, Array<BusRawData> busMessages, Flag shouldEnd)
{}

void IFilePluginV101::closeWriteFiles(String writerName)
{}

Bool IFilePluginV101::hasDataFiles(String pickerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo)
{
	return FALSE;
}

void IFilePluginV101::pickSession(String pickerName, Path inputRoot, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

void IFilePluginV101::setFileExtraConfig(String extra)
{}

void IFilePluginV101::updateStartTimeLocal(ULong posixTime, Double timeRatio)
{}

void IFilePluginV101::updateStartTimeUTC(ULong posixTime, Double timeRatio)
{}

// 文件读写插件接口 1.2
Double IFilePluginV102::getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo)
{
	return 0;
}

Bool IFilePluginV102::openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double timeOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo)
{
	return FALSE;
}

Bool IFilePluginV102::readFilesData(String readerName, InputTables inputs, Double targetTime, Flag shouldEnd)
{
	return FALSE;
}

void IFilePluginV102::closeReadFiles(String readerName)
{}

Bool IFilePluginV102::openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo)
{
	return FALSE;
}

void IFilePluginV102::writeFilesData(String writerName, InputTables inputs, Array<BusRawData> busMessages, Flag shouldEnd)
{}

void IFilePluginV102::closeWriteFiles(String writerName)
{}

Bool IFilePluginV102::hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo)
{
	return FALSE;
}

void IFilePluginV102::pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

void IFilePluginV102::setFileExtraConfig(String extra)
{}

void IFilePluginV102::updateStartTimeLocal(ULong posixTime, Double timeRatio)
{}

void IFilePluginV102::updateStartTimeUTC(ULong posixTime, Double timeRatio)
{}

// 文件读写插件接口 1.3
Double IFilePluginV103::getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo, Interface<ITimeServer> timeServer)
{
	return 0;
}

Bool IFilePluginV103::openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double jumpOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo, Interface<ITimeServer> timeServer)
{
	return FALSE;
}

Bool IFilePluginV103::readFilesData(String readerName, InputTablesX inputs, Double targetOffset, Flag shouldEnd)
{
	return FALSE;
}

void IFilePluginV103::closeReadFiles(String readerName)
{}

Bool IFilePluginV103::openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo, Interface<ITimeServer> timeServer)
{
	return FALSE;
}

void IFilePluginV103::writeFilesData(String writerName, InputTablesX inputs, Array<SessionBusRawData> busMessages, Flag shouldEnd)
{}

void IFilePluginV103::closeWriteFiles(String writerName)
{}

Bool IFilePluginV103::hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo)
{
	return FALSE;
}

void IFilePluginV103::pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback, Interface<ITimeServer> timeServer)
{}

void IFilePluginV103::setFileExtraConfig(String extra)
{}