
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleLambda()
{
	// 搜索数组
	console::print("- 1 -");

	auto arr = Array<UInt>::create(3, 101, 123, 777);
	auto indices = arr.searchAs([](UInt number){ return number % 3 == 0; });
	console::print(SS"Mod3=0 indices: " + String::merge(indices, ", "));

	// 数组转换
	console::print("- 2 -");

	auto input = Array<String>::create(3, "Hello", "Nice", "Weather");
	auto length = input.convert<UInt>([](String str){ return str.length(); });
	console::print(SS"Length: " + String::merge(length, ", "));
}