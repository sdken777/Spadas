
#ifndef SPADAS_DEPRECATED_H
#define SPADAS_DEPRECATED_H

namespace spadas
{
	// 数学（旧） //////////////////////////////////////////////////////////////
	namespace math
	{
		template <typename Type>
		void sort(Array<Type> arr);
	}

	// 插件相关类型定义（旧） //////////////////////////////////////////////////////////////
	typedef Time SessionID;
	
	struct GlobalTimestamp
	{
		SessionID base;
		Double offset;
		GlobalTimestamp() : offset(0)
		{}
		GlobalTimestamp(SessionID base, Double offset) : base(base), offset(offset)
		{}
		SPADAS_API Bool operator ==(GlobalTimestamp time);
		SPADAS_API Bool operator !=(GlobalTimestamp time);
		SPADAS_API String toString();
	};
	
	struct Signal
	{
		GlobalTimestamp timestamp;
		Double value;
		Signal() : value(0)
		{}
		Signal(GlobalTimestamp timestamp, Double val) : timestamp(timestamp), value(val)
		{}
	};
	typedef Dictionary<Array<Signal> > SignalTable;

	struct GeneralRawData
	{
		GlobalTimestamp timestamp;
		Array<Double> vector;
		Binary binary;
		GeneralRawData()
		{}
		GeneralRawData(GlobalTimestamp timestamp, Array<Double> vector, Binary binary) : timestamp(timestamp), vector(vector), binary(binary)
		{}
	};
	typedef Dictionary<Array<GeneralRawData> > RawDataTable;

	struct GeneralSample
	{
		GlobalTimestamp timeStamp;
		Array<GeneralElement> values;
		UInt significantCount;
		GeneralSample() : significantCount(0)
		{}
		GeneralSample(GlobalTimestamp timestamp, Array<GeneralElement> values) : timeStamp(timestamp), values(values), significantCount(0)
		{}
		GeneralSample(GlobalTimestamp timestamp, Array<GeneralElement> values, UInt significantCount) : timeStamp(timestamp), values(values), significantCount(significantCount)
		{}
		GeneralSample(SessionGeneralSample sample) : timeStamp(sample.timestamp.session.toTime(), sample.timestamp.offset), values(sample.values), significantCount(sample.significantCount)
		{}
	};
	typedef Dictionary<Array<GeneralSample> > SampleTable;

	class SPADAS_API SampleBuffer : public Object<class SampleBufferVars>
	{
	public:
		static const String TypeName;
		SampleBuffer();
		void setProtocol(String protocol);
		String getProtocol(Bool withChannel);
		void addSample(GeneralSample sample, UInt maxSize = 100);
		Bool isEmpty();
		UInt getSampleCount();
		SessionID getCurrentSession();
		Bool getEarliest(GeneralSample& sampleEarliest);
		Bool getLatest(GeneralSample& sampleLatest);
		Bool getNearest(GlobalTimestamp time, GeneralSample& sampleNearest);
		Bool getNext(GlobalTimestamp time, GeneralSample& sampleNext);
		Bool search(GlobalTimestamp time, GeneralSample& sampleBefore, GeneralSample& sampleAfter);
		template <typename Type>
		SampleInterpolationResult interpolate(GlobalTimestamp time, Type& interpolatedSample, UInt earlyThresh = 1000/* ms */);
	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};
	typedef Dictionary<SampleBuffer> SampleBufferTable;

	struct MatrixSample
	{
		GlobalTimestamp timeStamp;
		Array<Float> matData;
		UInt rows;
		UInt cols;
		MatrixSample() : rows(0), cols(0)
		{}
		MatrixSample(Size2D size)
		{
			matData = Array<Float>(size.dim0 * size.dim1);
			rows = size.dim0;
			cols = size.dim1;
		}
	};
	typedef Dictionary<Array<MatrixSample> > MatrixTable;

	struct BusRawData
	{
		Double ts;
		UInt ch;
		UInt id;
		Binary bin;
		BusRawData() : ts(0), ch(0), id(0)
		{}
		BusRawData(Double ts, UInt ch, UInt id, Binary bin) : ts(ts), ch(ch), id(id), bin(bin)
		{}
	};

	struct BusRawDataTable
	{
		SessionID base;
		Array<Array<BusRawData> > channelDatas;
		BusRawDataTable() : channelDatas(BC_NUM)
		{}
		SPADAS_API void clear();
	};

	class SPADAS_API IBusRawDataTransmitter
	{
	public:
		virtual ~IBusRawDataTransmitter() {};
		virtual void transmitMessage(BusRawData msg, UInt interval);
	};

	struct BusDeviceInfo
	{
		BusDeviceID id;
		Array<BusChannelType> types;
		String description;
	};

	struct BusMessage
	{
		BusMessageInfo info;
		GlobalTimestamp timestamp;
		Binary data;
		BusMessage()
		{}
		BusMessage(BusMessageInfo info, GlobalTimestamp timestamp, Binary data) : info(info), timestamp(timestamp), data(data)
		{}
	};
	typedef Dictionary<Array<BusMessage> > BusMessageTable;

	struct VideoRawData
	{
		Double ts;
		ULong posixUTC;
		ULong posixLocal;
		UInt ch;
		VideoInputMode inputMode;
		Binary data;
		Bool hasPreview;
		ImagePointer preview;
		VideoRawData() : ts(0), posixUTC(0), posixLocal(0), ch(0), hasPreview(FALSE)
		{}
	};

	struct VideoProcData
	{
		GlobalTimestamp timestamp;
		UInt ch;
		ImagePointer data;
		String metaProtocol;
		Array<Double> meta;
		VideoProcData() : ch(0)
		{}
	};

	struct VideoDeviceInfo
	{
		VideoDeviceID id;
		String description;
		Array<VideoInputMode> inputModes;
		VideoDeviceInfo()
		{}
	};

	struct VideoDeviceConfig
	{
		VideoDeviceID id;
		UInt mapChannel;
		VideoInputMode inputMode;
		UInt frameRate;
		VideoDeviceConfig() : mapChannel(0), frameRate(0)
		{}
	};

	struct VideoRawDataTable
	{
		SessionID base;
		Array<Array<VideoRawData> > channelDatas;
		SPADAS_API VideoRawDataTable();
		SPADAS_API void clear();
		SPADAS_API Bool isEmpty();
	};

	struct VideoProcDataTable
	{
		Array<Array<VideoProcData> > channelDatas;
		SPADAS_API VideoProcDataTable();
		SPADAS_API void clear();
	};

	class SPADAS_API IVideoPreviewExpress
	{
	public:
		virtual ~IVideoPreviewExpress() {};
		virtual void outputPreview(Double ts, UInt ch, ImagePointer preview);
	};

	struct InputTables
	{
		RawDataTable rawDatas;
		BusRawDataTable busRawDatas;
		VideoRawDataTable videoRawDatas;
		VideoProcDataTable videoProcDatas;
		BusMessageTable busMessages;
		SignalTable signals;
		SampleTable samples;
		MatrixTable matrices;
		InputTables()
		{}
		inline void clear()
		{
			rawDatas.clear();
			busRawDatas.clear();
			videoRawDatas.clear();
			videoProcDatas.clear();
			busMessages.clear();
			signals.clear();
			samples.clear();
			matrices.clear();
		}
	};

	struct OutputTables
	{
		SignalTable signals;
		SampleTable samples;
		MatrixTable matrices;
		OutputTables()
		{}
		inline void clear()
		{
			signals.clear();
			samples.clear();
			matrices.clear();
		}
	};

	// 插件相关实用功能（旧） //////////////////////////////////////////////////////////////
	enum class SampleState
	{
		None, 
		First, 
		Normal, 
	};

	class SPADAS_API BusDataSorter : public Object<class BusDataSorterVars>
	{
	public:
		static const String TypeName;
		BusDataSorter();
		struct SortedObject
		{
			Bool signal;
			Int id;
			Int index;
		};
		Int addSignals(Array<Signal> signals);
		Int addBusMessages(Array<BusMessage> messages);
		Bool process(Array<SortedObject>& sorted);
		Bool getSignal(SortedObject obj, Signal& signal);
		Bool getMessage(SortedObject obj, BusMessage& message);
		void reset();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};
	
	class SPADAS_API IGeneralRawDataOutput
	{
	public:
		virtual ~IGeneralRawDataOutput() {};
		virtual void outputGeneralRawData(String protocol, GeneralRawData data);
	};
	
	class SPADAS_API TriggerFilter : public Object<class TriggerFilterVars>
	{
	public:
		static const String TypeName;
		TriggerFilter();
		void setMinInterval(Double sec);
		Bool update(Bool state, GlobalTimestamp ts);
	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// 插件API（旧） /////////////////////////////////////////////////////////
	class SPADAS_API IPlugin
	{
	public:
		virtual ~IPlugin() {};
		virtual String getPluginType();
		virtual String getPluginVersion();
		virtual void closePlugin();
	};
	typedef Interface<IPlugin>(*GetPlugin)();

	class SPADAS_API IDevicePluginV200
	{
	public:
		virtual ~IDevicePluginV200() {};
		virtual void setDeviceConnection(String config);
		virtual void disconnectDevice();
		virtual GeneralDeviceStatus getDeviceStatus(String& info);
		virtual Array<GeneralDeviceStatus> getChildDeviceStatus();
		virtual void resetDeviceDataStream(SessionID session, Timer sync, String etcRoot);
		virtual void closeDeviceDataStream();
		virtual RawDataTable getDeviceNewData();
		virtual void useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter);
	};
	typedef Interface<IDevicePluginV200>(*GetDevicePluginV200)();

	class SPADAS_API IProcessorPluginV600
	{
	public:
		virtual ~IProcessorPluginV600() {};
		virtual Bool isDataStreamModeSupported();
		virtual Bool isUsingSetProcessorConfigX();
		virtual Bool isProcessorOnlineOnly();
		virtual Bool isProcessorOfflineOnly();
		virtual void setProcessorConfig(String config);
		virtual void setProcessorConfigX(String config, Bool onlineMode, Bool recordMode, Dictionary<Path> etcRoots);
		virtual void disableProcessor();
		virtual void processData(InputTables inputs, SampleBufferTable sampleBuffers, OutputTables outputs);
		virtual void useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter);
		virtual void updateStartTimeLocal(SessionID session, ULong posixTime, Double timeRatio);
		virtual void updateStartTimeUTC(SessionID session, ULong posixTime, Double timeRatio);
		virtual Bool isStandaloneTaskModeSupported();
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
	};
	typedef Interface<IProcessorPluginV600>(*GetProcessorPluginV600)();

	class SPADAS_API IFilePluginV100
	{
	public:
		virtual ~IFilePluginV100() {};
		virtual Double getFilesDuration(String readerName, Path inputRoot, Array<Path> generationRoots, FileIOBasicInfo basicInfo);
		virtual Bool openReadFiles(String readerName, Path inputRoot, Path generationRoot, Double timeOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo);
		virtual Bool readFilesData(String readerName, InputTables inputs, Double targetTime);
		virtual void closeReadFiles(String readerName);
		virtual Bool openWriteFiles(String writerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo);
		virtual void writeFilesData(String writerName, InputTables inputs, Array<BusRawData> busMessages);
		virtual void closeWriteFiles(String writerName);
		virtual Bool hasDataFiles(String pickerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo);
		virtual void pickSession(String pickerName, Path inputRoot, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
		virtual void setFileExtraConfig(String extra);
		virtual void updateStartTimeLocal(ULong posixTime, Double timeRatio);
		virtual void updateStartTimeUTC(ULong posixTime, Double timeRatio);
	};
	typedef Interface<IFilePluginV100>(*GetFilePluginV100)();

	class SPADAS_API IFilePluginV101
	{
	public:
		virtual ~IFilePluginV101() {};
		virtual Double getFilesDuration(String readerName, Path inputRoot, Array<Path> generationRoots, FileIOBasicInfo basicInfo);
		virtual Bool openReadFiles(String readerName, Path inputRoot, Path generationRoot, Double timeOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo);
		virtual Bool readFilesData(String readerName, InputTables inputs, Double targetTime, Flag shouldEnd);
		virtual void closeReadFiles(String readerName);
		virtual Bool openWriteFiles(String writerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo);
		virtual void writeFilesData(String writerName, InputTables inputs, Array<BusRawData> busMessages, Flag shouldEnd);
		virtual void closeWriteFiles(String writerName);
		virtual Bool hasDataFiles(String pickerName, Path inputRoot, Path generationRoot, FileIOBasicInfo basicInfo);
		virtual void pickSession(String pickerName, Path inputRoot, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
		virtual void setFileExtraConfig(String extra);
		virtual void updateStartTimeLocal(ULong posixTime, Double timeRatio);
		virtual void updateStartTimeUTC(ULong posixTime, Double timeRatio);
	};
	typedef Interface<IFilePluginV101>(*GetFilePluginV101)();

	class SPADAS_API IVideoPluginV400
	{
	public:
		virtual ~IVideoPluginV400() {};
		virtual Array<VideoDeviceInfo> getVideoDeviceList();
		virtual Bool openVideoDevice(Array<VideoDeviceConfig> configs, Timer sync);
		virtual void closeVideoDevice();
		virtual Bool queryVideoFrame(VideoRawData& frame);
		virtual void setVideoExtraConfig(String extra);
		virtual RawDataTable getVideoDeviceNewData(SessionID session);
		virtual void useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress);
	};
	typedef Interface<IVideoPluginV400>(*GetVideoPluginV400)();

	class SPADAS_API IDevicePluginV201
	{
	public:
		virtual ~IDevicePluginV201() {};
		virtual void setDeviceConnection(String config);
		virtual void disconnectDevice();
		virtual GeneralDeviceStatus getDeviceStatus(String& info);
		virtual Array<GeneralDeviceStatus> getChildDeviceStatus();
		virtual void resetDeviceDataStream(SessionID session, Timer sync, Path inputRoot, Bool eventMode);
		virtual void closeDeviceDataStream();
		virtual RawDataTable getDeviceNewData();
		virtual void useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter);
		virtual OptionalDouble getBufferFilesLatestTime(SessionID session);
		virtual void clearBufferFiles(SessionID session, Double freeTime);
		virtual void pickEvent(SessionID srcSession, PickConfig pick, Interface<IStandaloneTaskCallback> callback);
	};
	typedef Interface<IDevicePluginV201>(*GetDevicePluginV201)();

	class SPADAS_API IBusPluginV200
	{
	public:
		virtual ~IBusPluginV200() {};
		virtual Array<BusDeviceInfo> getBusDeviceList();
		virtual Bool openBusDevice(Array<BusDeviceConfig> configs, Timer sync);
		virtual void closeBusDevice();
		virtual Bool receiveBusMessage(BusRawData& rxData);
		virtual Bool transmitBusMessage(BusRawData txData);
		virtual void setBusExtraConfig(String extra);
		virtual Array<BusChannelPayload> getBusPayload();
	};
	typedef Interface<IBusPluginV200>(*GetBusPluginV200)();

	class SPADAS_API IVideoPluginV401
	{
	public:
		virtual ~IVideoPluginV401() {};
		virtual Array<VideoDeviceInfo> getVideoDeviceList();
		virtual Bool openVideoDevice(Array<VideoDeviceConfig> configs, Timer sync);
		virtual void closeVideoDevice();
		virtual Bool queryVideoFrame(VideoRawData& frame);
		virtual void setVideoExtraConfig(String extra);
		virtual RawDataTable getVideoDeviceNewData(SessionID session);
		virtual void useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress);
		virtual Array<String> getExclusiveKeywords();
	};
	typedef Interface<IVideoPluginV401>(*GetVideoPluginV401)();

	class SPADAS_API IProcessorPluginV601
	{
	public:
		virtual ~IProcessorPluginV601() {};
		virtual Bool isDataStreamModeSupported();
		virtual Bool isUsingSetProcessorConfigX();
		virtual Bool isProcessorOnlineOnly();
		virtual Bool isProcessorOfflineOnly();
		virtual void setProcessorConfig(String config);
		virtual void setProcessorConfigX(String config, Bool onlineMode, Bool recordMode, Map<SessionID, Path> inputRoots);
		virtual void disableProcessor();
		virtual void processData(InputTables inputs, SampleBufferTable sampleBuffers, OutputTables outputs);
		virtual void useBusTransmitter(Interface<IBusRawDataTransmitter> transmitter);
		virtual void updateStartTimeLocal(SessionID session, ULong posixTime, Double timeRatio);
		virtual void updateStartTimeUTC(SessionID session, ULong posixTime, Double timeRatio);
		virtual Bool isStandaloneTaskModeSupported();
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
	};
	typedef Interface<IProcessorPluginV601>(*GetProcessorPluginV601)();

	class SPADAS_API IFilePluginV102
	{
	public:
		virtual ~IFilePluginV102() {};
		virtual Double getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo);
		virtual Bool openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double timeOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo);
		virtual Bool readFilesData(String readerName, InputTables inputs, Double targetTime, Flag shouldEnd);
		virtual void closeReadFiles(String readerName);
		virtual Bool openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo);
		virtual void writeFilesData(String writerName, InputTables inputs, Array<BusRawData> busMessages, Flag shouldEnd);
		virtual void closeWriteFiles(String writerName);
		virtual Bool hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo);
		virtual void pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
		virtual void setFileExtraConfig(String extra);
		virtual void updateStartTimeLocal(ULong posixTime, Double timeRatio);
		virtual void updateStartTimeUTC(ULong posixTime, Double timeRatio);
	};
	typedef Interface<IFilePluginV102>(*GetFilePluginV102)();

	class SPADAS_API IPluginV101
	{
	public:
		virtual ~IPluginV101() {};
		virtual String getPluginType();
		virtual String getPluginVersion();
		virtual void closePlugin();
		virtual void onCrossData(String id, Binary data);
		virtual void useCrossTransmitter(Interface<ICrossTransmitter> transmitter);
		virtual Bool onCrossCall(String id, BaseObject context);
		virtual void useCrossCaller(Interface<ICrossCaller> caller);
	};
	typedef Interface<IPluginV101>(*GetPluginV101)();

	class SPADAS_API IPluginV102
	{
	public:
		virtual ~IPluginV102() {};
		virtual String getPluginType();
		virtual String getPluginVersion();
		virtual void closePlugin();
		virtual void onCrossData(String id, Binary data);
		virtual void useCrossTransmitter(Interface<ICrossTransmitter> transmitter);
		virtual Bool onCrossCall(String id, BaseObject context);
		virtual void useCrossCaller(Interface<ICrossCaller> caller);
		virtual void onStartOnlineSession(SessionIdentifier session, ULong startCPUTick);
		virtual void onStopOnlineSession();
		virtual Array<String> getStandaloneTaskNames();
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
	};
	typedef Interface<IPluginV102>(*GetPluginV102)();

	class SPADAS_API IPluginV103
	{
	public:
		virtual ~IPluginV103() {};
		virtual String getPluginType();
		virtual String getPluginVersion();
		virtual void closePlugin();
		virtual void initLanguage(String languageCode);
		virtual void onCrossData(String id, Binary data);
		virtual void useCrossTransmitter(Interface<ICrossTransmitter> transmitter);
		virtual Bool onCrossCall(String id, BaseObject context);
		virtual void useCrossCaller(Interface<ICrossCaller> caller);
		virtual void onStartOnlineSession(SessionIdentifier session, ULong startCPUTick);
		virtual void onStopOnlineSession();
		virtual Array<String> getStandaloneTaskNames();
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);
	};
	typedef Interface<IPluginV103>(*GetPluginV103)();

	// sort函数的实现 ///////////////////////////////////////////////////////
	template<typename Type>
	void spadas::math::sort(Array<Type> arr)
	{
		arr.sort();
	}

	// SampleBuffer函数的实现 /////////////////////////////////////////////////////////
	template<typename Type>
	SampleInterpolationResult SampleBuffer::interpolate(GlobalTimestamp time, Type& interpolatedSample, UInt earlyThresh)
	{
		if (this->isEmpty()) return SampleInterpolationResult::OutOfRange;
		if (this->getCurrentSession() != time.base) return SampleInterpolationResult::OutOfRange;

		GeneralSample sFirst, sLast;
		this->getEarliest(sFirst);
		this->getLatest(sLast);

		if (time.offset > sFirst.timeStamp.offset && time.offset <= sLast.timeStamp.offset)
		{
			GeneralSample sg1, sg2;
			if (!this->search(time, sg1, sg2)) return SampleInterpolationResult::OutOfRange;

			Double delta = sg2.timeStamp.offset - sg1.timeStamp.offset;
			Double w1 = (sg2.timeStamp.offset - time.offset) / delta;
			Double w2 = (time.offset - sg1.timeStamp.offset) / delta;

			String protocol = getProtocol(FALSE);

			if (!Type::supportInterpolation())
			{
				Bool parseResult;
				if (protocol.isEmpty()) parseResult = interpolatedSample.fromGeneralSample(w1 > w2 ? sg1 : sg2);
				else parseResult = interpolatedSample.fromGeneralSample(protocol, w1 > w2 ? sg1 : sg2);
				return parseResult ? SampleInterpolationResult::NearestInstead : SampleInterpolationResult::ParseError;
			}

			Type s1, s2;
			Bool parseResultS1, parseResultS2;
			if (protocol.isEmpty())
			{
				parseResultS1 = s1.fromGeneralSample(sg1);
				parseResultS2 = s2.fromGeneralSample(sg2);
			}
			else
			{
				parseResultS1 = s1.fromGeneralSample(protocol, sg1);
				parseResultS2 = s2.fromGeneralSample(protocol, sg2);
			}
			if (!parseResultS1 || !parseResultS2) return SampleInterpolationResult::ParseError;

			interpolatedSample = Type::interpolate(s1, w1, s2, w2, time);
			return SampleInterpolationResult::OK;
		}
		else if (time.offset > sLast.timeStamp.offset && time.offset < sLast.timeStamp.offset + 0.001 * earlyThresh)
		{
			return SampleInterpolationResult::TooEarly;
		}
		else
		{
			return SampleInterpolationResult::OutOfRange;
		}
	}
}

#endif
