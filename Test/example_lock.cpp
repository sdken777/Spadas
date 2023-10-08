
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

// 实现一个Workflow类
class LockTestVars : public Vars
{
public:
    ULong startCPUTick;
    ULong intervalSum;
    UInt intervalCount;
    Lock lock;
    LockTestVars(Bool spin) : startCPUTick(0), intervalSum(0), intervalCount(0), lock(spin) // 指定是否使用自旋锁
    {}
};

class LockTest : public Object<LockTestVars>, public IWorkflow
{
public:
    LockTest(Bool spin) : Object<LockTestVars>(new LockTestVars(spin), TRUE)
    {}

    OptionalDouble getAverageInterval() // 微秒
    {
        if (vars->intervalCount == 0) return OptionalDouble();
        else return (Double)vars->intervalSum / vars->intervalCount * 1000000 / Timer::cpuTicksPerSecond();
    }

private:
	Array<String> getThreadNames() override
	{
		return Array<String>::create(2, "trigger_thread", "wait_thread"); // 两个线程，一个负责触发，一个等待
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

// 主函数
void exampleLock()
{
    // 测试非自旋锁的实时性能
    LockTest normalLockTest(FALSE);
    auto threads = Threads::start(normalLockTest);
    system::wait(50);
    threads.stop();

    // 测试自旋锁的实时性能
    LockTest spinLockTest(TRUE);
    threads = Threads::start(spinLockTest);
    system::wait(50);
    threads.stop();

    // 打印测试结果
    console::print("Normal lock interval: " cat normalLockTest.getAverageInterval() cat "us");
    console::print("Spin lock interval: " cat spinLockTest.getAverageInterval() cat "us");
}