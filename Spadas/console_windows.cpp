
#include "spadas.h"

#include "console.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <stdio.h>
#include <conio.h>
#include <wchar.h>
#include <locale.h>
#include <windows.h>

namespace console_internal
{
	using namespace spadas;
	const UInt SCAN_SIZE = 1024;
}

using namespace spadas;
using namespace spadas_internal;
using namespace console_internal;

void spadas::console::popup(String text)
{
	MessageBoxW(0, text.data(), L"", MB_OK);
}

DefaultConsole::DefaultConsole() : Object<Vars>(new Vars(), TRUE)
{
	char *newLocale = setlocale(LC_ALL, "chs");
	if (newLocale == 0 || newLocale[0] == 0)
	{
		setlocale(LC_ALL, "C");
		wprintf(L"\n%s\n", L"Console: Locale initialization failed. The default console may be unavailable.");
	}
}

Bool DefaultConsole::supportScan()
{
	return TRUE;
}

String DefaultConsole::scan()
{
	WChar buf0[SCAN_SIZE];
	WChar *buf = fgetws(buf0, SCAN_SIZE, stdin);
	
	if (buf == 0) return String();
    
	buf[wcslen(buf)-1] = 0;
	return buf;
}

Bool DefaultConsole::supportCheckKey()
{
	return TRUE;
}

Enum<Key> DefaultConsole::checkKey()
{
	int keyHit = _kbhit();
	if (keyHit == 0) return Key::None;

	UInt key = (UInt)_getwch();
	switch (key)
	{
		case 13:
			return Key::Enter;
		case 32:
			return Key::Space;
		case 8:
			return Key::Back;
		case 27:
			return Key::Esc;
		case 9:
			return Key::Tab;
		case 44:
			return Key::Comma;
		case 46:
			return Key::Period;
		case 0:
		{
			key = (UInt)_getwch();
			if (key >= 59 && key <= 66)
			{
				return (Int)(key - 3);
			}
			return Key::Unknown;
		}
		case 224:
			switch (_getwch())
		{
			case 82:
				return Key::Insert;
			case 83:
				return Key::Delete;
			case 71:
				return Key::Home;
			case 79:
				return Key::End;
			case 73:
				return Key::PageUp;
			case 81:
				return Key::PageDown;
			case 72:
				return Key::Up;
			case 80:
				return Key::Down;
			case 75:
				return Key::Left;
			case 77:
				return Key::Right;
			default:
				return Key::Unknown;
		}
		default:
		{
			if (key >= 48 && key <= 57)
			{
				return (Int)(key - 28);
			}
			if (key >= 65 && key <= 90)
			{
				return (Int)(key - 35);
			}
			if (key >= 97 && key <= 122)
			{
				return (Int)(key - 67);
			}
			return Key::Unknown;
		}
	}
}

void DefaultConsole::print(String text, Enum<MessageLevel> level)
{
	wprintf_s(L"%s\n", text.data());
}

#endif
