
#ifndef SPADAS_DEPRECATED_H
#define SPADAS_DEPRECATED_H

namespace spadas
{
	// Plugin related type definition (deprecated) / 插件相关类型定义（旧） //////////////////////////////////////////////////////////////

	// Plugin related utility functions (deprecated) / 插件相关实用功能（旧） //////////////////////////////////////////////////////////////

	// Plugin API (deprecated) / 插件API（旧） /////////////////////////////////////////////////////////
	class SPADAS_API IPluginV200
	{
	public:
		virtual ~IPluginV200() {};
		virtual String getPluginType();
		virtual String getPluginVersion();
		virtual void closePlugin();
		virtual Dictionary<String> getThirdPartyNotices();
		virtual void initLocaleInfo(String languageCode, Bool preferPRCWeb);
		virtual void useLogger(Interface<ILogger> logger);
		virtual void setAppFilesPath(Path path);
		virtual void onCrossData(String id, Binary data);
		virtual void useCrossTransmitter(Interface<ICrossTransmitter> transmitter);
		virtual Bool onCrossCall(String id, BaseObject context);
		virtual void useCrossCaller(Interface<ICrossCaller> caller);
		virtual void onStartOnlineSession(SessionIdentifier session, ULong startCPUTick);
		virtual void onStopOnlineSession();
		virtual Array<String> getStandaloneTaskNames();
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
		virtual Array<String> getGuestSyncChannelNames();
	};

	class SPADAS_API IFilePluginV200
	{
	public:
		virtual ~IFilePluginV200() {};
		virtual Double getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo, Interface<ITimeServer> timeServer);
		virtual Bool openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double jumpOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo, Interface<ITimeServer> timeServer);
		virtual Bool readFilesData(String readerName, InputTables inputs, Double targetOffset, Flag shouldEnd);
		virtual void closeReadFiles(String readerName);
		virtual Bool openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo, Interface<ITimeServer> timeServer);
		virtual void writeFilesData(String writerName, InputTables inputs, Array<SessionBusRawData> busMessages, Flag shouldEnd);
		virtual void closeWriteFiles(String writerName);
		virtual Bool hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo);
		virtual void pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback, Interface<ITimeServer> timeServer);
		virtual void setFileExtraConfig(String extra);
	};
}

#endif
