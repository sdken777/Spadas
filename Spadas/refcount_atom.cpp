
#include "spadas.h"

#if defined(SPADAS_ENV_MACOS)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace spadas
{
	const UInt SPIN_INTERVAL_RANDOM_COUNT = 50;
	const UInt SPIN_CAS_MAX_INTERVAL = 200;
}

using namespace spadas;

Atom::Atom() : val(0)
{
}

Atom::Atom(Int val0) : val(val0)
{
}

Int Atom::get() const
{
	return val;
}
void Atom::set(Int val0) const
{
	*(volatile Int*)&val = val0;
}

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>

Int Atom::increase() const
{
	return (Int)InterlockedIncrement((volatile long*)&val);
}

Int Atom::decrease() const
{
	return (Int)InterlockedDecrement((volatile long*)&val);
}

Int Atom::add(Int number) const
{
	return (Int)InterlockedAdd((volatile long*)&val, (long)number);
}

Bool Atom::cas(Int oldVal, Int newVal) const
{
	return (Int)InterlockedCompareExchange((volatile long*)&val, (long)newVal, (long)oldVal) == oldVal;
}

#endif

#if defined(SPADAS_ENV_LINUX)

Int Atom::increase() const
{
	return __atomic_add_fetch((volatile int *)&val, 1, __ATOMIC_ACQ_REL);
}

Int Atom::decrease() const
{
	return __atomic_sub_fetch((volatile int *)&val, 1, __ATOMIC_ACQ_REL);
}

Int Atom::add(Int number) const
{
	return __atomic_add_fetch((volatile int *)&val, number, __ATOMIC_ACQ_REL);
}

Bool Atom::cas(Int oldVal, Int newVal) const
{
	return __atomic_compare_exchange((volatile int *)&val, &oldVal, &newVal, false, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
}

#endif

#if defined(SPADAS_ENV_MACOS)

#include <libkern/OSAtomic.h>

Int Atom::increase() const
{
	return (Int)OSAtomicIncrement32Barrier((volatile int32_t *)&val);
}

Int Atom::decrease() const
{
	return (Int)OSAtomicDecrement32Barrier((volatile int32_t *)&val);
}

Int Atom::add(Int number) const
{
	return (Int)OSAtomicAdd32Barrier((int32_t)number, (volatile int32_t *)&val);
}

Bool Atom::cas(Int oldVal, Int newVal) const
{
	return OSAtomicCompareAndSwap32Barrier((int32_t)oldVal, (int32_t)newVal, (volatile int32_t *)&val);
}

#endif

void Atom::casSpin(Int oldVal, Int newVal) const
{
	if (cas(oldVal, newVal)) return;

	while (TRUE)
	{
		for (auto e = math::random(SPIN_INTERVAL_RANDOM_COUNT).firstElem(); e.valid(); ++e)
		{
			UInt count = math::max(0u, (UInt)(SPIN_CAS_MAX_INTERVAL * e.value())) + 1;
			UInt k = 0;
			while (TRUE)
			{
				if (++k % count != 0) continue;
				if (cas(oldVal, newVal)) return;
				else break;
			}
		}
	}
}