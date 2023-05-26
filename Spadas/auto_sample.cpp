
#include "spadas.h"

namespace spadas
{
	class SessionSampleBufferVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(SessionSampleBuffer, Vars)

		String protocol;
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

const String spadas::SessionSampleBuffer::TypeName = "spadas.SessionSampleBuffer";

SessionSampleBuffer::SessionSampleBuffer() : Object<SessionSampleBufferVars>(new SessionSampleBufferVars, TRUE)
{
}

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
		else protocol = withChannel ? vars->protocol : vars->protocol.subString(0, atIndices[0]);
	}
	vars->spinLeave();
	return protocol;
}

void SessionSampleBuffer::addSample(SessionGeneralSample sample)
{
	if (sample.timestamp.offset <= 0) return;

	vars->spinEnter();
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
				vars->nSamples++;

				Double lowerBound = timeOffset - vars->bufferLength;
				ListNode<SessionGeneralSample> targetNode = vars->nodes.next();
				while (targetNode != vars->nodes)
				{
					if (targetNode->timestamp.offset >= lowerBound) break;
					targetNode.goNext();
					targetNode.removePrevious();
					vars->nSamples--;
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

Bool SessionSampleBuffer::getEarliest(SessionGeneralSample& sampleEarliest)
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

Bool SessionSampleBuffer::getLatest(SessionGeneralSample& sampleLatest)
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

Bool SessionSampleBuffer::getNearest(Double offset, SessionGeneralSample& sampleNearest)
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

	SessionGeneralSample& firstSample = vars->nodes.next().value();
	Double firstTime = firstSample.timestamp.offset;
	if (offset <= firstTime)
	{
		sampleNearest = firstSample;
		vars->spinLeave();
		return TRUE;
	}

	vars->spinLeave();

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
Bool SessionSampleBuffer::getNearest(Enum<TimeType> timeType, ULong time, SessionGeneralSample& sampleNearest)
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
		return getSampleTypeTime(sampleNearest.timestamp, timeType.value()) != 0;
	}

	vars->spinLeave();

	SessionGeneralSample sampleBefore, sampleAfter;
	if (!search(timeType, time, sampleBefore, sampleAfter)) return FALSE;

	if (getSampleTypeTime(sampleAfter.timestamp, timeType.value()) - time > time - getSampleTypeTime(sampleBefore.timestamp, timeType.value()))
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

Bool SessionSampleBuffer::search(Double offset, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter)
{
	vars->spinEnter();

	if (offset > vars->maxTime || vars->nSamples < 2)
	{
		vars->spinLeave();
		return FALSE;
	}

	Double firstTime = vars->nodes.next()->timestamp.offset;
	if (offset < firstTime)
	{
		vars->spinLeave();
		return FALSE;
	}

	if (vars->maxTime - offset > offset - firstTime) // 从前往后搜
	{
		ListNode<SessionGeneralSample> targetNode = vars->nodes.next().next();
		while (targetNode != vars->nodes)
		{
			if (targetNode->timestamp.offset >= offset)
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
		ListNode<SessionGeneralSample> targetNode = vars->nodes.previous().previous();
		while (targetNode != vars->nodes)
		{
			if (targetNode->timestamp.offset <= offset)
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
Bool SessionSampleBuffer::search(Enum<TimeType> timeType, ULong time, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter)
{
	vars->spinEnter();

	if (vars->nSamples < 2)
	{
		vars->spinLeave();
		return FALSE;
	}

	ULong firstTime = 0;
	ListNode<SessionGeneralSample> firstNode = vars->nodes.next();
	while (firstNode != vars->nodes)
	{
		ULong nodeTime = getSampleTypeTime(firstNode->timestamp, timeType.value());
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
	ListNode<SessionGeneralSample> lastNode = vars->nodes.previous();
	while (lastNode != vars->nodes)
	{
		ULong nodeTime = getSampleTypeTime(lastNode->timestamp, timeType.value());
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
		ListNode<SessionGeneralSample> node1 = firstNode;
		ListNode<SessionGeneralSample> node2 = firstNode.next();
		while (node2 != lastNode)
		{
			ULong node2Time = getSampleTypeTime(node2->timestamp, timeType.value());
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
		ListNode<SessionGeneralSample> node1 = lastNode;
		ListNode<SessionGeneralSample> node2 = lastNode.previous();
		while (node2 != firstNode)
		{
			ULong node2Time = getSampleTypeTime(node2->timestamp, timeType.value());
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