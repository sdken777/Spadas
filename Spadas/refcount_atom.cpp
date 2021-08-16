
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
	return __sync_add_and_fetch((volatile int *)&val, 1);
}

Int Atom::decrease()
{
	return __sync_sub_and_fetch((volatile int *)&val, 1);
}
#endif