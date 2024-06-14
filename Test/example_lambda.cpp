
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleLambda()
{
	// Search in array / 搜索数组
	console::print("- 1 -");

	auto arr = Array<UInt>::create(3, 101, 123, 777);
	auto indices = arr.searchAs([](UInt number){ return number % 3 == 0; });
	console::print("Mod3=0 indices: " cat String::merge(indices, ", "));

	// Convert to array of another type / 数组转换
	console::print("- 2 -");

	auto input = Array<String>::create(3, "Hello", "Nice", "Weather");
	auto length = input.convert<UInt>([](String str){ return str.length(); });
	console::print("Length: " cat String::merge(length, ", "));
}