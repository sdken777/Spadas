
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleConsole()
{
	// Enter text from the console and show in a message box / 从控制台输入文本并弹出对话框显示
	console::print("Input some text.");

	String text = console::scan();
	if (!text.isEmpty()) console::popup(text);

	// Key detection, enter Q to exit / 按键检测，输入Q退出
	console::print("Press keys, Q to break.");
	while (TRUE)
	{
		Enum<Key> key = console::checkKey();
		if (key == Key::Q) break;
		else if (key != Key::None) console::print(key);
	}
}