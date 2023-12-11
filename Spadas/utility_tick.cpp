
#include "oscillator.h"

namespace spadas
{
	Atom tickCount;
	class TickVars : public Vars
    {
    public:
		SPADAS_VARS(Tick, Vars)

		String name;
		Interface<ITickHandler> handler;
        UInt period;
		Flag trigger; // 非自旋
		ULong lastCPUTick; // 自旋
		Bool isSpin;
        TickVars(Bool isSpin) : name((String)"tick_" + tickCount.increase()), period(1), lastCPUTick(0), isSpin(isSpin)
        {
        }
	};
}

using namespace spadas;
using namespace oscillator_internal;

void ITickHandler::onTick(String name)
{
}

Tick::Tick()
{
}

Tick::Tick(String name, Interface<ITickHandler> handler, UInt period)
{
	SPADAS_ERROR_RETURN(handler.isNull());

	setVars(new TickVars(FALSE), TRUE);

	if (!name.isEmpty()) vars->name = name;
	if (handler.isValid()) vars->handler = handler;
	vars->period = math::max(period, 1u);
}

Tick::Tick(String name, Interface<ITickHandler> handler, UInt period, Bool spin)
{
	SPADAS_ERROR_RETURN(handler.isNull());

	setVars(new TickVars(spin), TRUE);

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
UInt Tick::getTimeInterval(UInt threadIndex)
{
	return vars->isSpin ? 0 : 1;
}
Bool Tick::onThreadBegin(UInt threadIndex)
{
	if (vars->isSpin) vars->lastCPUTick = Timer::cpuTick();
	else om.obj()->add(vars->trigger, vars->period, FALSE, TRUE);
    return TRUE;
}
void Tick::onThreadLoop(UInt threadIndex, Flag shouldEnd)
{
	if (vars->isSpin)
	{
		ULong step = Timer::cpuTicksPerSecond() * vars->period / 1000;
		while (Timer::cpuTick() < vars->lastCPUTick + step) {}
		vars->lastCPUTick += step;
	}
	else
	{
		if (!vars->trigger.waitSet(10)) return;
		vars->trigger.reset();
	}
    vars->handler->onTick(vars->name);
}
void Tick::onThreadEnd(UInt threadIndex)
{
	if (!vars->isSpin) om.obj()->remove(vars->trigger);
}
