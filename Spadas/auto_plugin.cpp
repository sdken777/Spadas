
#include "spadas.h"

using namespace spadas;

// 插件版本接口

SPADAS_DEFAULT_API void get_native_plugin_api_version(UInt& major, UInt& minor)
{
	major = 1;
	minor = 1;
}

SPADAS_DEFAULT_API void get_bus_plugin_api_version(UInt& major, UInt& minor)
{
	major = 2;
	minor = 0;
}

SPADAS_DEFAULT_API void get_video_plugin_api_version(UInt& major, UInt& minor)
{
	major = 4;
	minor = 0;
}

SPADAS_DEFAULT_API void get_dev_plugin_api_version(UInt& major, UInt& minor)
{
	major = 2;
	minor = 1;
}

SPADAS_DEFAULT_API void get_proc_plugin_api_version(UInt& major, UInt& minor)
{
	major = 6;
	minor = 1;
}

SPADAS_DEFAULT_API void get_file_plugin_api_version(UInt& major, UInt& minor)
{
	major = 1;
	minor = 2;
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

// 一般原生插件API 1.1
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

// 通用设备插件API 2.0
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

// 通用设备插件API 2.1
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

// 总线设备插件API 2.0
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

// 视频设备插件API 4.0
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

// 视频设备插件API 4.1
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

// 数据处理插件API 6.0
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

// 数据处理插件API 6.1
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

// 文件读写插件API 1.0
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

// 文件读写插件API 1.1
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

// 文件读写插件API 1.2
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