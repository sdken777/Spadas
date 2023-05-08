
namespace spadas_internal
{
    // default console and console interface
	class DefaultConsole : public Object<Vars>, public IConsole
	{
	public:
		DefaultConsole();
		void print(String text, Enum<MessageLevel> level);
		Bool supportScan();
		String scan();
		Bool supportCheckKey();
		Enum<Key> checkKey();
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
