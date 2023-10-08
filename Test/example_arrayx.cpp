
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleArrayX()
{
	// ArrayX扩展效率测试
	Timer timer;
	ArrayX<UInt> arrx;
	for (UInt i = 0; i < 10000000/* 一千万次 */; i++) arrx.append(i);
	console::print("Append: " cat timer.check());

	// ArrayX转数组效率测试
	timer.start();
	Array<UInt> arr = arrx.toArray();
	console::print("To array: " cat timer.check());

	// ArrayX析构效率测试
	timer.start();
	arrx = ArrayX<UInt>();
	console::print("Release: " cat timer.check());
}