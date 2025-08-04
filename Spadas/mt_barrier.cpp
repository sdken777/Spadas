
#include "spadas.h"

namespace spadas
{
	class BarrierVars : public Vars
	{
    public:
		SPADAS_VARS(Barrier, Vars)

		volatile UInt strength;
		volatile UInt nJoined;
		ListNode<Flag> flagCircle0; // the number of elements == nJoined
		Lock lock;
        BarrierVars()
        {
            strength = 0;
            nJoined = 0;
            flagCircle0.joinNext(flagCircle0);
        }
        ~BarrierVars()
        {
            flagCircle0.collapse();
        }
		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
	};
}

using namespace spadas;

Barrier::Barrier() : BaseObject(new BarrierVars)
{
}

void Barrier::setStrength(UInt strength)
{
	var()->lock.enter();
	if (var()->nJoined != 0)
	{
		SPADAS_ERROR_MSG("var()->nJoined != 0");
	}
	else
	{
		var()->strength = strength;
	}
	var()->lock.leave();
}

Bool Barrier::against(Flag interrupt)
{
	LockProxy lock(var()->lock);
	
	if (var()->strength == 0) return TRUE;
	
	if (var()->nJoined + 1 == var()->strength)
	{
		ListNode<Flag> thisNode = var()->flagCircle0.next();
		for (UInt i = 0; i < var()->nJoined; i++)
		{
			thisNode->set();
			thisNode = thisNode.next();
		}
		var()->flagCircle0.collapse();
		var()->flagCircle0.joinNext(var()->flagCircle0);
		var()->nJoined = 0;
		return TRUE;
	}

	Flag flag;
	var()->flagCircle0.insertPrevious(flag);
	var()->nJoined++;
	lock.releaseLock();
	
	while (!interrupt.check())
	{
		if (flag.waitSet(100)) return TRUE;
	}
	return FALSE;
}
