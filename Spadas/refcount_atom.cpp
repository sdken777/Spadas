
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
	val = val0;
}

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>

Int Atom::increase()
{
	return (Int)InterlockedIncrement((volatile LONG*)&val);
}

Int Atom::decrease()
{
	return (Int)InterlockedDecrement((volatile LONG*)&val);
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

#endif