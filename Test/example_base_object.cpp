#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

// Declaration of MyObject / MyObject声明
class MyObject : public BaseObject
{
public:
	SPADAS_CLASS("MyObject", MyObjectVars) // Declare type name to support type conversion / 声明类型名称以支持类型转换
	MyObject();
	MyObject(Double number);
	Double getNumber();
};

// Implementation of MyObject / MyObject实现
class MyObjectVars : public Vars
{
public:
	SPADAS_VARS(MyObject, Vars); // Declare Vars to support type conversion / 定义Vars关系以支持类型转换
	Double number;
	MyObjectVars(Double val) : number(val) {}
	String toString() override { return "My object with number: " cat number; }
};

MyObject::MyObject() : BaseObject(new MyObjectVars(0))
{}

MyObject::MyObject(Double number) : BaseObject(new MyObjectVars(number))
{}

Double MyObject::getNumber()
{
	return var()->number;
}

// Main function / 主函数
void exampleBaseObject()
{
	// Assign different types of objects to BaseObject array, and convert one object back to MyObject / 将不同类型对象赋值至BaseObject数组，并将其中一个转换回MyObject
	console::print("- 1 -");

	auto objs = Array<BaseObject>(3);
	objs[0] = String("Hello world");
	objs[1] = Timer();
	objs[2] = MyObject(100);

	console::print(String::merge(objs, ", "));

	MyObject myObj;
	if (objs[2].is<MyObject>(myObj))
	{
		console::print("The number is: " cat myObj.getNumber());
	}

	// Assign different types of new objects to BaseObject pointers in traditional way, and convert one object back to MyObject / 用传统方式将new出来的不同类型对象赋值至BaseObject指针，并将其中一个转换回MyObject
	console::print("- 2 -");

	BaseObject *obj0 = new String("Hello world");
	BaseObject *obj1 = new Timer();
	BaseObject *obj2 = new MyObject(100);

	console::print(catAll(*obj0, ", ", *obj1, ", ", *obj2));

	MyObject *myObjPtr = (MyObject*)obj2;
	console::print("The number is: " cat myObjPtr->getNumber());

	delete obj0;
	delete obj1;
	delete obj2;
}