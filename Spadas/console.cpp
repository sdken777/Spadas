
#include <stdio.h>
#include <wchar.h>
#include "console.h"

namespace console_internal
{
	Interface<IConsole> cs = DefaultConsole();
	LoggerManager lm;

	Bool scanning;
	Lock scanningLock;
	Bool keyWaiting;
	Lock keyWaitingLock;
}

using namespace spadas;
using namespace console_internal;

// IConsole
void IConsole::print(String text, Enum<MessageLevel> level)
{
#if defined(SPADAS_ENV_WINDOWS)
	wprintf_s(L"[SPADAS IConsole::print] Unimplemented method called. (Maybe the interface is invalid)\n");
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
    printf("[SPADAS IConsole::print] Unimplemented method called. (Maybe the interface is invalid)\n");
#endif
}

Bool IConsole::supportScan()
{
	return FALSE;
}

String IConsole::scan()
{
	return String();
}

Bool IConsole::supportCheckKey()
{
	return FALSE;
}

Enum<Key> IConsole::checkKey()
{
	return Key::Value::None;
}

// ILogger
void ILogger::print(String text)
{
}

// namespace console
Interface<IConsole> spadas::console::useConsole(Interface<IConsole> target)
{
	if (!target.isValid()) target = DefaultConsole();

	Interface<IConsole> oldCS = cs;
    cs = target;
	return oldCS;
}

Interface<ILogger> spadas::console::useThreadLogger(Interface<ILogger> target)
{
	UInt threadID = Threads::getCurrentThreadID();
	return lm.useLogger(threadID, target);
}

void spadas::console::print(String text)
{
	UInt threadID = Threads::getCurrentThreadID();
	if (!lm.print(threadID, text)) cs->print(text, MessageLevel::Value::Debug);
}
void spadas::console::print(String text, Enum<MessageLevel> level)
{
	if (level == MessageLevel::Value::Debug)
	{
		UInt threadID = Threads::getCurrentThreadID();
		if (!lm.print(threadID, text)) cs->print(text, MessageLevel::Value::Debug);
	}
	else cs->print(text, level);
}

String spadas::console::scan()
{
	if (cs->supportScan())
	{
		scanningLock.enter();
		scanning = TRUE;
		scanningLock.leave();
		
		String out = cs->scan();
		
		scanningLock.enter();
		scanning = FALSE;
		scanningLock.leave();
		
		return out;
	}
	else
	{
		return String();
	}
}

Enum<Key> spadas::console::waitKey()
{
	if (cs->supportCheckKey())
	{
		keyWaitingLock.enter();
		keyWaiting = TRUE;
		keyWaitingLock.leave();
		
		Enum<Key> out;
		scanningLock.enter();
		if (!scanning)
		{
			while (TRUE)
			{
				out = cs->checkKey();
				if (out == Key::Value::None) break;
			}
		}
		scanningLock.leave();
		while (TRUE)
		{
			system::wait(10);
			LockProxy lock(scanningLock);
			if (scanning) continue;
			else if ((out = cs->checkKey()) != Key::Value::None) break;
		}
		
		keyWaitingLock.enter();
		keyWaiting = FALSE;
		keyWaitingLock.leave();
		
		return out;
	}
	else
	{
		return Key::Value::None;
	}
}

Enum<Key> spadas::console::checkKey()
{
	if (cs->supportCheckKey())
	{
		LockProxy lock1(keyWaitingLock);
		if (keyWaiting) return Key::Value::None;
		
		LockProxy lock2(scanningLock);
		if (scanning) return Key::Value::None;
		
		return cs->checkKey();
	}
	else
	{
		return Key::Value::None;
	}
}
