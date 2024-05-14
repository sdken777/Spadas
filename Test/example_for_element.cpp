
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleForElement()
{
	// Array iteration (more efficient than indexing with []) / 遍历数组（比直接索引效率高）
	console::print("- 1 -");

	auto arr = Array<UInt>::create(3, 101, 123, 777);
	for (auto e = arr.firstElem(); e.valid(); ++e)
	{
		console::print(catAll(e.index(), ": ", e.value()));
	}
	for (auto e = arr.lastElem(); e.valid(); --e) // Reverse iteration / 倒序遍历
	{
		console::print(catAll(e.index(), ": ", e.value()));
	}

	// Linked list iteration (nodes can be removed during iteration) / 遍历链表（可一边遍历一边移除）
	console::print("- 2 -");

	auto list = List<UInt>(arr);
	for (auto e = list.head(); e.valid(); ++e)
	{
		console::print(catAll(e.index(), ": ", e.value()));
		if (e.value() == 123) e.remove();
	}

	console::print("Result: " cat String::merge(list.toArray()));
}