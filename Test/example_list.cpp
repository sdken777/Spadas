#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

void exampleList()
{
	// Efficiency test of linked-list appending / List扩展效率测试
	Timer timer;
	List<UInt> list;
	for (UInt i = 0; i < 10000000; i++) list.addToTail(i); // Ten million times / 一千万次
	console::print("Add to tail: " cat timer.check());

	// Efficiency test of linked-list insertion / List插入效率测试
	timer.start();
	for (UInt i = 0; i < 10000000; i++) list.addToHead(i); // Ten million times / 一千万次
	console::print("Add to head: " cat timer.check());

	// Efficiency test of linked-list removing / List移除效率测试
	timer.start();
	for (UInt i = 0; i < 10000000; i++) list.removeHead(); // Ten million times / 一千万次
	console::print("Remove head: " cat timer.check());

	// Efficiency test of linked-list conversion / List转数组效率测试
	timer.start();
	Array<UInt> arr = list.toArray();
	console::print("To array: " cat timer.check());

	// Efficiency test of linked-list releasing / List清空效率测试
	timer.start();
	list.clear();
	console::print("Clear: " cat timer.check());
}