
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleForElement()
{
	// 遍历数组（比直接索引效率高）
	console::print("- 1 -");

	auto arr = Array<UInt>::create(3, 101, 123, 777);
	for (auto e = arr.firstElem(); e.valid(); ++e)
	{
		console::print(SS e.index() + ": " + e.value());
	}
	for (auto e = arr.lastElem(); e.valid(); --e) // 倒序遍历
	{
		console::print(SS e.index() + ": " + e.value());
	}

	// 遍历链表（可一边遍历一边移除）
	console::print("- 2 -");

	auto list = List<UInt>(arr);
	for (auto e = list.head(); e.valid(); ++e)
	{
		console::print(e.value());
		if (e.value() == 123) e.remove();
	}

	console::print(SS"Result: " + String::merge(list.toArray(), ", "));
}