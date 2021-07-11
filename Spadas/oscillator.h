
namespace spadas_internal
{
	void sleepTime(UInt time);
	
	class Oscillator : public Object<class OscillatorVars>
	{
	public:
        Oscillator();
		void add(Flag trigger, UInt period, Bool disposable, Bool toSet);
		void remove(Flag trigger);
		void pulse();
	};

	class OscillatorManager
	{
	public:
		OscillatorManager();
		~OscillatorManager();
		Oscillator *obj();
	private:
		Lock lock;
		Oscillator *oscillator;
	};
	extern OscillatorManager om;
}