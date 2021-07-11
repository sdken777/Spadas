
#include "spadas.h"

namespace spadas
{
	class BarrierVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(Barrier, Vars)

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
	};
}

using namespace spadas;

const String spadas::Barrier::TypeName = "spadas.Barrier";

Barrier::Barrier() : Object<class BarrierVars>(new BarrierVars(), TRUE)
{
}

void Barrier::setStrength(UInt strength)
{
	vars->lock.enter();
	if (vars->nJoined != 0)
	{
		SPADAS_ERROR_MSG("vars->nJoined != 0");
	}
	else
	{
		vars->strength = strength;
	}
	vars->lock.leave();
}

Bool Barrier::against(Flag interrupt)
{
	LockProxy p(vars->lock);
	
	if (vars->strength == 0) return TRUE;
	
	if (vars->nJoined + 1 == vars->strength)
	{
		ListNode<Flag> thisNode = vars->flagCircle0.next();
		for (UInt i = 0; i < vars->nJoined; i++)
		{
			thisNode.value().set();
			thisNode = thisNode.next();
		}
		vars->flagCircle0.collapse();
		vars->flagCircle0.joinNext(vars->flagCircle0);
		vars->nJoined= 0;
		return TRUE;
	}

	Flag flag;
	vars->flagCircle0.insertPrevious(flag);
	vars->nJoined++;
	p.releaseLock();
	
	while (!interrupt.check())
	{
		if (flag.waitSet(100)) return TRUE;
	}
	return FALSE;
}
