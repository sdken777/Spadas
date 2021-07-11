
#include "spadas.h"

#include "oscillator.h"

namespace spadas
{
	Atom tickCount;
	class TickVars : public Vars
    {
    public:
		SPADAS_VARS_DEF(Tick, Vars)

		String name;
		Interface<ITickHandler> handler;
        UInt period;
		Flag trigger;
        TickVars() : name((String)"tick_" + tickCount.increase()), period(1)
        {
        }
	};
}

using namespace spadas;
using namespace spadas_internal;

const String spadas::Tick::TypeName = "spadas.Tick";

void ITickHandler::onTick(String name)
{
}

Tick::Tick()
{
}

Tick::Tick(String name, Interface<ITickHandler> handler, UInt period)
{
	SPADAS_ERROR_RETURN(handler.isNull());

	setVars(new TickVars(), TRUE);

	if (!name.isEmpty()) vars->name = name;
	if (handler.isValid()) vars->handler = handler;
	vars->period = math::max(period, 1u);
}

String Tick::getWorkflowName()
{
	return "tick_workflow";
}
Array<String> Tick::getThreadNames()
{
	return Array<String>::scalar("tick_thread");
}
Bool Tick::onThreadBegin(UInt threadIndex)
{
	om.obj()->add(vars->trigger, vars->period, FALSE, TRUE);
    return TRUE;
}
void Tick::onThreadLoop(UInt threadIndex, Flag shouldEnd)
{
    if (!vars->trigger.waitSet(10)) return;
    vars->trigger.reset();
    vars->handler->onTick(vars->name);
}
void Tick::onThreadEnd(UInt threadIndex)
{
	om.obj()->remove(vars->trigger);
}
