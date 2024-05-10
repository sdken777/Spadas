
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleEqual()
{
	// Always use == and != to get whether it's the same object (except for String, whether the string value is the same) / 统一使用==和!=判断是否为同一个对象（仅String例外，为字符串值是否相同）
	console::print("- 1 -");

	String string1, string2, string3 = string1;
	Timer timer1, timer2, timer3 = timer1;

	see(string1 == string2);
	see(string1 == string3);
	see(timer1 == timer2);
	see(timer1 == timer3);

	// Always use "contain" to get whether an array contains a value or object / 统一使用contain判断数组是否包含某个值或对象
	console::print("- 2 -");

	auto strings = Array<String>::create(3, "abc", "def", "xyz");
	console::print("strings contains 'def': " cat strings.contain("def"));

	Array<Timer> timers(3);
	console::print("timers contains Timer(): " cat timers.contain(Timer()));
}