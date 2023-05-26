
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleConsole()
{
	// 从控制台输入文本并弹出对话框显示
	console::print("Input some text.");

	String text = console::scan();
	if (!text.isEmpty()) console::popup(text);

	// 按键检测，输入Q退出
	console::print("Press keys, Q to break (5 seconds timeout).");
	Timer timer;
	while (timer.check() < 5000)
	{
		Enum<Key> key = console::checkKey();
		if (key == Key::Value::Q) break;
		else if (key != Key::Value::None) console::print(key);
	}
}