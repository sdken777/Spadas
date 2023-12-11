
#include "spadas.h"
#include "console.h"

namespace spadas
{
	struct TaskThreadContext
	{
		Flag valid;
		Interface<ITask> task;
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

#include <process.h>
#include <windows.h>
#undef max
#undef min

UInt __stdcall taskThreadFunc(Pointer param);
void taskThreadCreate(TaskManagerVars *vars)
{
	HANDLE thisHandle = (HANDLE)_beginthreadex(NULL, 0, taskThreadFunc, vars, 0, NULL);
	SetThreadPriority(thisHandle, THREAD_PRIORITY_NORMAL);
	CloseHandle(thisHandle);
}

UInt __stdcall taskThreadFunc(Pointer param)

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)

#include <pthread.h>
#include <unistd.h>

#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_NILRT)
#include <linux/unistd.h>
#endif

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
	TaskManagerVars *vars = (TaskManagerVars*)param;

	ListNode<TaskThreadContext> targetNode;
	while (!targetNode.value().task.isValid())
	{
		vars->threadsLock.enter();
		ListNode<TaskThreadContext> currentNode = vars->threads.next();
		while (currentNode != vars->threads)
		{
			if (!currentNode.value().valid.check())
			{
				targetNode = currentNode;
				break;
			}
			currentNode = currentNode.next();
		}
		vars->threadsLock.leave();
		system::wait(1);
	}

	targetNode.value().valid.set();
	targetNode.value().task->onRunTask(targetNode.value().shouldEnd);

	vars->threadsLock.enter();
	targetNode.removeSelf();
	vars->threadsLock.leave();

	lm.clearLogger(Threads::getCurrentThreadID());

	return 0;
}

TaskManager::TaskManager() : Object<TaskManagerVars>(new TaskManagerVars, TRUE)
{

}

void TaskManager::start(Interface<ITask> task)
{
	SPADAS_ERROR_RETURN(!task.isValid());

	vars->threadsLock.enter();

	ListNode<TaskThreadContext> currentNode = vars->threads.next();
	while (currentNode != vars->threads)
	{
		if (currentNode.value().task == task)
		{
			vars->threadsLock.leave();
			return;
		}
		currentNode.goNext();
	}

	vars->threadsLock.leave();

	TaskThreadContext newContext;
	newContext.task = task;

	vars->threadsLock.enter();
	vars->threads.insertPrevious(newContext);
	vars->threadsLock.leave();

	taskThreadCreate(vars);

	newContext.valid.waitSet();
}

Bool TaskManager::stop(Interface<ITask> task, UInt timeout)
{
	SPADAS_ERROR_RETURNVAL(!task.isValid(), TRUE);

	Bool targetNodeFound = FALSE;
	ListNode<TaskThreadContext> targetNode;

	vars->threadsLock.enter();
	{
		ListNode<TaskThreadContext> currentNode = vars->threads.next();
		while (currentNode != vars->threads)
		{
			if (currentNode.value().task == task)
			{
				targetNodeFound = TRUE;
				targetNode = currentNode;
				break;
			}
			currentNode.goNext();
		}
	}
	vars->threadsLock.leave();
	
	if (!targetNodeFound) return TRUE;

	targetNode.value().shouldEnd.set();

	Timer timer;
	while (timer.check() < (Double)timeout)
	{
		Bool found = FALSE;
		vars->threadsLock.enter();
		{
			ListNode<TaskThreadContext> currentNode = vars->threads.next();
			while (currentNode != vars->threads)
			{
				if (currentNode.value().task == task)
				{
					found = TRUE;
					break;
				}
				currentNode.goNext();
			}
		}
		vars->threadsLock.leave();

		if (!found) return TRUE;

		system::wait(10);
	}

	return FALSE;
}

Array<Interface<ITask> > TaskManager::getTasks()
{
	ArrayX<Interface<ITask> > list;
	LockProxy lock(vars->threadsLock);

	ListNode<TaskThreadContext> currentNode = vars->threads.next();
	while (currentNode != vars->threads)
	{
		list.append(currentNode.value().task);
		currentNode = currentNode.next();
	}

	return list.toArray();
}

Bool TaskManager::isTaskRunning(Interface<ITask> task)
{
	SPADAS_ERROR_RETURNVAL(task.isNull(), FALSE)

	ArrayX<Interface<ITask> > list;
	LockProxy lock(vars->threadsLock);

	ListNode<TaskThreadContext> currentNode = vars->threads.next();
	while (currentNode != vars->threads)
	{
		if (currentNode.value().task == task) return TRUE;
		currentNode.goNext();
	}

	return FALSE;
}

Bool TaskManager::waitAll(Flag interrupt)
{
	while (!interrupt.check())
	{
		LockProxy lock(vars->threadsLock);
		if (vars->threads.next() == vars->threads) return TRUE;
		system::wait(10);
	}
	return FALSE;
}

void TaskManager::stopAll(UInt timeout)
{
	vars->stopAll(timeout);
}

void TaskManagerVars::stopAll(UInt timeout)
{
	threadsLock.enter();

	ListNode<TaskThreadContext> currentNode = threads.next();
	while (currentNode != threads)
	{
		currentNode.value().shouldEnd.set();
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
