
#include "spadas.h"

using namespace spadas;

void exampleCreate()
{
	// 创建数组
	auto arr = Array<UInt>::create(3, 1, 2, 3);
	console::print(String::merge(arr, ", "));

	// 创建二进制数据
	auto bin = Binary::create(3, 1, 255, 128);
	console::print(String::createHexString(bin));

	// 创建字典
	auto dict = Dictionary<String>::create(3, "Name", "Jack", "Age", "25", "Weight", "70");
	console::print(String::merge(dict.keysSorted(), ", "));
}