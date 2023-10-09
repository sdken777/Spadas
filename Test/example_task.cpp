
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

// Implement a task class / 实现一个Task类
class TestThreads : public EmptyObject, public ITask
{
private:
	void onRunTask(Flag shouldEnd)
	{
		shouldEnd.waitSet();
		console::print("Thread over");
	}
};

// Main function / 主函数
void exampleTask()
{
	TaskManager tm;
	TestThreads task;

	UInt count = 0;
	while (count < 5)
	{
		tm.start(task);
		tm.stop(task);
		tm.start(TestThreads());
		console::print((String)count++ + ":" + tm.getTasks().size());
	}

	tm.stopAll();

	console::print(SS"Final:" + tm.getTasks().size());
}