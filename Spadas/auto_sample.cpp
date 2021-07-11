
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
}

using namespace spadas;

const String spadas::SampleBuffer::TypeName = "spadas.SampleBuffer";

SampleBuffer::SampleBuffer() : Object<SampleBufferVars>(new SampleBufferVars, TRUE)
{
}

void SampleBuffer::setProtocol(String protocol)
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

Bool SampleBuffer::isEmpty()
{
	return vars->nSamples == 0;
}

UInt SampleBuffer::getSampleCount()
{
	return vars->nSamples;
}

Time SampleBuffer::getCurrentSession()
{
	return vars->currentBase;
}

Bool SampleBuffer::getEarliest(GeneralSample& sampleEarliest)
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
