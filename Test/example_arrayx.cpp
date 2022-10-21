
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleArrayX()
{
	// 效率较Spadas6有大幅提高
	Timer timer;
	ArrayX<UInt> arrx;
	for (UInt i = 0; i < 10000000/* 一千万次 */; i++) arrx.append(i); // 扩展
	console::print(SS"Append: " + timer.check());

	timer.start();
	Array<UInt> arr = arrx.toArray(); // 转数组
	console::print(SS"To array: " + timer.check());

	timer.start();
	arrx = ArrayX<UInt>(); // 析构
	console::print(SS"Release: " + timer.check());
}