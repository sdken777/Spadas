
#include "spadas.h"
#include "console.h"

using namespace console_internal;

LoggerManager::LoggerManager()
{
	loggers = new Interface<ILogger>*[65536];
	utility::memorySet(0, loggers, 65536 * sizeof(Pointer));
}
LoggerManager::~LoggerManager()
{
	for (UInt i = 0; i < 65536; i++)
	{
		if (loggers[i]) delete[] loggers[i];
	}
	delete[] loggers;
}
Interface<ILogger> LoggerManager::useLogger(UInt threadID, Interface<ILogger> target)
{
	UInt group = threadID >> 16, index = threadID & 0xffff;
	if (!loggers[group])
	{
		lock.enter();
		if (!loggers[group]) loggers[group] = new Interface<ILogger>[65536];
		lock.leave();
	}

	Interface<ILogger>& loggerRef = loggers[group][index];
	Interface<ILogger> output = loggerRef;
	loggerRef = target;
	return output;
}
void LoggerManager::clearLogger(UInt threadID)
{
	UInt group = threadID >> 16, index = threadID & 0xffff;
	if (!loggers[group]) return;

	loggers[group][index] = Interface<ILogger>();
}
Bool LoggerManager::print(UInt threadID, String message)
{
	UInt group = threadID >> 16, index = threadID & 0xffff;
	if (!loggers[group])
	{
		lock.enter();
		if (!loggers[group]) loggers[group] = new Interface<ILogger>[65536];
		lock.leave();
	}

	Interface<ILogger>& loggerRef = loggers[group][index];
	if (loggerRef.isNull()) return FALSE;

	loggerRef->print(message);
	return TRUE;
}