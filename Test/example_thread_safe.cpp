#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

class ThreadSafeTest : public BaseObject, public IWorkflow
{
private:
	class ThreadSafeTestVars : public Vars
	{
	public:
		SPADAS_VARS(ThreadSafeTest, Vars);

		Safe<String> forRead; // Used to test multithread safety for copy constructor, destructor, and redirection operation / 用于测试拷贝构造函数、析构函数和重定向操作的多线程安全
		Safe<String> forWrite; // Used to test multithreaded safety of writing to the same object / 用于测试对同一个对象写操作的多线程安全
		Array<UInt> counters; // Record the number of write operations to check whether the result is correct / 记录写操作次数，用于确认操作结果是否正确
		Stream<BusDeviceData> streamFull, streamEmpty; // Used to test stream enqueue and dequeue / 用于测试数据流元素的推入和取出
		Bool streamDisable;
		
		ThreadSafeTestVars() : streamFull(10000, FALSE), streamEmpty(10000, FALSE), streamDisable(FALSE)
		{}
	};

public:
	SPADAS_CLASS("ThreadSafeTest", ThreadSafeTestVars)

	ThreadSafeTest() : BaseObject(new ThreadSafeTestVars)
	{
		var()->forRead = "A";
		var()->forWrite = String::createWithSize(1); // Empty objects cannot be locked (strings default to empty objects) / 空对象无法加锁（字符串默认为空对象）
		var()->counters = Array<UInt>(2, 0);
	}
	void clearStreams()
	{
		console::print("Elements: Full=" cat var()->streamFull.nElements() cat " Empty=" cat var()->streamEmpty.nElements());
		var()->streamDisable = TRUE;
	}
	void printResult()
	{
		UInt counterSum = var()->counters[0] + var()->counters[1];
		console::print(catAll(var()->forWrite.get().length(), "=", counterSum, " (", String::merge(var()->counters, ", "), ")")); // The string length of "forWrite" must be the same as the number of write operations / forWrite字符串长度应与写操作次数一致
	}

private:
	Array<String> getThreadNames() override
	{
		// Four threads, two to redirect "forRead" string, two to read "forRead" string and write to "forWrite" string / 四个子线程，两个对forRead字符串进行重定向操作，两个先对forRead字符串进行只读操作并对forWrite字符串进行写操作
		return Array<String>::create(4, "thread-0", "thread-1", "thread-2", "thread-3");
	}
	UInt getTimeInterval(UInt threadIndex) override
	{
		return 0; // All are spin threads / 全部都为自旋线程
	}
	void onThreadLoop(UInt threadIndex, Flag shouldEnd) override
	{
		if (threadIndex >= 2)
		{
			// Constantly redirects to newly created strings / 不断重定向至新创建的字符串
			var()->forRead = "A";

			// Constantly enqueue data to streams / 不断向数据流推入数据
			if (var()->streamDisable) return;
			if (threadIndex % 2 == 0)
			{
				BusDeviceData data;
				data.binary = Binary(8, 0);
				var()->streamFull.enqueue(data);
				var()->streamFull.enqueue(data);
				var()->streamFull.enqueue(data);
				var()->streamFull.enqueue(data);
			}
			else
			{
				BusDeviceData data;
				data.binary = Binary(8, 0);
				var()->streamEmpty.enqueue(data);
			}
		}
		else
		{
			// Keep reading strings and adding them to the end of another string / 不断读字符串并添加至另一字符串结尾
			SpinLocked<String>(var()->forWrite)->operator+=(var()->forRead.get());
			var()->counters[threadIndex]++;

			// Keep dequeuing data from streams / 不断从数据流取出数据
			if (threadIndex % 2 == 0)
			{
				var()->streamFull.dequeue(1);
			}
			else
			{
				var()->streamEmpty.dequeue(1);
				var()->streamEmpty.dequeue(1);
				var()->streamEmpty.dequeue(1);
				var()->streamEmpty.dequeue(1);
			}
		}
	}
};

void exampleThreadSafe()
{
	// Create 4 spin threads, wait 100 ms, stop the thread and print the result / 创建4个自旋线程，等待500毫秒，停止线程并打印结果
	ThreadSafeTest test;
	Threads threads = Threads::start(test);
	system::wait(500);
	test.clearStreams();
	system::wait(10);
	threads.stop();
	test.printResult();
}