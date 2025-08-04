
#if defined(SPADAS_ENV_WINDOWS)
#include <process.h>
#include <windows.h>
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
#include <pthread.h>
#include <unistd.h>
#if defined(SPADAS_ENV_LINUX)
#include <linux/unistd.h>
#endif
#endif

#include "spadas.h"
#include "console.h"

namespace spadas
{
	struct TaskThreadContext
	{
		Flag valid;
		Interface<ITask> task;
		Interface<ILogger> logger;
		Flag shouldEnd;

		TaskThreadContext() {}
	};

	class TaskManagerVars : public Vars
	{
	public:
		SPADAS_VARS(TaskManager, Vars)

		ListNode<TaskThreadContext> threads;
		Lock threadsLock;

		void stopAll(UInt timeout);

		TaskManagerVars()
		{
			threads.joinNext(threads);
		}
		~TaskManagerVars()
		{
			stopAll(1200);
			threads.collapse();
		}
		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
	};
}

using namespace spadas;
using namespace console_internal;

#if defined(SPADAS_ENV_WINDOWS)

UInt __stdcall taskThreadFunc(Pointer param);
void taskThreadCreate(TaskManagerVars *vars)
{
	HANDLE thisHandle = (HANDLE)_beginthreadex(NULL, 0, taskThreadFunc, vars, 0, NULL);
	SetThreadPriority(thisHandle, THREAD_PRIORITY_NORMAL);
	CloseHandle(thisHandle);
}

UInt __stdcall taskThreadFunc(Pointer param)

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

Pointer taskThreadFunc(Pointer param);
void taskThreadCreate(TaskManagerVars *vars)
{
	sched_param sched;
	sched.sched_priority = 50;

	pthread_attr_t threadAttri;
	pthread_attr_init(&threadAttri);
	pthread_attr_setschedparam(&threadAttri, &sched);
	pthread_attr_setdetachstate(&threadAttri, PTHREAD_CREATE_DETACHED);

	pthread_t thread;
	pthread_create(&thread, &threadAttri, taskThreadFunc, vars);

	pthread_attr_destroy(&threadAttri);
}

Pointer taskThreadFunc(Pointer param)

#endif

// taskThreadFunc
{
	TaskManagerVars *localVars = (TaskManagerVars*)param;

	ListNode<TaskThreadContext> targetNode;
	while (!targetNode->task.isValid())
	{
		localVars->threadsLock.enter();
		ListNode<TaskThreadContext> currentNode = localVars->threads.next();
		while (currentNode != localVars->threads)
		{
			if (!currentNode->valid.check())
			{
				targetNode = currentNode;
				break;
			}
			currentNode = currentNode.next();
		}
		localVars->threadsLock.leave();
		system::wait(1);
	}

	if (targetNode->logger.isValid()) lm.useLogger(Threads::getCurrentThreadID(), targetNode->logger);

	targetNode->valid.set();
	targetNode->task->onRunTask(targetNode->shouldEnd);

	localVars->threadsLock.enter();
	targetNode.removeSelf();
	localVars->threadsLock.leave();

	lm.clearLogger(Threads::getCurrentThreadID());

	return 0;
}

TaskManager::TaskManager() : BaseObject(new TaskManagerVars)
{

}

void TaskManager::start(Interface<ITask> task, Interface<ILogger> logger)
{
	SPADAS_ERROR_RETURN(!task.isValid());

	var()->threadsLock.enter();

	ListNode<TaskThreadContext> currentNode = var()->threads.next();
	while (currentNode != var()->threads)
	{
		if (currentNode->task == task)
		{
			var()->threadsLock.leave();
			return;
		}
		currentNode.goNext();
	}

	var()->threadsLock.leave();

	TaskThreadContext newContext;
	newContext.task = task;
	newContext.logger = logger;

	var()->threadsLock.enter();
	var()->threads.insertPrevious(newContext);
	var()->threadsLock.leave();

	taskThreadCreate(var());

	newContext.valid.waitSet();
}

void TaskManager::start(Interface<ITask> task)
{
	start(task, Interface<ILogger>());
}

Bool TaskManager::stop(Interface<ITask> task, UInt timeout)
{
	SPADAS_ERROR_RETURNVAL(!task.isValid(), TRUE);

	Bool targetNodeFound = FALSE;
	ListNode<TaskThreadContext> targetNode;

	var()->threadsLock.enter();
	{
		ListNode<TaskThreadContext> currentNode = var()->threads.next();
		while (currentNode != var()->threads)
		{
			if (currentNode->task == task)
			{
				targetNodeFound = TRUE;
				targetNode = currentNode;
				break;
			}
			currentNode.goNext();
		}
	}
	var()->threadsLock.leave();
	
	if (!targetNodeFound) return TRUE;

	targetNode->shouldEnd.set();

	Timer timer;
	while (timer.check() < (Double)timeout)
	{
		Bool found = FALSE;
		var()->threadsLock.enter();
		{
			ListNode<TaskThreadContext> currentNode = var()->threads.next();
			while (currentNode != var()->threads)
			{
				if (currentNode->task == task)
				{
					found = TRUE;
					break;
				}
				currentNode.goNext();
			}
		}
		var()->threadsLock.leave();

		if (!found) return TRUE;

		system::wait(10);
	}

	return FALSE;
}

Array<Interface<ITask> > TaskManager::getTasks()
{
	ArrayX<Interface<ITask> > list;
	LockProxy lock(var()->threadsLock);

	ListNode<TaskThreadContext> currentNode = var()->threads.next();
	while (currentNode != var()->threads)
	{
		list.append(currentNode->task);
		currentNode = currentNode.next();
	}

	return list.toArray();
}

Bool TaskManager::isTaskRunning(Interface<ITask> task)
{
	SPADAS_ERROR_RETURNVAL(task.isNull(), FALSE)

	ArrayX<Interface<ITask> > list;
	LockProxy lock(var()->threadsLock);

	ListNode<TaskThreadContext> currentNode = var()->threads.next();
	while (currentNode != var()->threads)
	{
		if (currentNode->task == task) return TRUE;
		currentNode.goNext();
	}

	return FALSE;
}

Bool TaskManager::waitAll(Flag interrupt)
{
	while (!interrupt.check())
	{
		LockProxy lock(var()->threadsLock);
		if (var()->threads.next() == var()->threads) return TRUE;
		system::wait(10);
	}
	return FALSE;
}

void TaskManager::stopAll(UInt timeout)
{
	var()->stopAll(timeout);
}

void TaskManagerVars::stopAll(UInt timeout)
{
	threadsLock.enter();

	ListNode<TaskThreadContext> currentNode = threads.next();
	while (currentNode != threads)
	{
		currentNode->shouldEnd.set();
		currentNode = currentNode.next();
	}

	threadsLock.leave();

	Timer timer;
	while (timer.check() < (Double)timeout)
	{
		threadsLock.enter();
		if (threads.next() == threads)
		{
			threadsLock.leave();
			return;
		}
		threadsLock.leave();
		system::wait(10);
	}
}
