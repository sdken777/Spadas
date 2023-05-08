
#include "spadas.h"

#include "oscillator.h"
#include "console.h"

namespace spadas
{
    using namespace spadas_internal;
    
	class ThreadsVars : public Vars
	{
	public:
    	SPADAS_VARS_DEF(Threads, Vars)

        Interface<IWorkflow> workflow;
        WorkflowStatus status;
        Array<Timer> timers;
		Barrier barrier;
		Array<Bool> beginRet;
		Flag allBegin, allEnd;
        Flag shouldEnd;
        Lock lock;

		ThreadsVars() : lock(TRUE)
		{}
	};
    
    class ThreadsManager
    {
    private:
        ListNode<Threads> threadsCircle0;
        UInt workflowCount;
        Lock lock;
        
    public:
        ThreadsManager()
        {
            threadsCircle0.joinNext(threadsCircle0);
            workflowCount = 1;
        }
        ~ThreadsManager()
        {
			SPADAS_ERROR_PASS(!stopAllThreads(1200));
        }
		Bool/* 超时返回FALSE */ stopAllThreads(UInt timeout)
		{
			Array<Threads> threads = getAllThreads();
			if (threads.isEmpty()) return TRUE;

			Array<Bool> stopped(threads.size(), FALSE);

			UInt currentThreadID = Threads::getCurrentThreadID();
			for (UInt i = 0; i < threads.size(); i++)
			{
				Bool skip = FALSE;
				WorkflowStatus status = threads[i].getWorkflowStatus();
				for (UInt n = 0; n < status.threadStatus.size(); n++)
				{
					auto targetThreadID = status.threadStatus[n].threadID;
					if (targetThreadID.isValid() && targetThreadID.value() == currentThreadID)
					{
						skip = TRUE;
						break;
					}
				}

				if (skip) stopped[i] = TRUE;
				else threads[i].stopAsync();
			}

			Timer timer;
			while (timer.check() < timeout)
			{
				sleepTime(10);

				for (UInt i = 0; i < threads.size(); i++)
				{
					if (stopped[i]) continue;
					if (threads[i].stop(0)) stopped[i] = TRUE;
				}

				Bool allStopped = TRUE;
				for (UInt i = 0; i < stopped.size(); i++)
				{
					if (!stopped[i]) allStopped = FALSE;
				}
				if (allStopped) return TRUE;
			}

			return FALSE;
		}
        UInt newWorkflowID()
        {
            lock.enter();
            UInt out = workflowCount++;
            if (workflowCount >= 0x10000000) workflowCount = 1;
            lock.leave();
            return out;
        }
        Threads getThreads(Interface<IWorkflow> workflow)
        {
            Threads out;
            lock.enter();
            ListNode<Threads> currentNode = threadsCircle0.next();
            while (currentNode != threadsCircle0)
            {
                if (currentNode.value().getWorkflow() == workflow &&
					currentNode.value().getWorkflowStatus().isActive)
                {
                    out = currentNode.value();
                    break;
                }
                currentNode = currentNode.next();
            }
            lock.leave();
            return out;
        }
        Array<Threads> getAllThreads()
        {
			ArrayX<Threads> buf;
            lock.enter();
            ListNode<Threads> currentNode = threadsCircle0.next();
            while (currentNode != threadsCircle0)
            {
                buf[buf.size()] = currentNode.value();
                currentNode = currentNode.next();
            }
            lock.leave();
            return buf.toArray(Region(0, buf.size()));
        }
        void addThreads(Threads threads)
        {
            lock.enter();
            threadsCircle0.insertNext(threads);
            lock.leave();
        }
        void remove(UInt workflowID)
        {
			Bool removed = FALSE;
			while (TRUE)
			{
				lock.enter();
				ListNode<Threads> currentNode = threadsCircle0.next();
				while (currentNode != threadsCircle0)
				{
					if (currentNode.value().getWorkflowStatus().workflowID == workflowID)
					{
						currentNode = currentNode.previous();
						currentNode.removeNext();
						removed = TRUE;
						break;
					}
					currentNode = currentNode.next();
				}
				lock.leave();
				
				if (removed) break;
				else sleepTime(10);
			}
        }
    };
    ThreadsManager threadsManager;
    
    const Float UPDATE_EXP_COEF = 3.0f;
	Float updateTime(Float averageTime, Float newTime)
	{
		if (averageTime == 0.0f) return newTime;
		Float tmp = math::exp(-UPDATE_EXP_COEF * newTime);
		return averageTime * tmp + newTime * (1.0f - tmp);
	}
}

using namespace spadas;
using namespace spadas_internal;

const String spadas::Threads::TypeName = "spadas.Threads";

#if defined(SPADAS_ENV_WINDOWS)

#include <process.h>
#include <windows.h>
#undef max
#undef min

UInt Threads::getCurrentThreadID()
{
	return (UInt)GetCurrentThreadId();
}

UInt __stdcall threadFunc(Pointer param);
void threadCreate(ThreadsVars *vars)
{
	HANDLE thisHandle = (HANDLE)_beginthreadex(NULL, 0, threadFunc, vars, 0, NULL);
	if (thisHandle)
	{
		SetThreadPriority(thisHandle, THREAD_PRIORITY_NORMAL);
		CloseHandle(thisHandle);
	}
}

UInt __stdcall threadFunc(Pointer param)

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <pthread.h>
#include <unistd.h>

#if defined(SPADAS_ENV_LINUX)
#include <linux/unistd.h>
UInt Threads::getCurrentThreadID()
{
	return syscall(__NR_gettid);
}
#endif
#if defined(SPADAS_ENV_MACOS)
UInt Threads::getCurrentThreadID()
{
	return (UInt)pthread_mach_thread_np(pthread_self());
}
#endif

Pointer threadFunc(Pointer param);
void threadCreate(ThreadsVars *vars)
{
	sched_param sched;
	sched.sched_priority = 50;
	
	pthread_attr_t threadAttri;
	pthread_attr_init(&threadAttri);
	pthread_attr_setschedparam(&threadAttri, &sched);
	pthread_attr_setdetachstate(&threadAttri, PTHREAD_CREATE_DETACHED);
	
	pthread_t thread;
	pthread_create(&thread, &threadAttri, threadFunc, vars);
	
	pthread_attr_destroy(&threadAttri);
}

Pointer threadFunc(Pointer param)

#endif

// threadFunc
{
    ThreadsVars *vars = (ThreadsVars*)param;

    // get thread index
	vars->lock.enter();
    UInt threadIndex = UINF;
    for (UInt i = 0; i < vars->status.threadStatus.size(); i++)
    {
        if ((volatile Bool)vars->status.threadStatus[i].isActive == FALSE)
        {
			vars->status.threadStatus[i].isActive = TRUE;
            threadIndex = i;
            break;
        }
    }

    // get thread ID
    ThreadStatus& status = vars->status.threadStatus[threadIndex];
	UInt curThreadID = Threads::getCurrentThreadID();
    status.threadID = curThreadID;
	vars->lock.leave();
	
	// thread initialization
	Bool ret = vars->workflow->onThreadBegin(threadIndex);
	if (threadIndex <= vars->beginRet.size()) vars->beginRet[threadIndex] = ret;

	// begin synchronization
	vars->barrier.against(Flag());
	if (threadIndex == 0) vars->allBegin.set();

	// main loop
	while (ret)
	{
		// run onLoop and record time
		vars->timers[threadIndex].start();
		vars->workflow->onThreadLoop(threadIndex, vars->shouldEnd);
		Float thisProcessTime = (Float)vars->timers[threadIndex].check();

		// update status and get time interval
		vars->lock.enter();
		status.averageProcessTime = status.averageProcessTime.isValid() ? updateTime(status.averageProcessTime.value(), thisProcessTime) : thisProcessTime;
		UInt timeInterval;
		if (status.userTimeInterval.isValid()) timeInterval = status.userTimeInterval.value();
		else timeInterval = vars->workflow->getTimeInterval(threadIndex);
		vars->lock.leave();

		// wait
		if (timeInterval < 1000)
		{
			system::wait(timeInterval);
		}
		else
		{
			Timer longWaitTimer;
			while (!vars->shouldEnd.check())
			{
				// get time interval
				vars->lock.enter();
				if (status.userTimeInterval.isValid()) timeInterval = status.userTimeInterval.value();
				else timeInterval = vars->workflow->getTimeInterval(threadIndex);
				vars->lock.leave();

				// wait
				Float longWaitTimerCheck = (Float)longWaitTimer.check();
				if (longWaitTimerCheck + 1000.0f < (Float)timeInterval)
				{
					system::wait(1000);
				}
				else
				{
					if (longWaitTimerCheck < (Float)timeInterval) system::wait((UInt)((Float)timeInterval - longWaitTimerCheck));
					break;
				}
			}
		}

		// see the shouldEnd flag
		if (vars->shouldEnd.check()) break;
	}

	// end synchronization 1
	vars->barrier.against(Flag());

	// thread deinitialization
	vars->workflow->onThreadEnd(threadIndex);
    
    // set isActive to FALSE
    status.isActive = FALSE;
	
	// end synchronization 2
	vars->barrier.against(Flag());

	// remove from ThreadsManager
    if (threadIndex == 0)
    {
        vars->status.isActive = FALSE;
		vars->allEnd.set();

		sleepTime(1);
		threadsManager.remove(vars->status.workflowID);
    }

	// clear thread logger
	lm.clearLogger(curThreadID);
    
	return 0;
}

Threads::Threads()
{
}

Bool Threads::isRunning()
{
	if (!vars) return FALSE;
	vars->lock.enter();
	Bool isRunning = vars->status.isActive;
	vars->lock.leave();
	return isRunning;
}

Bool Threads::stop(UInt timeout)
{
	if (!vars) return TRUE;
    if ((volatile Bool)vars->status.isActive == FALSE) return TRUE;
    
	vars->shouldEnd.set();
	if (timeout == 0) return FALSE;

    Timer timer;
    while (timer.check() < (Double)timeout)
    {
        if (vars->allEnd.waitSet(10)) return TRUE;
    }
    return FALSE;
}

void Threads::stopAsync()
{
	if (!vars) return;
	if ((volatile Bool)vars->status.isActive == FALSE) return;

	vars->shouldEnd.set();
}

Interface<IWorkflow> Threads::getWorkflow()
{
	return vars ? vars->workflow : Interface<IWorkflow>();
}

WorkflowStatus Threads::getWorkflowStatus()
{
	if (!vars) return WorkflowStatus();
    vars->lock.enter();
    WorkflowStatus out = vars->status;
    out.workflowName = vars->status.workflowName.clone();
    out.threadStatus = vars->status.threadStatus.clone();
    for (UInt i = 0; i < out.threadStatus.size(); i++)
    {
        out.threadStatus[i].threadName = vars->status.threadStatus[i].threadName.clone();
        out.threadStatus[i].currentLoopTime = (Float)vars->timers[i].check();
    }
    vars->lock.leave();
	return out;
}

void Threads::useUserTimeInterval(UInt threadIndex, UInt interval)
{
	if (!vars) return;
    UInt nThreads = vars->status.threadStatus.size();
    if (threadIndex >= nThreads) return;
	SPADAS_ERROR_RETURN(!vars->workflow->supportUserTimeInterval(threadIndex));

	vars->lock.enter();
    vars->status.threadStatus[threadIndex].userTimeInterval = interval;
	vars->lock.leave();
}

void Threads::useDefaultTimeInterval(UInt threadIndex)
{
	if (!vars) return;
    UInt nThreads = vars->status.threadStatus.size();
    if (threadIndex >= nThreads) return;
	vars->lock.enter();
	vars->status.threadStatus[threadIndex].userTimeInterval = Optional<UInt>();
	vars->lock.leave();
}

Threads Threads::start(Interface<IWorkflow> workflow, Array<Bool>& threadsRet)
{
	SPADAS_ERROR_RETURNVAL(!workflow.isValid(), Threads());

    Array<String> threadNames = workflow->getThreadNames();
    UInt nThreads = threadNames.size();
	SPADAS_ERROR_RETURNVAL(nThreads == 0, Threads());

	SPADAS_ERROR_RETURNVAL(threadsManager.getThreads(workflow).isValid(), Threads());

    String workflowName = workflow->getWorkflowName();
	if (workflowName.isEmpty()) workflowName = "unknown_workflow";

    Threads out;
	out.setVars(new ThreadsVars(), TRUE);
    out.vars->workflow = workflow;
    out.vars->timers = Array<Timer>(nThreads);
	out.vars->barrier.setStrength(nThreads);
    out.vars->status.isActive = TRUE;
    out.vars->status.workflowID = threadsManager.newWorkflowID();
    out.vars->status.workflowName = workflowName;
	out.vars->beginRet = Array<Bool>(nThreads, FALSE);
    
    Array<ThreadStatus> threadsStatus(nThreads);
    out.vars->status.threadStatus = threadsStatus;
    for (UInt i = 0; i < nThreads; i++)
    {
		threadsStatus[i].isActive = FALSE;
		threadsStatus[i].threadName = threadNames[i].isEmpty() ? "unknown_thread" : threadNames[i];
        threadCreate(out.vars);
    }
	out.vars->allBegin.waitSet();

	threadsRet = out.vars->beginRet.clone();
    
    threadsManager.addThreads(out);
	return out;
}

Threads Threads::start(Interface<IWorkflow> workflow)
{
	Array<Bool> dummy;
	return start(workflow, dummy);
}

void Threads::run(Interface<IWorkflow> workflow, Flag interrupt)
{
	SPADAS_ERROR_RETURN(!workflow.isValid());
    
    String workflowName = workflow->getWorkflowName();
	if (workflowName.isEmpty()) workflowName = "unknown_workflow";

    Array<String> threadNames = workflow->getThreadNames();
    UInt nThreads = threadNames.size();
	SPADAS_ERROR_RETURN(nThreads != 1);
    
    String threadName = threadNames[0];
	if (threadName.isEmpty()) threadName = "unknown_thread";
    
    if (!workflow->onThreadBegin(0))
    {
		SPADAS_ERROR_MSG("!workflow->onThreadBegin(0)");
        workflow->onThreadEnd(0);
        return;
    }

    while (TRUE)
    {
        workflow->onThreadLoop(0, interrupt);
        if (interrupt.check()) break;
        system::wait(workflow->getTimeInterval(0));
    }
    
    workflow->onThreadEnd(0);
}

Bool Threads::stopAllThreads(UInt timeout)
{
	return threadsManager.stopAllThreads(timeout);
}

Threads Threads::getThreadsOf(Interface<IWorkflow> workflow)
{
	SPADAS_ERROR_RETURNVAL(workflow.isNull(), Threads());
	return threadsManager.getThreads(workflow);
}

Array<Threads> Threads::getAllThreads()
{
	return threadsManager.getAllThreads();
}
