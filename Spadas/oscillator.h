
namespace spadas_internal
{
	using namespace spadas;

	void sleepTime(UInt time);
	
	enum class DelayDeleteTimeout
	{
		TenSeconds = 0,
		Count = 1,
	};

	class Oscillator : public Object<class OscillatorVars>
	{
	public:
        Oscillator();
		void add(Flag trigger, UInt period, Bool disposable, Bool toSet);
		void remove(Flag trigger);
		void pulse();
		void delayDelete(BaseObject obj, DelayDeleteTimeout timeout);
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