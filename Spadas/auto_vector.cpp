
#include "spadas.h"

namespace spadas
{
	typedef String VectorID;
	typedef GeneralRawData Vector;

	struct VectorRecordTask
	{
		Time base;
		Double begin;
		Double end;
		Path folderPath;
	};

	class VectorIOVars : public Vars
	{
	public:
	};

	class SPADAS_API VectorIO : public Object<class VectorIOVars>, public IWorkflow
	{
	public:
		/// 创建矢量表IO
		VectorIO();

		/// 启用CSV文件输入
		void enableFileInput(Path rootFolder, Time base, Double offset, Array<String> bannedStarterList);

		/// 启用CSV文件输入
		void enableFileInputWithDirectPath(Path sessionFolder, Time base, Double offset, Array<String> bannedStarterList);

		/// 禁用CSV文件输入(默认状态)
		void disableFileInput();

		/// 启用CSV文件输出
		void enableFileOutput(Path rootFolder, Time base);

		/// 启用CSV文件输出
		void enableFileOutputWithDirectPath(Path sessionFolder, Time base);

		/// 禁用CSV文件输出(默认状态)
		void disableFileOutput(Time base);

		/// 设置是否启用缓存采集
		void setBufferRecordEnabled(Bool enabled);

		/// 设置CSV文件输入回放目标(用最快速度读取至指定位置)
		void setFileInputReplayTarget(Double offset);

		/// 设置各矢量通道延迟，key为VectorID (0~200ms)
		void setVectorDelays(Dictionary<Double> delays);

		/// 获取返回实际时间与目标时间的gap[秒]
		Double getReplayTimeGap();

		/// 获取输入CSV文件时长
		Double getFileLength();

		/// 是否CSV文件输入完毕
		Bool isFileInputEnd();

		/// 获取矢量ID列表
		Array<VectorID> getVectorIDList();

		/// 获取指定矢量ID的新数据
		Array<Vector> getNewVectors(VectorID id);

		/// 获取所有新数据至表，返回所有队列中的最大长度
		UInt getNewDataToTable(RawDataTable table);

		/// 输出矢量(time单位秒，范围0~无限大)
		void outputVector(VectorID id, Array<Double> vector, Binary binary, Double time);

		/// 输出矢量(time单位秒，范围0~无限大)，旧版兼容
		void outputManually(VectorID id, Array<Double> vector, Binary binary, UInt nDigits, Double time);

		/// 设置缓存记录缓存大小
		void setBufferRecordBufferSize(Double size);

		/// 添加缓存记录任务
		void addBufferRecordTask(VectorRecordTask task);

		/// 准备停止线程 (可选，在Threads::stop前调用)
		void prepareEnd();

		/// 获取最终时间戳[秒] (在Threads::stop后调用)
		Double getFinalTime();

		/// 获取目标CSV文件时长
		static Double getFileLength(Path rootFolder, Time base);

		/// 获取目标CSV文件时长
		static Double getFileLength(Path sessionPath);

	private:
		String getWorkflowName();
		Array<String> getThreadNames();
		Bool onThreadBegin(UInt threadIndex);
		void onThreadLoop(UInt threadIndex, Flag shouldEnd);
		void onThreadEnd(UInt threadIndex);
	};

	class VectorPickerVars : public Vars
	{
	public:
	};

	class SPADAS_API VectorPicker : public Object<class VectorPickerVars>, public IWorkflow
	{
	public:
		/// 创建矢量表数据提取对象
		VectorPicker();

		/// 开始摘取，返回是否成功开始
		Bool start(Path srcRoot, Time srcBase, Range srcRange, Path dstRoot, Time dstBase);

		/// 开始摘取，返回是否成功开始
		Bool startWithDirectPath(Path srcPath, Time srcBase, Range srcRange, Path dstPath, Time dstBase);

		/// 获取当前处理进度，返回是否已结束 [%]
		Bool getProgress(Double& progress);

		/// 强制停止
		void stop();

	private:
		virtual String getWorkflowName();
		virtual Array<String> getThreadNames();
		virtual void onThreadLoop(UInt threadIndex, Flag shouldEnd);
	};
}

using namespace spadas;

VectorIO::VectorIO() : Object<VectorIOVars>(new VectorIOVars, TRUE)
{}

void VectorIO::enableFileInput(Path rootFolder, Time base, Double offset, Array<String> bannedStarterList)
{}

void VectorIO::enableFileInputWithDirectPath(Path sessionFolder, Time base, Double offset, Array<String> bannedStarterList)
{}

void VectorIO::disableFileInput()
{}

void VectorIO::enableFileOutput(Path rootFolder, Time base)
{}

void VectorIO::enableFileOutputWithDirectPath(Path sessionFolder, Time base)
{}

void VectorIO::disableFileOutput(Time base)
{}

void VectorIO::setBufferRecordEnabled(Bool enabled)
{}

void VectorIO::setFileInputReplayTarget(Double offset)
{}

void VectorIO::setVectorDelays(Dictionary<Double> delays)
{}

Double VectorIO::getReplayTimeGap()
{
	return 0;
}

Double VectorIO::getFileLength()
{
	return 0;
}

Bool VectorIO::isFileInputEnd()
{
	return TRUE;
}

Array<VectorID> VectorIO::getVectorIDList()
{
	return Array<VectorID>();
}

Array<Vector> VectorIO::getNewVectors(VectorID id)
{
	return Array<Vector>();
}

UInt VectorIO::getNewDataToTable(RawDataTable table)
{
	return 0;
}

void VectorIO::outputVector(VectorID id, Array<Double> vector, Binary binary, Double time)
{}

void VectorIO::outputManually(VectorID id, Array<Double> vector, Binary binary, UInt nDigits, Double time)
{}

void VectorIO::setBufferRecordBufferSize(Double size)
{}

void VectorIO::addBufferRecordTask(VectorRecordTask task)
{}

void VectorIO::prepareEnd()
{}

Double VectorIO::getFinalTime()
{
	return 0;
}

Double VectorIO::getFileLength(Path rootFolder, Time base)
{
	return 0;
}

Double VectorIO::getFileLength(Path sessionPath)
{
	return 0;
}

String VectorIO::getWorkflowName()
{
	return "";
}
Array<String> VectorIO::getThreadNames()
{
	return Array<String>();
}
Bool VectorIO::onThreadBegin(UInt threadIndex)
{
	return FALSE;
}

void VectorIO::onThreadLoop(UInt threadIndex, Flag shouldEnd)
{}

void VectorIO::onThreadEnd(UInt threadIndex)
{}

VectorPicker::VectorPicker() : Object<VectorPickerVars>(new VectorPickerVars, TRUE)
{}

Bool VectorPicker::start(Path srcRoot, Time srcBase, Range srcRange, Path dstRoot, Time dstBase)
{
	return FALSE;
}

Bool VectorPicker::startWithDirectPath(Path srcPath, Time srcBase, Range srcRange, Path dstPath, Time dstBase)
{
	return FALSE;
}

Bool VectorPicker::getProgress(Double& progress)
{
	return FALSE;
}

void VectorPicker::stop()
{}

String VectorPicker::getWorkflowName()
{
	return "";
}

Array<String> VectorPicker::getThreadNames()
{
	return Array<String>();
}

void VectorPicker::onThreadLoop(UInt threadIndex, Flag shouldEnd)
{}