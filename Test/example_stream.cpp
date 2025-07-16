#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

void exampleStream()
{
	// Efficiency test of stream element enqueue and dequeue / 数据流元素推入和取出效率测试
	Stream<UInt> stream;
	Timer timer;

	ULong dequeuedSum = 0;
	for (UInt i = 0; i < 10000000; i++) // Ten million times / 一千万次
	{
		stream.enqueue(i);

		UInt dequeued = 0;
		if (stream.dequeueOne(dequeued)) dequeuedSum += dequeued;
	}

	console::print("Enqueue/dequeue time: " cat + timer.check());
	console::print("Dequeued sum: " cat dequeuedSum);
}