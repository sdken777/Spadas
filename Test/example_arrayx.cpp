#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

void exampleArrayX()
{
	// Efficiency test of array appending / ArrayX扩展效率测试
	Timer timer;
	ArrayX<UInt> arrx;
	for (UInt i = 0; i < 10000000; i++) arrx.append(i); // Ten million times / 一千万次
	console::print("Append: " cat timer.check());

	// Efficiency test of array conversion / ArrayX转数组效率测试
	timer.start();
	Array<UInt> arr = arrx.toArray();
	console::print("To array: " cat timer.check());

	// Efficiency test of array releasing / ArrayX析构效率测试
	timer.start();
	arrx.clear();
	console::print("Release: " cat timer.check());
}