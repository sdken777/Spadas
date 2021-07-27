
#include "spadas.h"

using namespace spadas;

// 实现一个Task类
class TestThreads : public EmptyObject, public ITask
{
private:
	void onRunTask(Flag shouldEnd)
	{
		shouldEnd.waitSet();
		console::print("Thread over");
	}
};

// 主函数
void exampleTask()
{
	TaskManager tm;
	TestThreads task;

	UInt count = 0;
	while (count < 10)
	{
		tm.start(task);
		tm.stop(task);
		tm.start(TestThreads());
		console::print((String)count++ + ":" + tm.getTasks().size());
	}

	tm.stopAll();

	console::print(SS"Final:" + tm.getTasks().size());
}