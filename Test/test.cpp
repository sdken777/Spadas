
#include "spadas.h"
#include <vector>

using namespace spadas;

// Task test /////////////////////////////////////////////////////////////////////
class TestThreads : public EmptyObject, public ITask
{
private:
	void onRunTask(Flag shouldEnd)
	{
		shouldEnd.waitSet();
		console::print("thread over");
	}
};

// Cast test (declaration) /////////////////////////////////////////////////////////////////////
class CalcClass : public Object<class CalcClassVars>
{
public:
	static const String TypeName;
	Double calc(Double a, Double b);
};

class AddClass : public Object<class AddClassVars>
{
public:
	static const String TypeName;
	AddClass();
	ULong getID();
};

class MultiClass : public Object<class MultiClassVars>
{
public:
	static const String TypeName;
	MultiClass();
	String getID();
};

// Cast test (vars implementation) /////////////////////////////////////////////////////////////////////

class CalcClassVars : public Vars
{
public:
	SPADAS_VARS_DEF(CalcClass, Vars)
	virtual Double calc(Double a, Double b) { return 0; }
};

class AddClassVars : public CalcClassVars
{
public:
	SPADAS_VARS_DEF(AddClass, CalcClassVars)
	ULong id;
	AddClassVars(ULong id) : id(id) {}
	virtual Double calc(Double a, Double b) { return a + b; }
};

class MultiClassVars : public CalcClassVars
{
public:
	SPADAS_VARS_DEF(MultiClass, CalcClassVars)
	String id;
	MultiClassVars(String id) : id(id) {}
	virtual Double calc(Double a, Double b) { return a * b; }
};

// Cast test (class implementation) /////////////////////////////////////////////////////////////////////
const String CalcClass::TypeName = "CalcClass";
const String AddClass::TypeName = "AddClass";
const String MultiClass::TypeName = "MultiClass";

Double CalcClass::calc(Double a, Double b)
{ return vars ? vars->calc(a, b) : 0; }

AddClass::AddClass() : Object<AddClassVars>(new AddClassVars(12345), TRUE)
{}

ULong AddClass::getID()
{ return vars->id; }

MultiClass::MultiClass() : Object<MultiClassVars>(new MultiClassVars("world"), TRUE)
{}

String MultiClass::getID()
{ return vars->id; }

// Main class /////////////////////////////////////////////////////////////////////
class Test
{
public:
	void taskTest()
	{
		TaskManager tm;
		TestThreads task;

		UInt count = 0;
		while (console::checkKey() == Key::None)
		{
			tm.start(task);
			tm.stop(task);
			tm.start(TestThreads());
			console::print((String)count++ + ":" + tm.getTasks().size());
		}

		tm.stopAll();

		console::print(SS"final:" + tm.getTasks().size());
	}

	void existTest()
	{
		console::print(SS"a file exist: " + Path("/home/ken/Desktop/a").exist());
		console::print(SS"a folder exist: " + Path("/home/ken/Desktop/a/").exist());
		console::print(SS"b file exist: " + Path("/home/ken/Desktop/b").exist());
		console::print(SS"b folder exist: " + Path("/home/ken/Desktop/b/").exist());
	}

	void pathTest()
	{
		String pathes[4];
		pathes[0] = "/home/ken/folder/";
		pathes[1] = "/home/ken/file";
		pathes[2] = "C:\\Users\\Ken\\folder\\";
		pathes[3] = "C:\\Users\\Ken\\file";

		for (UInt i = 0; i < 4; i++)
		{
			Path path(pathes[i]);
			if (path.isValid()) console::print(pathes[i] + ": " + path.fullPath());
			else console::print(pathes[i] + ": invalid");
		}
	}

	void deriveTest()
	{
		Array<BaseObject> objs = Array<BaseObject>::create(2, AddClass().as<BaseObject>(), MultiClass().as<BaseObject>());
		for (UInt i = 0; i < 2; i++)
		{
			CalcClass myCalcObj = objs[i].as<CalcClass>();
			console::print(myCalcObj.getTypeName() + ": " + myCalcObj.calc(2, 3));

			if (myCalcObj.is<AddClass>()) console::print(myCalcObj.as<AddClass>().getID());
			if (myCalcObj.is<MultiClass>()) console::print(myCalcObj.as<MultiClass>().getID());
		}
	}

	void arrayTest()
	{
		Timer timer;

		console::waitKey();
		{
			UInt* arr = new UInt[ARRAY_SIZE_LIMIT];
			timer.start();
			for (UInt i = 0; i < ARRAY_SIZE_LIMIT; i++) arr[i] = i;
			delete[] arr;
		}
		console::print(SS"[]: " + timer.check());

		console::waitKey();
		{
			Array<UInt> arr(ARRAY_SIZE_LIMIT);
			timer.start();
			for (auto elem = arr.firstElem(); elem.valid(); ++elem) elem = elem.index();
		}
		console::print(SS"Array: " + timer.check());

		console::waitKey();
		{
			ArrayX<UInt> arr;
			timer.start();
			for (UInt i = 0; i < ARRAY_SIZE_LIMIT; i++) arr.append(i);
		}
		console::print(SS"ArrayX: " + timer.check());

		console::waitKey();
		{
			ArrayX<UInt> arr(ARRAYX_SEGMENT_LIMIT, 0);
			timer.start();
			for (UInt i = 0; i < ARRAY_SIZE_LIMIT; i++) arr.append(i);
		}
		console::print(SS"ArrayX(max): " + timer.check());

		console::waitKey();
		{
			std::vector<UInt> arr;
			timer.start();
			for (UInt i = 0; i < ARRAY_SIZE_LIMIT; i++) arr.push_back(i);
		}
		console::print(SS"vector: " + timer.check());

		console::waitKey();
		{
			List<UInt> arr;
			timer.start();
			UInt count = ARRAY_SIZE_LIMIT / 10;
			for (UInt i = 0; i < count; i++) arr.addToTail(i);
		}
		console::print(SS"List: " + (timer.check() * 10));

		console::waitKey();
	}
};

int main(int argc, char* argv[])
{
	Test().taskTest();
	Test().pathTest();
	Test().deriveTest();
	Test().arrayTest();

	console::print("over");
	return 0;
}