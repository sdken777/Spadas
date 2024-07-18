
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
	typedef Interface<IPluginV200>(*GetPluginV200)();
}

#endif
