
#include "spadas.h"
#include "oscillator.h"

namespace spadas
{
	class GeneralIOObjectVars : public Vars
	{
	public:
		SPADAS_VARS(GeneralIOObject, Vars)

		Binary input;
		Binary output;
	};

	const UInt GENERAL_RAW_OBJECT_LIFE = 9; // seconds

	class GeneralRawObjectVars : public Vars
	{
	public:
		SPADAS_VARS(GeneralRawObject, Vars)

		BaseObject obj;
		ULong startTick;
		ULong endTick;
		MilliPosix startPosix;
		MilliPosix endPosix;

		GeneralRawObjectVars(BaseObject obj) : obj(obj)
		{}
	};
}

using namespace oscillator_internal;

// ITimestampSearch ///////////////////////////////////////////////////////////////////////////

Bool ITimestampSearch::readNextTimestamp(File file, Double& time, ULong& pos)
{
	return FALSE;
}

Range ITimestampSearch::getTailSearchRange()
{
	return Range(100, 10000);
}

File TimestampSearch::search(Path path, Double time, Interface<ITimestampSearch> searcher)
{
	return search(path, time, searcher, Optional<String>());
}

File TimestampSearch::search(Path path, Double time, Interface<ITimestampSearch> searcher, Optional<String> password)
{
	SPADAS_ERROR_RETURNVAL(path.isNull() || path.isFolder() || searcher.isNull(), File());

	ULong fileSize = path.fileSize();
	if (fileSize < 8) return File(); // 小于8字节文件认为是空文件

	File file = password.isValid() ? File::openEncrypted(path, password.value()) : File::openText(path);
	if (!file.isValid()) return File();

	// 确定尾部搜索范围
	Range tailRange = searcher->getTailSearchRange();
	Int tailSmallRange = (Int)math::round(tailRange.lower);
	Int tailLargeRange = (Int)math::round(tailRange.upper);
	if (tailSmallRange <= 0 || tailLargeRange <= 0)
	{
		tailSmallRange = 100;
		tailLargeRange = 10000;
	}
	if (tailLargeRange < tailSmallRange) tailLargeRange = tailSmallRange;

	Int posThreshold = tailSmallRange + tailLargeRange;
	const Double timeThreshold = 0.2; // 秒

	UInt startPos = (UInt)file.position();
	if (fileSize > (ULong)startPos + posThreshold)
	{
		// 获取第一帧数据时间戳
		file.seek(startPos);

		Double firstTS = 0;
		ULong firstPos = 0;
		if (!searcher->readNextTimestamp(file, firstTS, firstPos)) return File();

		if (firstTS >= time)
		{
			file.seek(firstPos);
			return file;
		}

		// 获取最后一帧数据时间戳
		Double lastTS = 0;
		ULong lastPos = 0;
		Bool lastPosFound = FALSE;

		file.seek(fileSize - tailSmallRange);
		while (TRUE)
		{
			Double ts = 0;
			ULong pos = 0;
			if (searcher->readNextTimestamp(file, ts, pos))
			{
				lastTS = ts;
				lastPos = pos;
				lastPosFound = TRUE;
			}
			else break;
		}

		if (!lastPosFound && tailLargeRange > tailSmallRange)
		{
			file.seek(fileSize - tailLargeRange);
			while (TRUE)
			{
				Double ts = 0;
				ULong pos = 0;
				if (searcher->readNextTimestamp(file, ts, pos))
				{
					lastTS = ts;
					lastPos = pos;
					lastPosFound = TRUE;
				}
				else break;
			}
		}

		if (!lastPosFound) return File();
		if (lastTS < time) return File();

		// 二分法缩小范围
		while (TRUE)
		{
			if (lastPos < firstPos + posThreshold)
			{
				file.seek(firstPos);
				break;
			}

			ULong targetPos = (firstPos + lastPos) / 2;
			file.seek(targetPos);

			Double ts = 0;
			ULong pos = 0;
			if (!searcher->readNextTimestamp(file, ts, pos)) return File(); // 理论上不应出现

			if (pos >= lastPos)
			{
				file.seek(firstPos);
				break;
			}

			if (ts == time) // 正好相等，直接返回
			{
				file.seek(pos);
				return file;
			}
			else if (ts < time && ts > time - timeThreshold) // 已经足够接近目标时间戳
			{
				file.seek(pos);
				break;
			}
			else if (ts > time)
			{
				lastTS = ts;
				lastPos = pos;
			}
			else // ts <= time - timeThreshold
			{
				firstTS = ts;
				firstPos = pos;
			}
		}
	}
	else file.seek(startPos); // 文件不大的情况直接从头搜

	while (TRUE)
	{
		Double ts = 0;
		ULong pos = 0;
		if (!searcher->readNextTimestamp(file, ts, pos)) return File();

		if (ts >= time)
		{
			file.seek(pos);
			break;
		}
	}

	return file;
}

Optional<Range> TimestampSearch::getTimeRange(Path path, Interface<ITimestampSearch> searcher)
{
	return getTimeRange(path, searcher, Optional<String>());
}

Optional<Range> TimestampSearch::getTimeRange(Path path, Interface<ITimestampSearch> searcher, Optional<String> password)
{
	SPADAS_ERROR_RETURNVAL(path.isNull() || path.isFolder() || searcher.isNull(), Optional<Range>());

	ULong fileSize = path.fileSize();
	if (fileSize < 8) return Optional<Range>(); // 小于8字节文件认为是空文件

	File file = password.isValid() ? File::openEncrypted(path, password.value()) : File::openText(path);
	if (!file.isValid()) return Optional<Range>();

	// 确定尾部搜索范围
	Range tailRange = searcher->getTailSearchRange();
	Int tailSmallRange = (Int)math::round(tailRange.lower);
	Int tailLargeRange = (Int)math::round(tailRange.upper);
	if (tailSmallRange <= 0 || tailLargeRange <= 0)
	{
		tailSmallRange = 100;
		tailLargeRange = 10000;
	}
	if (tailLargeRange < tailSmallRange) tailLargeRange = tailSmallRange;

	Int posThreshold = tailSmallRange + tailLargeRange;

	UInt startPos = (UInt)file.position();
	if (fileSize > (ULong)startPos + posThreshold)
	{
		// 获取第一帧数据时间戳
		file.seek(startPos);

		Double firstTS = 0;
		ULong firstPos = 0;
		if (!searcher->readNextTimestamp(file, firstTS, firstPos)) return Optional<Range>();

		// 获取最后一帧数据时间戳
		Double lastTS = 0;
		Bool lastPosFound = FALSE;

		file.seek(fileSize - tailSmallRange);
		while (TRUE)
		{
			Double ts = 0;
			ULong pos = 0;
			if (searcher->readNextTimestamp(file, ts, pos))
			{
				lastTS = ts;
				lastPosFound = TRUE;
			}
			else break;
		}

		if (!lastPosFound && tailLargeRange > tailSmallRange)
		{
			file.seek(fileSize - tailLargeRange);
			while (TRUE)
			{
				Double ts = 0;
				ULong pos = 0;
				if (searcher->readNextTimestamp(file, ts, pos))
				{
					lastTS = ts;
					lastPosFound = TRUE;
				}
				else break;
			}
		}

		if (!lastPosFound) return Range((Float)firstTS, (Float)firstTS);

		if (lastTS < firstTS) return Optional<Range>();
		else return Range((Float)firstTS, (Float)lastTS);
	}
	else // 文件不大的情况直接从头搜
	{
		file.seek(startPos);

		Double firstTS = 0;
		ULong firstPos = 0;
		if (!searcher->readNextTimestamp(file, firstTS, firstPos)) return Optional<Range>();

		Double lastTS = firstTS;

		Double ts = 0;
		ULong pos = 0;
		while (searcher->readNextTimestamp(file, ts, pos))
		{
			lastTS = ts;
		}

		if (lastTS < firstTS) return Optional<Range>();
		else return Range((Float)firstTS, (Float)lastTS);
	}
}

// IFlexHandler ///////////////////////////////////////////////////////////////////////////

void IFlexHandler::createData(Pointer data)
{
}
void IFlexHandler::destroyData(Pointer data)
{
}

// GeneralIOObject ///////////////////////////////////////////////////////////////////////////

GeneralIOObject::GeneralIOObject() : Object<GeneralIOObjectVars>(new GeneralIOObjectVars, TRUE)
{
}

Binary GeneralIOObject::getInput()
{
	return vars->input;
}

void GeneralIOObject::setInput(Binary data)
{
	vars->input = data;
}

Binary GeneralIOObject::getOutput()
{
	return vars->output;
}

void GeneralIOObject::setOutput(Binary data)
{
	vars->output = data;
}

// GeneralRawObject ///////////////////////////////////////////////////////////////////////////

GeneralRawObject::GeneralRawObject()
{}

GeneralRawObject::GeneralRawObject(BaseObject obj)
{
	SPADAS_ERROR_RETURN(obj.isNull());

	GeneralRawObjectVars *newVars = new GeneralRawObjectVars(obj);
	newVars->startTick = Timer::cpuTick();
	newVars->startPosix = system::getTimeWithMS().localTimeToPosix();
	newVars->endTick = newVars->startTick + Timer::cpuTicksPerSecond() * GENERAL_RAW_OBJECT_LIFE;
	newVars->endPosix = newVars->startPosix + 1000 * GENERAL_RAW_OBJECT_LIFE;
	setVars(newVars, TRUE);

	om.obj()->delayDelete(obj, DelayDeleteTimeout::TenSeconds);
}

GeneralRawObject::GeneralRawObject(SessionGeneralRawData data)
{
	if (data.binary.size() != 40) return;

	Vars *objVars = (Vars*)(*(ULong*)&data.binary[0]);
	if (!objVars) return;

	ULong startTick = *(ULong*)&data.binary[8];
	ULong endTick = *(ULong*)&data.binary[16];
	MilliPosix startPosix = *(MilliPosix*)&data.binary[24];
	MilliPosix endPosix = *(MilliPosix*)&data.binary[32];
	ULong curTick = Timer::cpuTick();
	MilliPosix curPosix = system::getTimeWithMS().localTimeToPosix();
	if (curTick < startTick || curTick > endTick || curPosix < startPosix || curPosix > endPosix) return;

	Object<Vars> obj = BaseObject::castCreate(objVars);
	GeneralRawObjectVars *newVars = new GeneralRawObjectVars(*(BaseObject*)(&obj));
	newVars->startTick = startTick;
	newVars->startPosix = startPosix;
	newVars->endTick = endTick;
	newVars->endPosix = endPosix;
	setVars(newVars, TRUE);
}

BaseObject GeneralRawObject::getObject()
{
	return vars ? vars->obj : BaseObject();
}

GeneralDeviceData GeneralRawObject::toGeneralDeviceData(ULong cpuTick, String protocol)
{
	SPADAS_ERROR_RETURNVAL(!vars, GeneralDeviceData());

	GeneralDeviceData output;
	output.protocol = protocol;
	output.cpuTick = cpuTick;
	output.binary = Binary(40);

	*(ULong*)&output.binary[0] = (ULong)vars->obj.getVars();
	*(ULong*)&output.binary[8] = vars->startTick;
	*(ULong*)&output.binary[16] = vars->endTick;
	*(ULong*)&output.binary[24] = vars->startPosix;
	*(ULong*)&output.binary[32] = vars->endPosix;

	return output;
}

SessionGeneralRawData GeneralRawObject::toSessionGeneralRawData(FullTimestamp timestamp)
{
	SPADAS_ERROR_RETURNVAL(!vars, SessionGeneralRawData());

	SessionGeneralRawData output;
	output.timestamp = timestamp;
	output.binary = Binary(40);

	*(ULong*)&output.binary[0] = (ULong)vars->obj.getVars();
	*(ULong*)&output.binary[8] = vars->startTick;
	*(ULong*)&output.binary[16] = vars->endTick;
	*(ULong*)&output.binary[24] = vars->startPosix;
	*(ULong*)&output.binary[32] = vars->endPosix;

	return output;
}