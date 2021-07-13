
#include "spadas.h"

#include "console.h"
#include <stdio.h>
#include <wchar.h>

namespace spadas_internal
{
	Interface<IConsole> cs = DefaultConsole();
	Bool scanning;
	Lock scanningLock;
	Bool keyWaiting;
	Lock keyWaitingLock;
}

using namespace spadas;
using namespace spadas_internal;

// MessageLevel
Bool MessageLevel::isValid(Int val)
{
	return val >= MessageLevel::Debug && val <= MessageLevel::Error;
}

String MessageLevel::toString(Int val)
{
	switch (val)
	{
	case MessageLevel::Error:
		return "Error level";
	case MessageLevel::Warning:
		return "Warning level";
	case MessageLevel::Info:
		return "Info level";
	case MessageLevel::Debug:
		return "Debug level";
	default:
		return String();
	}
}

Int MessageLevel::defaultValue()
{
	return MessageLevel::Debug;
}

// Key
Bool Key::isValid(Int val)
{
    return (val >= Key::None && val <= Key::F8) || val == Key::Unknown;
}
String Key::toString(Int val)
{
	if (Key::isValid(val))
	{
		if (val < Key::None || val > Key::F8)
		{
			if (val == Key::Unknown) return "Unknown Key";
			else return String();
		}
		else
		{
			if (val < Key::Key0)
			{
				switch (val)
				{
				case Key::None:
					return "None";
				case Key::Enter:
					return "Key Enter";
				case Key::Space:
					return "Key Space";
				case Key::Back:
					return "Key Backspace";
				case Key::Esc:
					return "Key Esc";
				case Key::Tab:
					return "Key Tab";
				case Key::Shift:
					return "Key Shift";
				case Key::Ctrl:
					return "Key Ctrl";
				case Key::Insert:
					return "Key Insert";
				case Key::Delete:
					return "Key Delete";
				case Key::Home:
					return "Key Home";
				case Key::End:
					return "Key End";
				case Key::PageUp:
					return "Key PageUp";
				case Key::PageDown:
					return "Key PageDown";
				case Key::Up:
					return "Key Up";
				case Key::Down:
					return "Key Down";
				case Key::Left:
					return "Key Left";
				case Key::Right:
					return "Key Right";
				case Key::Comma:
					return "Key Comma";
				case Key::Period:
					return "Key Period";
				default:
					return String();
				}
			}
			else if (val < Key::A)
			{
				return (String)"Key " + (val - Key::Key0);
			}
			else if (val < Key::F1)
			{
				return (String)"Key " + (Char)(65 + val - Key::A);
			}
			else
			{
				return (String)"Key F" + (val - Key::F1 + 1);
			}
		}
	}
    else return String();
}
Int Key::defaultValue()
{
	return Key::None;
}

// IConsole
void IConsole::print(String text, Enum<MessageLevel> level)
{
#if defined(SPADAS_ENV_WINDOWS)
	wprintf_s(L"[SPADAS IConsole::print] Unimplemented method called. (Maybe the interface is invalid)\n");
#elif defined(SPADAS_ENV_LINUX)
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
	return Key::None;
}

// namespace console
Interface<IConsole> spadas::console::useConsole(Interface<IConsole> target)
{
	if (!target.isValid()) target = DefaultConsole();

	Interface<IConsole> oldCS = cs;
    cs = target;
	return oldCS;
}

void spadas::console::print(String text)
{
    cs->print(text, MessageLevel::Debug);
}
void spadas::console::print(String text, Enum<MessageLevel> level)
{
	cs->print(text, level);
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
				if (out == Key::None) break;
			}
		}
		scanningLock.leave();
		while (TRUE)
		{
			Flag(10).waitSet();
			LockProxy p(scanningLock);
			if (scanning) continue;
			else if ((out = cs->checkKey()) != Key::None) break;
		}
		
		keyWaitingLock.enter();
		keyWaiting = FALSE;
		keyWaitingLock.leave();
		
		return out;
	}
	else
	{
		return Key::None;
	}
}

Enum<Key> spadas::console::checkKey()
{
	if (cs->supportCheckKey())
	{
		LockProxy p1(keyWaitingLock);
		if (keyWaiting) return Key::None;
		
		LockProxy p2(scanningLock);
		if (scanning) return Key::None;
		
		return cs->checkKey();
	}
	else
	{
		return Key::None;
	}
}
