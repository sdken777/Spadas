
#include "spadas.h"

#include "console.h"

#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>

#if defined(SPADAS_ENV_MACOS)
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace console_internal
{
	using namespace spadas;

	const UInt SCAN_SIZE = 1024;

	termios beginConioMode()
	{
		termios oldTermios;
		termios newTermios;

		tcgetattr(STDIN_FILENO, &oldTermios);
		newTermios = oldTermios;

		newTermios.c_lflag &= ~(ECHO | ICANON);
		tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

		return oldTermios;
	}

	void endConioMode(termios oldTermios)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
	}

	Int getch()
	{
		termios oldTermios = beginConioMode();

		int key = getchar();

		endConioMode(oldTermios);
		return key;
	}

	Bool kbhit()
	{
		termios oldTermios = beginConioMode();

		timeval time = { 0, 0 };
		fd_set fdSet;
		utility::memorySet(0, &fdSet, sizeof(fd_set));
		FD_ZERO(&fdSet);
		FD_SET(0, &fdSet);

		int keyIn = select(1, &fdSet, NULL, NULL, &time);

		endConioMode(oldTermios);
		return keyIn == 0 ? FALSE : TRUE;
	}

	Int intToKey(int key)
	{
		switch (key)
		{
			case 10:
				return Key::Enter;
			case 32:
				return Key::Space;
			case 9:
				return Key::Tab;
			case 44:
				return Key::Comma;
			case 46:
				return Key::Period;
			case 127:
				return Key::Back;
			case 126:
				return Key::Delete;

			case 27:
			{
#if defined(SPADAS_ENV_LINUX)
				if (stdin->_IO_read_ptr == stdin->_IO_read_end)
				{
					return Key::Esc;
				}
#endif
#if defined(SPADAS_ENV_MACOS)
				if (stdin->_r <= 0)
				{
					return Key::Esc;
				}
#endif
				switch (getch())
				{
					case 79:
					{
						switch (getch())
						{
							case 72:
								return Key::Home;
							case 70:
								return Key::End;
							case 80:
								return Key::F1;
							case 81:
								return Key::F2;
							case 82:
								return Key::F3;
							case 83:
								return Key::F4;
						}
					}
					case 91:
					{
						switch (getch())
						{
							case 50:
								getch();
								return Key::Insert;
							case 51:
								getch();
								return Key::Delete;
							case 53:
								getch();
								return Key::PageUp;
							case 54:
								getch();
								return Key::PageDown;
							case 65:
								return Key::Up;
							case 66:
								return Key::Down;
							case 67:
								return Key::Right;
							case 68:
								return Key::Left;
							case 49:
							{
								switch (getch())
								{
									case 53:
										getch();
										return Key::F5;
									case 55:
										getch();
										return Key::F6;
									case 56:
										getch();
										return Key::F7;
									case 57:
										getch();
										return Key::F8;
								}
							}
						}
					}
				}
				return Key::Unknown;
			}

			default:
			{
				if (key >= 48 && key <= 57)
				{
					return key - 28;
				}
				if (key >= 65 && key <= 90)
				{
					return key - 35;
				}
				if (key >= 97 && key <= 122)
				{
					return key - 67;
				}
				return Key::Unknown;
			}
		}
		return Key::Unknown;
	}
}

using namespace spadas;
using namespace spadas_internal;
using namespace console_internal;

#if defined(SPADAS_ENV_LINUX)
void console::popup(String text)
{
	String cmd = (String)"xmessage -center \"" + text + "\"";
	if (::system((Char*)cmd.bytes())) {}
}
#endif
#if defined(SPADAS_ENV_MACOS)
void console::popup(String text)
{
	CFStringRef cfString = CFStringCreateWithBytes(NULL, (UInt8*)text.bytes(), text.length(), kCFStringEncodingUTF8, false);
	CFUserNotificationDisplayAlert(0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, CFSTR(""), cfString, NULL, NULL, NULL, NULL);
	CFRelease(cfString);
}
#endif

void DefaultConsole::print(String text, Enum<MessageLevel> level)
{
	printf("%s\n", (Char*)text.bytes());
}

String DefaultConsole::scan()
{
	Char buf0[SCAN_SIZE];
	Char *buf = fgets(buf0, SCAN_SIZE, stdin);

	if (buf == 0) return String();

	buf[strlen(buf)-1] = 0;
	return buf;
}

DefaultConsole::DefaultConsole() : Object<Vars>(new Vars(), TRUE)
{
#if defined(SPADAS_ENV_MACOS)
	char *newLocale = setlocale(LC_ALL, "en_US.UTF-8");
	if (newLocale == 0 || newLocale[0] == 0)
	{
		setlocale(LC_ALL, "C");
		printf("\n%s\n", "Console: Locale initialization failed. The default console may be unavailable.");
	}
#endif
}

Bool DefaultConsole::supportScan()
{
	return TRUE;
}

Bool DefaultConsole::supportCheckKey()
{
	return TRUE;
}

Enum<Key> DefaultConsole::checkKey()
{
	Bool keyIn = kbhit();
	if (keyIn) return intToKey(getch());
	else return Key::None;
}

#endif
