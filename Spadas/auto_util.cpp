
#include "spadas.h"

namespace spadas
{
	class GeneralIOObjectVars : public Vars
	{
	public:
		SPADAS_VARS(GeneralIOObject, Vars)

		Binary input;
		Binary output;
	};
}

using namespace spadas;

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
	SPADAS_ERROR_RETURNVAL(path.isNull() || path.isFolder() || searcher.isNull(), File());

	ULong fileSize = path.fileSize();
	if (fileSize < 8) return File(); // 小于8字节文件认为是空文件

	File file = File::openText(path);
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
	SPADAS_ERROR_RETURNVAL(path.isNull() || path.isFolder() || searcher.isNull(), Optional<Range>());

	ULong fileSize = path.fileSize();
	if (fileSize < 8) return Optional<Range>(); // 小于8字节文件认为是空文件

	File file = File::openText(path);
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

void IFlexHandler::createData(Pointer data)
{
}
void IFlexHandler::destroyData(Pointer data)
{
}

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