#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

// Implement a workflow class / 实现一个Workflow类
class LockTest : public BaseObject, public IWorkflow
{
private:
    class LockTestVars : public Vars
    {
    public:
        SPADAS_VARS(LockTest, Vars);

        ULong startCPUTick;
        ULong intervalSum;
        UInt intervalCount;
        Lock lock;
        
        LockTestVars(Bool spin) : startCPUTick(0), intervalSum(0), intervalCount(0), lock(spin) // Specify weather to use a spin lock / 指定是否使用自旋锁
        {}
    };

public:
    SPADAS_CLASS("LockTest", LockTestVars)

    LockTest(Bool spin) : BaseObject(new LockTestVars(spin))
    {}

    OptionalDouble getAverageInterval() // Microseconds / 微秒
    {
        if (var()->intervalCount == 0) return OptionalDouble();
        else return (Double)var()->intervalSum / var()->intervalCount * 1000000 / Timer::cpuTicksPerSecond();
    }

private:
	Array<String> getThreadNames() override
	{
		return Array<String>::create(2, "trigger_thread", "wait_thread"); // Two threads, one for triggering, another for waiting / 两个线程，一个负责触发，一个等待
	}
	void onThreadLoop(UInt threadIndex, Flag shouldEnd) override
	{
		if (threadIndex == 0)
		{
            LockProxy lock(var()->lock);
			system::wait(3);
			var()->startCPUTick = Timer::cpuTick();
		}
		else
		{
            LockProxy lock(var()->lock);
            if (var()->startCPUTick != 0)
            {
                var()->intervalSum += Timer::cpuTick() - var()->startCPUTick;
                var()->intervalCount++;
                var()->startCPUTick = 0;
            }
		}
	}
};

// Main function / 主函数
void exampleLock()
{
    // Test realtime performance of non-spin locks / 测试非自旋锁的实时性能
    LockTest normalLockTest(FALSE);
    auto threads = Threads::start(normalLockTest);
    system::wait(50);
    threads.stop();

    // Test realtime performance of spin locks / 测试自旋锁的实时性能
    LockTest spinLockTest(TRUE);
    threads = Threads::start(spinLockTest);
    system::wait(50);
    threads.stop();

    // Print result / 打印测试结果
    console::print(catAll("Normal lock interval: ", normalLockTest.getAverageInterval(), "us"));
    console::print(catAll("Spin lock interval: ", spinLockTest.getAverageInterval(), "us"));
}