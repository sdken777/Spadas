
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
}
