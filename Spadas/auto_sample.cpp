
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

SessionSample::SessionSample() : Object<SessionSampleVars>(new SessionSampleVars, TRUE)
{}

FullTimestamp& SessionSample::timestamp()
{
	return vars->timestamp;
}

Bool SessionSample::fromSample(String protocol, SessionSample sample)
{
	return vars->fromSample(protocol, sample);
}

Bool SessionSample::supportInterpolation()
{
	return vars->supportInterpolation();
}

SessionSample SessionSample::interpolate(SessionSample& s1, Double w1, SessionSample& s2, Double w2, FullTimestamp timestamp)
{
	return vars->interpolate(s1, w1, s2, w2, timestamp);
}

// SessionGeneralSample

SessionGeneralSample::SessionGeneralSample() : Object<SessionGeneralSampleVars>(new SessionGeneralSampleVars, TRUE)
{
	vars->significantCount = 0;
}

SessionGeneralSample::SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values) : Object<SessionGeneralSampleVars>(new SessionGeneralSampleVars, TRUE)
{
	vars->timestamp = timestamp;
	vars->values = values;
	vars->significantCount = 0;
}

SessionGeneralSample::SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values, UInt significantCount) : Object<SessionGeneralSampleVars>(new SessionGeneralSampleVars, TRUE)
{
	vars->timestamp = timestamp;
	vars->values = values;
	vars->significantCount = significantCount;
}

FullTimestamp& SessionGeneralSample::timestamp()
{
	return vars->timestamp;
}

Array<GeneralElement>& SessionGeneralSample::values()
{
	return vars->values;
}

UInt& SessionGeneralSample::significantCount()
{
	return vars->significantCount;
}

// SessionMatrixSample

SessionMatrixSample::SessionMatrixSample() : Object<SessionMatrixSampleVars>(new SessionMatrixSampleVars, TRUE)
{
	vars->rows = 0;
	vars->cols = 0;
}

SessionMatrixSample::SessionMatrixSample(Size2D size) : Object<SessionMatrixSampleVars>(new SessionMatrixSampleVars, TRUE)
{
	vars->matData = Array<Float>(size.dim0 * size.dim1, 0);
	vars->rows = size.dim0;
	vars->cols = size.dim1;
}

FullTimestamp& SessionMatrixSample::timestamp()
{
	return vars->timestamp;
}

Array<Float>& SessionMatrixSample::matData()
{
	return vars->matData;
}

UInt& SessionMatrixSample::rows()
{
	return vars->rows;
}

UInt& SessionMatrixSample::cols()
{
	return vars->cols;
}

// SessionSampleBuffer

SessionSampleBuffer::SessionSampleBuffer() : Object<SessionSampleBufferVars>(new SessionSampleBufferVars, TRUE)
{}

void SessionSampleBuffer::setBufferLength(Double length)
{
	vars->spinEnter();
	vars->bufferLength = math::clamp(length, 1.0, 60.0);
	vars->spinLeave();
}

void SessionSampleBuffer::setProtocol(String protocol)
{
	vars->spinEnter();
	vars->protocol = protocol;
	vars->spinLeave();
}

String SessionSampleBuffer::getProtocol(Bool withChannel)
{
	vars->spinEnter();
	Array<UInt> atIndices = vars->protocol.search("@");
	String protocol;
	if (atIndices.isEmpty()) protocol = vars->protocol;
	else
	{
		if (atIndices.size() != 1 || atIndices[0] == 0) {}
		else protocol = withChannel ? vars->protocol : vars->protocol.span(0, atIndices[0]).clone();
	}
	vars->spinLeave();
	return protocol;
}

void SessionSampleBuffer::addSample(SessionSample sample)
{
	SPADAS_ERROR_RETURN(sample.timestamp().offset <= 0);
	SPADAS_ERROR_RETURN(!sample.is<SessionGeneralSample>() && !sample.is<SessionMatrixSample>());

	vars->spinEnter();
	{
		Double timeOffset = sample.timestamp().offset;
		if (vars->currentSession != sample.timestamp().session)
		{
			vars->nodes.collapse();
			vars->nodes.joinNext(vars->nodes);
			vars->nodes.insertPrevious(sample);
			vars->nSamples = 1;
			vars->currentSession = sample.timestamp().session;
			vars->maxTime = timeOffset;
		}
		else if (timeOffset >= vars->maxTime - vars->bufferLength)
		{
			if (timeOffset > vars->maxTime)
			{
				vars->nodes.insertPrevious(sample);
				vars->maxTime = timeOffset;
				vars->nSamples++;

				Double lowerBound = timeOffset - vars->bufferLength;
				ListNode<SessionSample> targetNode = vars->nodes.next();
				while (targetNode != vars->nodes)
				{
					if (targetNode->timestamp().offset >= lowerBound) break;
					targetNode.goNext();
					targetNode.removePrevious();
					vars->nSamples--;
				}
			}
			else
			{
				ListNode<SessionSample> targetNode = vars->nodes.previous();
				while (targetNode != vars->nodes)
				{
					if (timeOffset >= targetNode->timestamp().offset)
					{
						targetNode.insertNext(sample);
						break;
					}
					targetNode.goPrevious();
				}
				if (targetNode == vars->nodes)
				{
					vars->nodes.insertNext(sample);
				}
				vars->nSamples++;
			}
		}
	}
	vars->spinLeave();
}

Bool SessionSampleBuffer::isEmpty()
{
	vars->spinEnter();
	Bool res = vars->nSamples == 0;
	vars->spinLeave();
	return res;
}

UInt SessionSampleBuffer::getSampleCount()
{
	vars->spinEnter();
	UInt res = vars->nSamples;
	vars->spinLeave();
	return res;
}

SessionIdentifier SessionSampleBuffer::getCurrentSession()
{
	vars->spinEnter();
	auto res = vars->currentSession;
	vars->spinLeave();
	return res;
}

Bool SessionSampleBuffer::getEarliest(SessionSample& sampleEarliest)
{
	vars->spinEnter();
	if (vars->nSamples == 0)
	{
		vars->spinLeave();
		return FALSE;
	}
	else
	{
		sampleEarliest = vars->nodes.next().value();
		vars->spinLeave();
		return TRUE;
	}
}

Bool SessionSampleBuffer::getLatest(SessionSample& sampleLatest)
{
	vars->spinEnter();
	if (vars->nSamples == 0)
	{
		vars->spinLeave();
		return FALSE;
	}
	else
	{
		sampleLatest = vars->nodes.previous().value();
		vars->spinLeave();
		return TRUE;
	}
}

Bool SessionSampleBuffer::getNearest(Double offset, SessionSample& sampleNearest)
{
	vars->spinEnter();

	if (vars->nSamples == 0)
	{
		vars->spinLeave();
		return FALSE;
	}

	if (vars->nSamples == 1)
	{
		sampleNearest = vars->nodes.next().value();
		vars->spinLeave();
		return TRUE;
	}

	if (offset >= vars->maxTime)
	{
		sampleNearest = vars->nodes.previous().value();
		vars->spinLeave();
		return TRUE;
	}

	SessionSample& firstSample = vars->nodes.next().value();
	Double firstTime = firstSample.timestamp().offset;
	if (offset <= firstTime)
	{
		sampleNearest = firstSample;
		vars->spinLeave();
		return TRUE;
	}

	vars->spinLeave();

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
	vars->spinEnter();

	if (vars->nSamples == 0) 
	{
		vars->spinLeave();
		return FALSE;
	}

	if (vars->nSamples == 1)
	{
		sampleNearest = vars->nodes.next().value();
		vars->spinLeave();
		return getSampleTypeTime(sampleNearest.timestamp(), timeType.value()) != 0;
	}

	vars->spinLeave();

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
	vars->spinEnter();

	if (offset > vars->maxTime || vars->nSamples < 2)
	{
		vars->spinLeave();
		return FALSE;
	}

	Double firstTime = vars->nodes.next()->timestamp().offset;
	if (offset < firstTime)
	{
		vars->spinLeave();
		return FALSE;
	}

	if (vars->maxTime - offset > offset - firstTime) // 从前往后搜
	{
		ListNode<SessionSample> targetNode = vars->nodes.next().next();
		while (targetNode != vars->nodes)
		{
			if (targetNode->timestamp().offset >= offset)
			{
				sampleBefore = targetNode.previous().value();
				sampleAfter = targetNode.value();
				vars->spinLeave();
				return TRUE;
			}
			targetNode.goNext();
		}
	}
	else // 从后往前搜
	{
		ListNode<SessionSample> targetNode = vars->nodes.previous().previous();
		while (targetNode != vars->nodes)
		{
			if (targetNode->timestamp().offset <= offset)
			{
				sampleBefore = targetNode.value();
				sampleAfter = targetNode.next().value();
				vars->spinLeave();
				return TRUE;
			}
			targetNode.goPrevious();
		}
	}

	vars->spinLeave();
	return FALSE;
}
Bool SessionSampleBuffer::search(Enum<TimeType> timeType, ULong time, SessionSample& sampleBefore, SessionSample& sampleAfter)
{
	vars->spinEnter();

	if (vars->nSamples < 2)
	{
		vars->spinLeave();
		return FALSE;
	}

	ULong firstTime = 0;
	ListNode<SessionSample> firstNode = vars->nodes.next();
	while (firstNode != vars->nodes)
	{
		ULong nodeTime = getSampleTypeTime(firstNode->timestamp(), timeType.value());
		if (nodeTime != 0)
		{
			firstTime = nodeTime;
			break;
		}
		firstNode.goNext();
	}
	if (firstNode == vars->nodes)
	{
		vars->spinLeave();
		return FALSE;
	}

	ULong lastTime = 0;
	ListNode<SessionSample> lastNode = vars->nodes.previous();
	while (lastNode != vars->nodes)
	{
		ULong nodeTime = getSampleTypeTime(lastNode->timestamp(), timeType.value());
		if (nodeTime != 0)
		{
			lastTime = nodeTime;
			break;
		}
		lastNode.goPrevious();
	}

	if (lastNode == vars->nodes || time < firstTime || time > lastTime)
	{
		vars->spinLeave();
		return FALSE;
	}

	if (firstTime == lastTime)
	{
		sampleBefore = firstNode.value();
		sampleAfter = lastNode.value();
		vars->spinLeave();
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
		vars->spinLeave();
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
		vars->spinLeave();
		return TRUE;
	}
}