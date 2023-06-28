
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

class ThreadSafeTestVars : public Vars
{
public:
	Safe<String> forRead; // 用于测试拷贝构造函数、析构函数和重定向操作的多线程安全
	Safe<String> forWrite; // 用于测试对同一个对象写操作的多线程安全
	Array<UInt> counters; // 记录写操作次数，用于确认操作结果是否正确
};

class ThreadSafeTest : public Object<ThreadSafeTestVars>, public IWorkflow
{
public:
	ThreadSafeTest() : Object<ThreadSafeTestVars>(new ThreadSafeTestVars, TRUE)
	{
		vars->forRead = "A";
		vars->forWrite = String::createWithSize(1); // 空对象无法加锁（字符串默认为空对象）
		vars->counters = Array<UInt>(2, 0);
	}
	void printResult()
	{
		UInt counterSum = vars->counters[0] + vars->counters[1];
		console::print(SS vars->forWrite.get().length() + "=" + counterSum + " (" + String::merge(vars->counters) + ")"); // forWrite字符串长度应与写操作次数一致
	}

private:
	Array<String> getThreadNames() override
	{
		// 四个子线程，两个对forRead字符串进行重定向操作，两个先对forRead字符串进行只读操作并对forWrite字符串进行写操作
		return Array<String>::create(4, "thread-0", "thread-1", "thread-2", "thread-3");
	}
	UInt getTimeInterval(UInt threadIndex) override
	{
		return 0; // 全部都为自旋线程
	}
	void onThreadLoop(UInt threadIndex, Flag shouldEnd) override
	{
		if (threadIndex >= 2)
		{
			// 不断重定向至新创建的字符串
			vars->forRead = "A";
		}
		else
		{
			// 不断读字符串并添加至另一字符串结尾
			SpinLocked<String>(vars->forWrite)->operator+=(vars->forRead.get());
			vars->counters[threadIndex]++;
		}
	}
};

void exampleThreadSafe()
{
	// 创建4个自旋线程，等待100毫秒，停止线程并打印结果
	ThreadSafeTest test;
	Threads threads = Threads::start(test);
	system::wait(100);
	threads.stop();
	test.printResult();
}