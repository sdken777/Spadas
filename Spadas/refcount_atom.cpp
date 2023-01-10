
#include "spadas.h"

using namespace spadas;

Atom::Atom() : val(0)
{
}

Atom::Atom(Int val0) : val(val0)
{
}

Int Atom::get()
{
	return val;
}
void Atom::set(Int val0)
{
	*(volatile Int*)&val = val0;
}

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>

Int Atom::increase()
{
	return (Int)InterlockedIncrement((volatile long*)&val);
}

Int Atom::decrease()
{
	return (Int)InterlockedDecrement((volatile long*)&val);
}

Int Atom::add(Int number)
{
	return (Int)InterlockedAdd((volatile long*)&val, (long)number);
}

Bool Atom::cas(Int oldVal, Int newVal)
{
	return (Int)InterlockedCompareExchange((volatile long*)&val, (long)newVal, (long)oldVal) == oldVal;
}

#endif

#if defined(SPADAS_ENV_LINUX)

Int Atom::increase()
{
	return __atomic_add_fetch((volatile int *)&val, 1, __ATOMIC_ACQ_REL);
}

Int Atom::decrease()
{
	return __atomic_sub_fetch((volatile int *)&val, 1, __ATOMIC_ACQ_REL);
}

Int Atom::add(Int number)
{
	return __atomic_add_fetch((volatile int *)&val, number, __ATOMIC_ACQ_REL);
}

Bool Atom::cas(Int oldVal, Int newVal)
{
	return __atomic_compare_exchange((volatile int *)&val, &oldVal, &newVal, false, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
}

#endif

#if defined(SPADAS_ENV_MACOS)

#include <libkern/OSAtomic.h>

Int Atom::increase()
{
	return (Int)OSAtomicIncrement32Barrier((volatile int32_t *)&val);
}

Int Atom::decrease()
{
	return (Int)OSAtomicDecrement32Barrier((volatile int32_t *)&val);
}

Int Atom::add(Int number)
{
	return (Int)OSAtomicAdd32Barrier((int32_t)number, (volatile int32_t *)&val);
}

Bool Atom::cas(Int oldVal, Int newVal)
{
	return OSAtomicCompareAndSwap32Barrier((int32_t)oldVal, (int32_t)newVal, (volatile int32_t *)&val);
}

#endif