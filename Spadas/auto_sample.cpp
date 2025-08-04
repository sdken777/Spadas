
#include "spadas.h"

namespace spadas
{
	class SessionGeneralSampleVars : public SessionSampleVars
	{
	public:
		SPADAS_VARS(SessionGeneralSample, SessionSampleVars)

		Array<GeneralElement> values;
		UInt significantCount;
	};

	class SessionMatrixSampleVars : public SessionSampleVars
	{
	public:
		SPADAS_VARS(SessionMatrixSample, SessionSampleVars)
		
		Array<Float> matData;
		UInt rows;
		UInt cols;
	};

	class SessionSampleBufferVars : public Vars
	{
	public:
		SPADAS_VARS(SessionSampleBuffer, Vars)

		String protocol;
		ListNode<SessionSample> nodes;
		UInt nSamples;
		SessionIdentifier currentSession;
		Double maxTime; // sec
		Double bufferLength; // sec

		SessionSampleBufferVars() : bufferLength(3.0)
		{
			nodes.joinNext(nodes);
			nSamples = 0;
			maxTime = -100000000;
		}
		~SessionSampleBufferVars()
		{
			nodes.collapse();
		}
		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
	};

	inline ULong getSampleTypeTime(FullTimestamp& timestamp, TimeType::Value type)
	{
		ULong *times = (ULong*)&timestamp.cpuTick;
		return times[(UInt)type];
	}
}

using namespace spadas;

// SessionSample

Bool SessionSampleVars::fromSample(String protocol, SessionSample sample)
{
	return FALSE;
}

Bool SessionSampleVars::supportInterpolation()
{
	return FALSE;
}

SessionSample SessionSampleVars::interpolate(SessionSample& s1, Double w1, SessionSample& s2, Double w2, FullTimestamp timestamp)
{
	return SessionSample();
}

String SessionSampleVars::getTypeName()
{
	return SessionSample::typeName();
}

Bool SessionSampleVars::isType(ULong id)
{
	return id == SessionSample::typeName().getID() || Vars::isType(id);
}

Bool SessionSampleVars::isType(String name)
{
	return name == SessionSample::typeName() || Vars::isType(name);
}

SessionSample::SessionSample() : BaseObject(new SessionSampleVars)
{}

SessionSample::SessionSample(SessionSampleVars *vars) : BaseObject(vars)
{}

FullTimestamp& SessionSample::timestamp()
{
	return var()->timestamp;
}

Bool SessionSample::fromSample(String protocol, SessionSample sample)
{
	return var()->fromSample(protocol, sample);
}

Bool SessionSample::supportInterpolation()
{
	return var()->supportInterpolation();
}

SessionSample SessionSample::interpolate(SessionSample& s1, Double w1, SessionSample& s2, Double w2, FullTimestamp timestamp)
{
	return var()->interpolate(s1, w1, s2, w2, timestamp);
}

// SessionGeneralSample

SessionGeneralSample::SessionGeneralSample() : SessionSample(new SessionGeneralSampleVars)
{
	var()->significantCount = 0;
}

SessionGeneralSample::SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values) : SessionSample(new SessionGeneralSampleVars)
{
	var()->timestamp = timestamp;
	var()->values = values;
	var()->significantCount = 0;
}

SessionGeneralSample::SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values, UInt significantCount) : SessionSample(new SessionGeneralSampleVars)
{
	var()->timestamp = timestamp;
	var()->values = values;
	var()->significantCount = significantCount;
}

FullTimestamp& SessionGeneralSample::timestamp()
{
	return var()->timestamp;
}

Array<GeneralElement>& SessionGeneralSample::values()
{
	return var()->values;
}

UInt& SessionGeneralSample::significantCount()
{
	return var()->significantCount;
}

// SessionMatrixSample

SessionMatrixSample::SessionMatrixSample() : SessionSample(new SessionMatrixSampleVars)
{
	var()->rows = 0;
	var()->cols = 0;
}

SessionMatrixSample::SessionMatrixSample(Size2D size) : SessionSample(new SessionMatrixSampleVars)
{
	var()->matData = Array<Float>(size.dim0 * size.dim1, 0);
	var()->rows = size.dim0;
	var()->cols = size.dim1;
}

FullTimestamp& SessionMatrixSample::timestamp()
{
	return var()->timestamp;
}

Array<Float>& SessionMatrixSample::matData()
{
	return var()->matData;
}

UInt& SessionMatrixSample::rows()
{
	return var()->rows;
}

UInt& SessionMatrixSample::cols()
{
	return var()->cols;
}

// SessionSampleBuffer

SessionSampleBuffer::SessionSampleBuffer() : BaseObject(new SessionSampleBufferVars)
{}

void SessionSampleBuffer::setBufferLength(Double length)
{
	var()->spinEnter();
	var()->bufferLength = math::clamp(length, 1.0, 60.0);
	var()->spinLeave();
}

void SessionSampleBuffer::setProtocol(String protocol)
{
	var()->spinEnter();
	var()->protocol = protocol;
	var()->spinLeave();
}

String SessionSampleBuffer::getProtocol(Bool withChannel)
{
	var()->spinEnter();
	Array<UInt> atIndices = var()->protocol.search("@");
	String protocol;
	if (atIndices.isEmpty()) protocol = var()->protocol;
	else
	{
		if (atIndices.size() != 1 || atIndices[0] == 0) {}
		else protocol = withChannel ? var()->protocol : var()->protocol.span(0, atIndices[0]).clone();
	}
	var()->spinLeave();
	return protocol;
}

void SessionSampleBuffer::addSample(SessionSample sample)
{
	SPADAS_ERROR_RETURN(sample.timestamp().offset <= 0);
	SPADAS_ERROR_RETURN(!sample.is<SessionGeneralSample>() && !sample.is<SessionMatrixSample>());

	var()->spinEnter();
	{
		Double timeOffset = sample.timestamp().offset;
		if (var()->currentSession != sample.timestamp().session)
		{
			var()->nodes.collapse();
			var()->nodes.joinNext(var()->nodes);
			var()->nodes.insertPrevious(sample);
			var()->nSamples = 1;
			var()->currentSession = sample.timestamp().session;
			var()->maxTime = timeOffset;
		}
		else if (timeOffset >= var()->maxTime - var()->bufferLength)
		{
			if (timeOffset > var()->maxTime)
			{
				var()->nodes.insertPrevious(sample);
				var()->maxTime = timeOffset;
				var()->nSamples++;

				Double lowerBound = timeOffset - var()->bufferLength;
				ListNode<SessionSample> targetNode = var()->nodes.next();
				while (targetNode != var()->nodes)
				{
					if (targetNode->timestamp().offset >= lowerBound) break;
					targetNode.goNext();
					targetNode.removePrevious();
					var()->nSamples--;
				}
			}
			else
			{
				ListNode<SessionSample> targetNode = var()->nodes.previous();
				while (targetNode != var()->nodes)
				{
					if (timeOffset >= targetNode->timestamp().offset)
					{
						targetNode.insertNext(sample);
						break;
					}
					targetNode.goPrevious();
				}
				if (targetNode == var()->nodes)
				{
					var()->nodes.insertNext(sample);
				}
				var()->nSamples++;
			}
		}
	}
	var()->spinLeave();
}

Bool SessionSampleBuffer::isEmpty()
{
	var()->spinEnter();
	Bool res = var()->nSamples == 0;
	var()->spinLeave();
	return res;
}

UInt SessionSampleBuffer::getSampleCount()
{
	var()->spinEnter();
	UInt res = var()->nSamples;
	var()->spinLeave();
	return res;
}

SessionIdentifier SessionSampleBuffer::getCurrentSession()
{
	var()->spinEnter();
	auto res = var()->currentSession;
	var()->spinLeave();
	return res;
}

Bool SessionSampleBuffer::getEarliest(SessionSample& sampleEarliest)
{
	var()->spinEnter();
	if (var()->nSamples == 0)
	{
		var()->spinLeave();
		return FALSE;
	}
	else
	{
		sampleEarliest = var()->nodes.next().value();
		var()->spinLeave();
		return TRUE;
	}
}

Bool SessionSampleBuffer::getLatest(SessionSample& sampleLatest)
{
	var()->spinEnter();
	if (var()->nSamples == 0)
	{
		var()->spinLeave();
		return FALSE;
	}
	else
	{
		sampleLatest = var()->nodes.previous().value();
		var()->spinLeave();
		return TRUE;
	}
}

Bool SessionSampleBuffer::getNearest(Double offset, SessionSample& sampleNearest)
{
	var()->spinEnter();

	if (var()->nSamples == 0)
	{
		var()->spinLeave();
		return FALSE;
	}

	if (var()->nSamples == 1)
	{
		sampleNearest = var()->nodes.next().value();
		var()->spinLeave();
		return TRUE;
	}

	if (offset >= var()->maxTime)
	{
		sampleNearest = var()->nodes.previous().value();
		var()->spinLeave();
		return TRUE;
	}

	SessionSample& firstSample = var()->nodes.next().value();
	Double firstTime = firstSample.timestamp().offset;
	if (offset <= firstTime)
	{
		sampleNearest = firstSample;
		var()->spinLeave();
		return TRUE;
	}

	var()->spinLeave();

	SessionSample sampleBefore, sampleAfter;
	if (!search(offset, sampleBefore, sampleAfter)) return FALSE;

	if (sampleAfter.timestamp().offset - offset > offset - sampleBefore.timestamp().offset)
	{
		sampleNearest = sampleBefore;
		return TRUE;
	}
	else
	{
		sampleNearest = sampleAfter;
		return TRUE;
	}
}
Bool SessionSampleBuffer::getNearest(Enum<TimeType> timeType, ULong time, SessionSample& sampleNearest)
{
	var()->spinEnter();

	if (var()->nSamples == 0) 
	{
		var()->spinLeave();
		return FALSE;
	}

	if (var()->nSamples == 1)
	{
		sampleNearest = var()->nodes.next().value();
		var()->spinLeave();
		return getSampleTypeTime(sampleNearest.timestamp(), timeType.value()) != 0;
	}

	var()->spinLeave();

	SessionSample sampleBefore, sampleAfter;
	if (!search(timeType, time, sampleBefore, sampleAfter)) return FALSE;

	if (getSampleTypeTime(sampleAfter.timestamp(), timeType.value()) - time > time - getSampleTypeTime(sampleBefore.timestamp(), timeType.value()))
	{
		sampleNearest = sampleBefore;
		return TRUE;
	}
	else
	{
		sampleNearest = sampleAfter;
		return TRUE;
	}
}

Bool SessionSampleBuffer::search(Double offset, SessionSample& sampleBefore, SessionSample& sampleAfter)
{
	var()->spinEnter();

	if (offset > var()->maxTime || var()->nSamples < 2)
	{
		var()->spinLeave();
		return FALSE;
	}

	Double firstTime = var()->nodes.next()->timestamp().offset;
	if (offset < firstTime)
	{
		var()->spinLeave();
		return FALSE;
	}

	if (var()->maxTime - offset > offset - firstTime) // 从前往后搜
	{
		ListNode<SessionSample> targetNode = var()->nodes.next().next();
		while (targetNode != var()->nodes)
		{
			if (targetNode->timestamp().offset >= offset)
			{
				sampleBefore = targetNode.previous().value();
				sampleAfter = targetNode.value();
				var()->spinLeave();
				return TRUE;
			}
			targetNode.goNext();
		}
	}
	else // 从后往前搜
	{
		ListNode<SessionSample> targetNode = var()->nodes.previous().previous();
		while (targetNode != var()->nodes)
		{
			if (targetNode->timestamp().offset <= offset)
			{
				sampleBefore = targetNode.value();
				sampleAfter = targetNode.next().value();
				var()->spinLeave();
				return TRUE;
			}
			targetNode.goPrevious();
		}
	}

	var()->spinLeave();
	return FALSE;
}
Bool SessionSampleBuffer::search(Enum<TimeType> timeType, ULong time, SessionSample& sampleBefore, SessionSample& sampleAfter)
{
	var()->spinEnter();

	if (var()->nSamples < 2)
	{
		var()->spinLeave();
		return FALSE;
	}

	ULong firstTime = 0;
	ListNode<SessionSample> firstNode = var()->nodes.next();
	while (firstNode != var()->nodes)
	{
		ULong nodeTime = getSampleTypeTime(firstNode->timestamp(), timeType.value());
		if (nodeTime != 0)
		{
			firstTime = nodeTime;
			break;
		}
		firstNode.goNext();
	}
	if (firstNode == var()->nodes)
	{
		var()->spinLeave();
		return FALSE;
	}

	ULong lastTime = 0;
	ListNode<SessionSample> lastNode = var()->nodes.previous();
	while (lastNode != var()->nodes)
	{
		ULong nodeTime = getSampleTypeTime(lastNode->timestamp(), timeType.value());
		if (nodeTime != 0)
		{
			lastTime = nodeTime;
			break;
		}
		lastNode.goPrevious();
	}

	if (lastNode == var()->nodes || time < firstTime || time > lastTime)
	{
		var()->spinLeave();
		return FALSE;
	}

	if (firstTime == lastTime)
	{
		sampleBefore = firstNode.value();
		sampleAfter = lastNode.value();
		var()->spinLeave();
		return TRUE;
	}

	if (lastTime - time > time - firstTime) // 从前往后搜
	{
		ListNode<SessionSample> node1 = firstNode;
		ListNode<SessionSample> node2 = firstNode.next();
		while (node2 != lastNode)
		{
			ULong node2Time = getSampleTypeTime(node2->timestamp(), timeType.value());
			if (node2Time != 0)
			{
				if (node2Time >= time) break;
				node1 = node2;
			}
			node2.goNext();
		}
		sampleBefore = node1.value();
		sampleAfter = node2.value();
		var()->spinLeave();
		return TRUE;
	}
	else // 从后往前搜
	{
		ListNode<SessionSample> node1 = lastNode;
		ListNode<SessionSample> node2 = lastNode.previous();
		while (node2 != firstNode)
		{
			ULong node2Time = getSampleTypeTime(node2->timestamp(), timeType.value());
			if (node2Time != 0)
			{
				if (node2Time <= time) break;
				node1 = node2;
			}
			node2.goPrevious();
		}
		sampleBefore = node2.value();
		sampleAfter = node1.value();
		var()->spinLeave();
		return TRUE;
	}
}