
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleArrayX()
{
	// The efficiency is greatly improved compared with Spadas6 / 效率较Spadas6有大幅提高
	Timer timer;
	ArrayX<UInt> arrx;
	for (UInt i = 0; i < 10000000/* Ten million times / 一千万次 */; i++) arrx.append(i); // Append / 扩展
	console::print(SS"Append: " + timer.check());

	timer.start();
	Array<UInt> arr = arrx.toArray(); // Convert to array / 转数组
	console::print(SS"To array: " + timer.check());

	timer.start();
	arrx = ArrayX<UInt>(); // Release / 析构
	console::print(SS"Release: " + timer.check());
}