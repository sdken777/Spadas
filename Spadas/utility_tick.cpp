
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

	if (!name.isEmpty()) var()->name = name;
	if (handler.isValid()) var()->handler = handler;
	var()->period = math::max(period, 1u);
}

Tick::Tick(String name, Interface<ITickHandler> handler, UInt period, Bool spin)
{
	SPADAS_ERROR_RETURN(handler.isNull());

	setVars(new TickVars(spin), TRUE);

	if (!name.isEmpty()) var()->name = name;
	if (handler.isValid()) var()->handler = handler;
	var()->period = math::max(period, 1u);
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
	return var()->isSpin ? 0 : 1;
}
Bool Tick::onThreadBegin(UInt threadIndex)
{
	if (var()->isSpin) var()->lastCPUTick = Timer::cpuTick();
	else om.obj()->add(var()->trigger, var()->period, FALSE, TRUE);
    return TRUE;
}
void Tick::onThreadLoop(UInt threadIndex, Flag shouldEnd)
{
	if (var()->isSpin)
	{
		ULong step = Timer::cpuTicksPerSecond() * var()->period / 1000;
		while (Timer::cpuTick() < var()->lastCPUTick + step) {}
		var()->lastCPUTick += step;
	}
	else
	{
		if (!var()->trigger.waitSet(10)) return;
		var()->trigger.reset();
	}
    var()->handler->onTick(var()->name);
}
void Tick::onThreadEnd(UInt threadIndex)
{
	if (!var()->isSpin) om.obj()->remove(var()->trigger);
}
