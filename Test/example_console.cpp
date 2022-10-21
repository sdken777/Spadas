
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleConsole()
{
	// 从控制台输入文本并弹出对话框显示
	console::print("Input some text.");

	String text = console::scan();
	if (!text.isEmpty()) console::popup(text);

	// 按键检测，输入Q推出
	console::print("Press keys, Q to break.");
	while (TRUE)
	{
		Enum<Key> key = console::checkKey();
		if (key == Key::Q) break;
		else if (key != Key::None) console::print(key);
	}
}