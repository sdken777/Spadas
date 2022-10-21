
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleEqual()
{
	// 统一使用==和!=判断是否为同一个对象（仅String例外，为字符串值是否相同）
	console::print("- 1 -");

	String string1, string2, string3 = string1;
	Timer timer1, timer2, timer3 = timer1;

	console::print(SS"string1==string2 : " + (string1 == string2));
	console::print(SS"string1==string3 : " + (string1 == string3));
	console::print(SS"timer1==timer2 : " + (timer1 == timer2));
	console::print(SS"timer1==timer3 : " + (timer1 == timer3));

	// 统一使用contain判断数组是否包含某个值或对象
	console::print("- 2 -");

	auto strings = Array<String>::create(3, "abc", "def", "xyz");
	console::print(SS"strings contains 'def': " + strings.contain("def"));

	Array<Timer> timers(3);
	console::print(SS"timers contains Timer(): " + timers.contain(Timer()));
}