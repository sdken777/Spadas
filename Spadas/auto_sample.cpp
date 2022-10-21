
#include "spadas.h"

namespace spadas
{
	class SampleBufferVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(SampleBuffer, Vars)

		String protocol;
		Lock lock;
		ListNode<GeneralSample> nodes;
		UInt nSamples;
		Time currentBase;
		Double maxTime;

		SampleBufferVars()
		{
			nodes.joinNext(nodes);
			nSamples = 0;
			maxTime = -100000000;
		}
		~SampleBufferVars()
		{
			nodes.collapse();
		}
	};
	class SessionSampleBufferVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(SessionSampleBuffer, Vars)

		String protocol;
		Lock lock;
		ListNode<SessionGeneralSample> nodes;
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
	};

	inline ULong getSampleTypeTime(FullTimestamp& timestamp, TimeType type)
	{
		ULong *times = (ULong*)&timestamp.cpuTick;
		return times[(UInt)type];
	}
}

using namespace spadas;

const String spadas::SampleBuffer::TypeName = "spadas.SampleBuffer";
const String spadas::SessionSampleBuffer::TypeName = "spadas.SessionSampleBuffer";

SampleBuffer::SampleBuffer() : Object<SampleBufferVars>(new SampleBufferVars, TRUE)
{
}
SessionSampleBuffer::SessionSampleBuffer() : Object<SessionSampleBufferVars>(new SessionSampleBufferVars, TRUE)
{
}

void SessionSampleBuffer::setBufferLength(Double length)
{
	vars->bufferLength = math::clamp(length, 1.0, 60.0);
}

void SampleBuffer::setProtocol(String protocol)
{
	vars->protocol = protocol;
}
void SessionSampleBuffer::setProtocol(String protocol)
{
	vars->protocol = protocol;
}

String SampleBuffer::getProtocol(Bool withChannel)
{
	Array<UInt> atIndices = vars->protocol.search("@");
	if (atIndices.isEmpty()) return vars->protocol;
	else
	{
		if (atIndices.size() != 1 || atIndices[0] == 0) return String();
		else return withChannel ? vars->protocol : String(vars->protocol, Region(0, atIndices[0]));
	}
}
String SessionSampleBuffer::getProtocol(Bool withChannel)
{
	Array<UInt> atIndices = vars->protocol.search("@");
	if (atIndices.isEmpty()) return vars->protocol;
	else
	{
		if (atIndices.size() != 1 || atIndices[0] == 0) return String();
		else return withChannel ? vars->protocol : String(vars->protocol, Region(0, atIndices[0]));
	}
}

void SampleBuffer::addSample(GeneralSample sample, UInt maxSize)
{
	vars->lock.enter();
	{
		if (vars->currentBase != sample.timeStamp.base)
		{
			vars->nodes.collapse();
			vars->nodes.joinNext(vars->nodes);
			vars->nodes.insertPrevious(sample);
			vars->nSamples = 1;
			vars->currentBase = sample.timeStamp.base;
			vars->maxTime = sample.timeStamp.offset;
		}
		else
		{
			Double timeOffset = sample.timeStamp.offset;
			if (timeOffset >= vars->maxTime)
			{
				vars->nodes.insertPrevious(sample);
				vars->maxTime = timeOffset;
			}
			else
			{
				ListNode<GeneralSample> targetNode = vars->nodes.previous();
				while (targetNode != vars->nodes)
				{
					if (timeOffset >= targetNode.value().timeStamp.offset)
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
			}
			
			vars->nSamples++;
			while (vars->nSamples > maxSize)
			{
				vars->nodes.removeNext();
				vars->nSamples--;
			}
		}
	}
	vars->lock.leave();
}
void SessionSampleBuffer::addSample(SessionGeneralSample sample)
{
	if (sample.timestamp.offset <= 0) return;

	vars->lock.enter();
	{
		Double timeOffset = sample.timestamp.offset;
		if (vars->currentSession != sample.timestamp.session)
		{
			vars->nodes.collapse();
			vars->nodes.joinNext(vars->nodes);
			vars->nodes.insertPrevious(sample);
			vars->nSamples = 1;
			vars->currentSession = sample.timestamp.session;
			vars->maxTime = timeOffset;
		}
		else if (timeOffset >= vars->maxTime - vars->bufferLength)
		{
			if (timeOffset > vars->maxTime)
			{
				vars->nodes.insertPrevious(sample);
				vars->maxTime = timeOffset;

				Double lowerBound = timeOffset - vars->bufferLength;
				ListNode<SessionGeneralSample> targetNode = vars->nodes.next();
				while (targetNode != vars->nodes)
				{
					if (targetNode->timestamp.offset >= lowerBound) break;
					targetNode.goNext();
					targetNode.removePrevious();
				}
			}
			else
			{
				ListNode<SessionGeneralSample> targetNode = vars->nodes.previous();
				while (targetNode != vars->nodes)
				{
					if (timeOffset >= targetNode->timestamp.offset)
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
			}
			
			vars->nSamples++;
		}
	}
	vars->lock.leave();
}

Bool SampleBuffer::isEmpty()
{
	return vars->nSamples == 0;
}
Bool SessionSampleBuffer::isEmpty()
{
	return vars->nSamples == 0;
}

UInt SampleBuffer::getSampleCount()
{
	return vars->nSamples;
}
UInt SessionSampleBuffer::getSampleCount()
{
	return vars->nSamples;
}

Time SampleBuffer::getCurrentSession()
{
	return vars->currentBase;
}
SessionIdentifier SessionSampleBuffer::getCurrentSession()
{
	return vars->currentSession;
}

Bool SampleBuffer::getEarliest(GeneralSample& sampleEarliest)
{
	LockProxy p(vars->lock);

	if (vars->nSamples == 0) return FALSE;

	sampleEarliest = vars->nodes.next().value();
	return TRUE;
}
Bool SessionSampleBuffer::getEarliest(SessionGeneralSample& sampleEarliest)
{
	LockProxy p(vars->lock);

	if (vars->nSamples == 0) return FALSE;

	sampleEarliest = vars->nodes.next().value();
	return TRUE;
}

Bool SampleBuffer::getLatest(GeneralSample& sampleLatest)
{
	LockProxy p(vars->lock);

	if (vars->nSamples == 0) return FALSE;

	sampleLatest = vars->nodes.previous().value();
	return TRUE;
}
Bool SessionSampleBuffer::getLatest(SessionGeneralSample& sampleLatest)
{
	LockProxy p(vars->lock);

	if (vars->nSamples == 0) return FALSE;

	sampleLatest = vars->nodes.previous().value();
	return TRUE;
}

Bool SampleBuffer::getNearest(GlobalTimestamp time, GeneralSample& sampleNearest)
{
	if (TRUE)
	{
		LockProxy p(vars->lock);

		if (vars->nSamples == 0) return FALSE;
		if (time.base != vars->currentBase) return FALSE;

		if (vars->nSamples == 1)
		{
			sampleNearest = vars->nodes.next().value();
			return TRUE;
		}

		if (time.offset > vars->maxTime)
		{
			sampleNearest = vars->nodes.previous().value();
			return TRUE;
		}

		GeneralSample& firstSample = vars->nodes.next().value();
		Double firstTime = firstSample.timeStamp.offset;
		if (time.offset < firstTime)
		{
			sampleNearest = firstSample;
			return TRUE;
		}
	}

	GeneralSample sampleBefore, sampleAfter;
	if (!search(time, sampleBefore, sampleAfter)) return FALSE;

	if (sampleAfter.timeStamp.offset - time.offset > time.offset - sampleBefore.timeStamp.offset)
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
Bool SessionSampleBuffer::getNearest(Double offset, SessionGeneralSample& sampleNearest)
{
	{
		LockProxy p(vars->lock);

		if (vars->nSamples == 0) return FALSE;

		if (vars->nSamples == 1)
		{
			sampleNearest = vars->nodes.next().value();
			return TRUE;
		}

		if (offset >= vars->maxTime)
		{
			sampleNearest = vars->nodes.previous().value();
			return TRUE;
		}

		SessionGeneralSample& firstSample = vars->nodes.next().value();
		Double firstTime = firstSample.timestamp.offset;
		if (offset <= firstTime)
		{
			sampleNearest = firstSample;
			return TRUE;
		}
	}

	SessionGeneralSample sampleBefore, sampleAfter;
	if (!search(offset, sampleBefore, sampleAfter)) return FALSE;

	if (sampleAfter.timestamp.offset - offset > offset - sampleBefore.timestamp.offset)
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
Bool SessionSampleBuffer::getNearest(TimeType timeType, ULong time, SessionGeneralSample& sampleNearest)
{
	{
		LockProxy p(vars->lock);

		if (vars->nSamples == 0) return FALSE;

		if (vars->nSamples == 1)
		{
			sampleNearest = vars->nodes.next().value();
			return getSampleTypeTime(sampleNearest.timestamp, timeType) != 0;
		}
	}

	SessionGeneralSample sampleBefore, sampleAfter;
	if (!search(timeType, time, sampleBefore, sampleAfter)) return FALSE;

	if (getSampleTypeTime(sampleAfter.timestamp, timeType) - time > time - getSampleTypeTime(sampleBefore.timestamp, timeType))
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

Bool SampleBuffer::getNext(GlobalTimestamp time, GeneralSample& sampleNext)
{
	LockProxy p(vars->lock);

	if (vars->nSamples == 0) return FALSE;
	if (time.base != vars->currentBase) return FALSE;

	ListNode<GeneralSample> targetNode = vars->nodes.previous();
	if (targetNode.value().timeStamp.offset <= time.offset) return FALSE;

	targetNode = targetNode.previous();
	while (targetNode != vars->nodes)
	{
		if (targetNode.value().timeStamp.offset <= time.offset)
		{
			targetNode = targetNode.next();
			break;
		}
		targetNode.goPrevious();
	}

	if (targetNode == vars->nodes) sampleNext = vars->nodes.next().value();
	else sampleNext = targetNode.value();

	return TRUE;
}

Bool SampleBuffer::search(GlobalTimestamp time, GeneralSample& sampleBefore, GeneralSample& sampleAfter)
{
	LockProxy p(vars->lock);

	if (time.base != vars->currentBase) return FALSE;
	if (time.offset > vars->maxTime) return FALSE;
	if (vars->nSamples < 2) return FALSE;

	Double firstTime = vars->nodes.next().value().timeStamp.offset;
	if (time.offset < firstTime) return FALSE;

	if (vars->maxTime - time.offset > time.offset - firstTime) // 从前往后搜
	{
		ListNode<GeneralSample> targetNode = vars->nodes.next().next();
		while (targetNode != vars->nodes)
		{
			if (targetNode.value().timeStamp.offset >= time.offset)
			{
				sampleBefore = targetNode.previous().value();
				sampleAfter = targetNode.value();
				return TRUE;
			}
			targetNode.goNext();
		}
	}
	else // 从后往前搜
	{
		ListNode<GeneralSample> targetNode = vars->nodes.previous().previous();
		while (targetNode != vars->nodes)
		{
			if (targetNode.value().timeStamp.offset <= time.offset)
			{
				sampleBefore = targetNode.value();
				sampleAfter = targetNode.next().value();
				return TRUE;
			}
			targetNode.goPrevious();
		}
	}

	return FALSE;
}
Bool SessionSampleBuffer::search(Double offset, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter)
{
	LockProxy p(vars->lock);

	if (offset > vars->maxTime) return FALSE;
	if (vars->nSamples < 2) return FALSE;

	Double firstTime = vars->nodes.next()->timestamp.offset;
	if (offset < firstTime) return FALSE;

	if (vars->maxTime - offset > offset - firstTime) // 从前往后搜
	{
		ListNode<SessionGeneralSample> targetNode = vars->nodes.next().next();
		while (targetNode != vars->nodes)
		{
			if (targetNode->timestamp.offset >= offset)
			{
				sampleBefore = targetNode.previous().value();
				sampleAfter = targetNode.value();
				return TRUE;
			}
			targetNode.goNext();
		}
	}
	else // 从后往前搜
	{
		ListNode<SessionGeneralSample> targetNode = vars->nodes.previous().previous();
		while (targetNode != vars->nodes)
		{
			if (targetNode->timestamp.offset <= offset)
			{
				sampleBefore = targetNode.value();
				sampleAfter = targetNode.next().value();
				return TRUE;
			}
			targetNode.goPrevious();
		}
	}

	return FALSE;
}
Bool SessionSampleBuffer::search(TimeType timeType, ULong time, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter)
{
	LockProxy p(vars->lock);

	if (vars->nSamples < 2) return FALSE;

	ULong firstTime = 0;
	ListNode<SessionGeneralSample> firstNode = vars->nodes.next();
	while (firstNode != vars->nodes)
	{
		ULong nodeTime = getSampleTypeTime(firstNode->timestamp, timeType);
		if (nodeTime != 0)
		{
			firstTime = nodeTime;
			break;
		}
		firstNode.goNext();
	}
	if (firstNode == vars->nodes) return FALSE;

	ULong lastTime = 0;
	ListNode<SessionGeneralSample> lastNode = vars->nodes.previous();
	while (lastNode != vars->nodes)
	{
		ULong nodeTime = getSampleTypeTime(lastNode->timestamp, timeType);
		if (nodeTime != 0)
		{
			lastTime = nodeTime;
			break;
		}
		lastNode.goPrevious();
	}
	if (lastNode == vars->nodes) return FALSE;

	if (time < firstTime || time > lastTime) return FALSE;

	if (firstTime == lastTime)
	{
		sampleBefore = firstNode.value();
		sampleAfter = lastNode.value();
		return TRUE;
	}

	if (lastTime - time > time - firstTime) // 从前往后搜
	{
		ListNode<SessionGeneralSample> node1 = firstNode;
		ListNode<SessionGeneralSample> node2 = firstNode.next();
		while (node2 != lastNode)
		{
			ULong node2Time = getSampleTypeTime(node2->timestamp, timeType);
			if (node2Time != 0)
			{
				if (node2Time >= time) break;
				node1 = node2;
			}
			node2.goNext();
		}
		sampleBefore = node1.value();
		sampleAfter = node2.value();
		return TRUE;
	}
	else // 从后往前搜
	{
		ListNode<SessionGeneralSample> node1 = lastNode;
		ListNode<SessionGeneralSample> node2 = lastNode.previous();
		while (node2 != firstNode)
		{
			ULong node2Time = getSampleTypeTime(node2->timestamp, timeType);
			if (node2Time != 0)
			{
				if (node2Time <= time) break;
				node1 = node2;
			}
			node2.goPrevious();
		}
		sampleBefore = node2.value();
		sampleAfter = node1.value();
		return TRUE;
	}
}