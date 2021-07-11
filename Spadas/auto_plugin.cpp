
#include "spadas.h"

using namespace spadas;

// 插件版本接口

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
	minor = 0;
}

SPADAS_DEFAULT_API void get_proc_plugin_api_version(UInt& major, UInt& minor)
{
	major = 6;
	minor = 0;
}

// 公共API

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

// 通用设备插件API 2.0
#pragma region device-2.0

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

void IDevicePluginV200::resetDeviceDataStream(Time session, Timer sync, String etcRoot)
{}

void IDevicePluginV200::closeDeviceDataStream()
{}

RawDataTable IDevicePluginV200::getDeviceNewData()
{
	return RawDataTable();
}

void IDevicePluginV200::useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter)
{}

#pragma endregion

// 总线设备插件API 2.0
#pragma region bus-2.0

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

void IBusPluginV200::transmitBusMessage(BusRawData txData)
{}

void IBusPluginV200::setBusExtraConfig(String extra)
{}

Array<BusChannelPayload> IBusPluginV200::getBusPayload()
{
	return Array<BusChannelPayload>();
}

#pragma endregion

// 视频设备插件API 4.0
#pragma region video-4.0

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

RawDataTable IVideoPluginV400::getVideoDeviceNewData(Time session)
{
	return RawDataTable();
}

void IVideoPluginV400::useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress)
{}

#pragma endregion

// 数据处理插件API 6.0
#pragma region proc-6.0

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

void IProcessorPluginV600::setProcessorConfigX(String config, Dictionary<Path> etcRoots)
{}

void IProcessorPluginV600::disableProcessor()
{}

void IProcessorPluginV600::processData(InputTables inputs, SampleBufferTable sampleBuffers, OutputTables outputs)
{}

void IProcessorPluginV600::useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter)
{}

void IProcessorPluginV600::updateStartTimeLocal(Time session, ULong posixTime, Double timeRatio)
{}

void IProcessorPluginV600::updateStartTimeUTC(Time session, ULong posixTime, Double timeRatio)
{}

Bool IProcessorPluginV600::isStandaloneTaskModeSupported()
{
	return FALSE;
}

void IProcessorPluginV600::runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback)
{}

#pragma endregion
