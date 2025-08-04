
#include "spadas.h"

namespace console_internal
{
	using namespace spadas;

    // default console and console interface
	class DefaultConsole : public EmptyObject, public IConsole
	{
	public:
		DefaultConsole();
		void print(String text, Enum<MessageLevel> level) override;
		Bool supportScan() override;
		String scan() override;
		Bool supportCheckKey() override;
		Enum<Key> checkKey() override;
	};
	extern Interface<IConsole> cs;

	// logger manager
	class LoggerManager
	{
	public:
		LoggerManager();
		~LoggerManager();
		Interface<ILogger> useLogger(UInt threadID, Interface<ILogger> target);
		void clearLogger(UInt threadID);
		Bool print(UInt threadID, String message);
	private:
		Interface<ILogger>** loggers;
		Lock lock;
	};
	extern LoggerManager lm;
}
