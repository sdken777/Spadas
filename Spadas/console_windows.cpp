
#if defined(SPADAS_ENV_WINDOWS)

#include "console.h"
#include "string_spadas.h"
#include <stdio.h>
#include <conio.h>
#include <wchar.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

namespace console_internal
{
	using namespace spadas;

	const UInt SCAN_SIZE = 1024;

	OptionalBool runInMsys;
	Bool isRunInMsys()
	{
		if (!runInMsys.valid)
		{
			String msystem = getenv("MSYSTEM");
			runInMsys = !msystem.isEmpty();
		}
		return runInMsys.value;
	}
}

using namespace spadas;
using namespace console_internal;
using namespace string_internal;

void spadas::console::popup(String text)
{
	MessageBoxW(0, text.wchars().data(), L"", MB_OK);
}

DefaultConsole::DefaultConsole() : Object<Vars>(new Vars(), TRUE)
{}

Bool DefaultConsole::supportScan()
{
	return TRUE;
}

String DefaultConsole::scan()
{
	if (isRunInMsys())
	{
		Char utf8Arr[SCAN_SIZE];
		Char *utf8Ptr = fgets(utf8Arr, SCAN_SIZE, stdin);

		if (utf8Ptr == 0) return String();

		UInt utf8Length = (UInt)strlen(utf8Ptr);
		if (utf8Length <= 1 || utf8Length > SCAN_SIZE) return String();

		utf8Length--;
		utf8Ptr[utf8Length] = 0;

		return utf8Arr;
	}
	else
	{
		Byte ansisArr[SCAN_SIZE];
		Byte *ansisPtr = (Byte*)fgets((Char*)ansisArr, SCAN_SIZE, stdin);
		
		if (ansisPtr == 0) return String();

		UInt ansisLength = (UInt)strlen((Char*)ansisPtr);
		if (ansisLength <= 1 || ansisLength > SCAN_SIZE) return String();

		ansisLength--;
		ansisPtr[ansisLength] = 0;

		Array<WChar> wchars(ansisLength + 1);
		UInt wcharsLength = ansiToWChar(ansisPtr, ansisLength, wchars.data(), wchars.size());
		wchars[wcharsLength] = 0;

		return wchars;
	}
}

Bool DefaultConsole::supportCheckKey()
{
	return !isRunInMsys();
}

Enum<Key> DefaultConsole::checkKey()
{
	int keyHit = _kbhit();
	if (keyHit == 0) return Key::Value::None;

	UInt key = (UInt)_getwch();
	switch (key)
	{
		case 13:
			return Key::Value::Enter;
		case 32:
			return Key::Value::Space;
		case 8:
			return Key::Value::Back;
		case 27:
			return Key::Value::Esc;
		case 9:
			return Key::Value::Tab;
		case 44:
			return Key::Value::Comma;
		case 46:
			return Key::Value::Period;
		case 0:
		{
			key = (UInt)_getwch();
			if (key >= 59 && key <= 66)
			{
				return (Key::Value)(key - 3);
			}
			return Key::Value::Unknown;
		}
		case 224:
			switch (_getwch())
		{
			case 82:
				return Key::Value::Insert;
			case 83:
				return Key::Value::Delete;
			case 71:
				return Key::Value::Home;
			case 79:
				return Key::Value::End;
			case 73:
				return Key::Value::PageUp;
			case 81:
				return Key::Value::PageDown;
			case 72:
				return Key::Value::Up;
			case 80:
				return Key::Value::Down;
			case 75:
				return Key::Value::Left;
			case 77:
				return Key::Value::Right;
			default:
				return Key::Value::Unknown;
		}
		default:
		{
			if (key >= 48 && key <= 57)
			{
				return (Key::Value)(key - 28);
			}
			if (key >= 65 && key <= 90)
			{
				return (Key::Value)(key - 35);
			}
			if (key >= 97 && key <= 122)
			{
				return (Key::Value)(key - 67);
			}
			return Key::Value::Unknown;
		}
	}
}

void DefaultConsole::print(String text, Enum<MessageLevel> level)
{
	if (isRunInMsys())
	{
		printf_s("%s\n", text.chars().data());
		fflush(stdout);
	}
	else
	{
		Array<WChar> wchars = text.wchars();
		Binary ansis(wchars.size() * 2);
		UInt ansiLength = wCharToAnsi(wchars.data(), wchars.size(), ansis.data(), ansis.size());
		ansis[ansiLength] = 0;
		printf_s("%s\n", (Char*)ansis.data());
	}
}

#endif
