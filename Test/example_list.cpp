
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleList()
{
	// List扩展效率测试
	Timer timer;
	List<UInt> list;
	for (UInt i = 0; i < 10000000/* 一千万次 */; i++) list.addToTail(i);
	console::print(SS"Add to tail: " + timer.check());

	// List插入效率测试
	timer.start();
	for (UInt i = 0; i < 10000000/* 一千万次 */; i++) list.addToHead(i);
	console::print(SS"Add to head: " + timer.check());

	// List移除效率测试
	timer.start();
	for (UInt i = 0; i < 10000000/* 一千万次 */; i++) list.removeHead();
	console::print(SS"Remove head: " + timer.check());

	// List转数组效率测试
	timer.start();
	Array<UInt> arr = list.toArray();
	console::print(SS"To array: " + timer.check());

	// List清空效率测试
	timer.start();
	list.clear();
	console::print(SS"Clear: " + timer.check());
}