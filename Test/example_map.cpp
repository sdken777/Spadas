
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleMap()
{
	// Add the Map template class to realize the mapping between any type, and no longer limit the number of key-value pairs / 增加Map模板类实现任意类型间的映射，且不再限制键值对的个数
	Map<IntKey, String> map1;
	map1[1234567] = "Hello";
	map1[7654321] = "World";
	
	Map<Timer, String> map2;
	Timer timer1, timer2;
	map2[timer1] = "Timer1";
	map2[timer2] = "Timer2";

	// Use "keyValues" method to query all key-value pairs at one time to improve efficiency / 通过keyValues方法一次性拿出所有键值对，提高效率
	for (auto pair = map1.keyValues().firstElem(); pair.valid(); ++pair)
	{
		console::print(SS pair->key.value() + " -> " + pair->value);
	}

	// Get a value without creating a new key-value pair with "get" or "tryGet" (using operator [] will automatically create a new key-value pair) / 通过get/tryGet在不新创建键值对的情况下获得值（使用[]索引将自动创建新键值对）
	console::print(SS"Timer1 -> " + map2.get(timer1, "No timer"));
	console::print(SS"Timer() -> " + map2.get(Timer(), "No timer"));

	String timer2Value;
	if (map2.tryGet(timer2, timer2Value)) console::print(SS"Timer2 -> " + timer2Value);
}