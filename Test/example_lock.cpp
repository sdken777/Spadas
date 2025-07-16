#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

// Implement a workflow class / 实现一个Workflow类
class LockTestVars : public Vars
{
public:
    ULong startCPUTick;
    ULong intervalSum;
    UInt intervalCount;
    Lock lock;
    LockTestVars(Bool spin) : startCPUTick(0), intervalSum(0), intervalCount(0), lock(spin) // Specify weather to use a spin lock / 指定是否使用自旋锁
    {}
};

class LockTest : public Object<LockTestVars>, public IWorkflow
{
public:
    LockTest(Bool spin) : Object<LockTestVars>(new LockTestVars(spin), TRUE)
    {}

    OptionalDouble getAverageInterval() // Microseconds / 微秒
    {
        if (vars->intervalCount == 0) return OptionalDouble();
        else return (Double)vars->intervalSum / vars->intervalCount * 1000000 / Timer::cpuTicksPerSecond();
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
            LockProxy lock(vars->lock);
			system::wait(3);
			vars->startCPUTick = Timer::cpuTick();
		}
		else
		{
            LockProxy lock(vars->lock);
            if (vars->startCPUTick != 0)
            {
                vars->intervalSum += Timer::cpuTick() - vars->startCPUTick;
                vars->intervalCount++;
                vars->startCPUTick = 0;
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