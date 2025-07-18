
#include "spadas.h"

namespace utility_internal
{
	using namespace spadas;

    struct CheckpointStatContext
    {
        ULong firstCpuTick;
        ULong lastCpuTick;
        String lastCheckpointName;
        Dictionary<ULong> cpuTickCosts;

        CheckpointStatContext() : firstCpuTick(0), lastCpuTick(0)
        {}
    };
	Dictionary<CheckpointStatContext> statTable;
    Lock statTableLock(TRUE);
}

using namespace spadas;
using namespace utility_internal;

void spadas::utility::addCheckpoint(String statName, String checkpointName)
{
    ULong enterCpuTick = Timer::cpuTick();

    if (checkpointName.isEmpty())
    {
        clearCheckpoint(statName);
        return;
    }

    LockProxy p(statTableLock);
    CheckpointStatContext& context = statTable[statName];

    if (context.lastCpuTick != 0)
    {
        ULong cpuTickCost = enterCpuTick - context.lastCpuTick;
        if (context.cpuTickCosts.contain(context.lastCheckpointName)) context.cpuTickCosts[context.lastCheckpointName] += cpuTickCost;
        else context.cpuTickCosts[context.lastCheckpointName] = cpuTickCost;
    }

    context.lastCheckpointName = checkpointName;

    ULong leaveCpuTick = Timer::cpuTick();
    context.lastCpuTick = leaveCpuTick;
    if (context.firstCpuTick == 0) context.firstCpuTick = leaveCpuTick;
}

CheckpointStat spadas::utility::clearCheckpoint(String statName, Bool reset)
{
    ULong enterCpuTick = Timer::cpuTick();

    LockProxy p(statTableLock);
    CheckpointStatContext& context = statTable[statName];

    if (context.lastCpuTick != 0)
    {
        ULong cpuTickCost = enterCpuTick - context.lastCpuTick;
        if (context.cpuTickCosts.contain(context.lastCheckpointName)) context.cpuTickCosts[context.lastCheckpointName] += cpuTickCost;
        else context.cpuTickCosts[context.lastCheckpointName] = cpuTickCost;
    }

    context.lastCheckpointName = String();
    context.lastCpuTick = 0;

    CheckpointStat stat;
    if (reset && context.firstCpuTick != 0)
    {
        ULong cpuTicksPerSecond = Timer::cpuTicksPerSecond();
        for (auto kvs = context.cpuTickCosts.keyValues().firstElem(); kvs.valid(); ++kvs)
        {
            stat.timeCosts[kvs->key] = (Double)kvs->value / cpuTicksPerSecond;
        }
        stat.totalTime = (Double)(enterCpuTick - context.firstCpuTick) / cpuTicksPerSecond;

        context.cpuTickCosts.clear();
        context.firstCpuTick = 0;
    }
    return stat;
}

String CheckpointStat::toString()
{
    if (totalTime == 0) return String();

    auto kvs = timeCosts.keyValues();
    kvs.sortAs([](auto& a, auto& b) { return a.value < b.value; });

    ArrayX<String> rows;
    for (auto e = kvs.firstElem(); e.valid(); ++e)
    {
        Double percentage = e->value / totalTime * 100;
        String timeInfo;
        if (e->value >= 1) timeInfo = String(e->value, 3) + "s";
        else if (e->value >= 0.001) timeInfo = String(e->value * 1000, 3) + "ms";
        else timeInfo = String(e->value * 1000000, 3) + "us";
        rows.append(String(percentage, 1) + "% : " + e->key + " (" + timeInfo + ")");
    }
    return String::merge(rows.toArray(), '\n');
}