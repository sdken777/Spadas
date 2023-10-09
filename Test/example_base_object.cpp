
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

// Declaration of MyObject / MyObject声明
class MyObject : public Object<class MyObjectVars>
{
public:
	static const String TypeName; // Declare TypeName to support BaseObject conversion / 声明TypeName以支持BaseObject类型转换
	MyObject();
	MyObject(Double number);
	Double getNumber();
};

// Implementation of MyObject / MyObject实现
const String MyObject::TypeName = "MyObject"; // Implement TypeName to support BaseObject conversion / 实现TypeName以支持BaseObject类型转换

class MyObjectVars : public Vars
{
public:
	SPADAS_VARS_DEF(MyObject, Vars); // Declare Vars to support BaseObject conversion / 定义Vars关系以支持BaseObject类型转换
	Double number;
	MyObjectVars(Double val) : number(val) {}
	String toString() override { return SS "My object with number: " + number; }
};

MyObject::MyObject() : Object<MyObjectVars>(new MyObjectVars(0), TRUE)
{}

MyObject::MyObject(Double number) : Object<MyObjectVars>(new MyObjectVars(number), TRUE)
{}

Double MyObject::getNumber()
{
	return vars->number;
}

// Main function / 主函数
void exampleBaseObject()
{
	// Put objects of different types into a BaseObject array / 用BaseObject数组装入不同类型对象
	console::print("- 1 -");

	auto objs = Array<BaseObject>(3);
	objs[0] = String("Hello world").as<BaseObject>();
	objs[1] = Timer().as<BaseObject>();
	objs[2] = MyObject(100).as<BaseObject>();

	console::print(String::merge(objs, ", "));

	// Convert to MyObject / 转换回MyObject
	console::print("- 2 -");

	Optional<MyObject> myObj = objs[2].cast<MyObject>();
	if (myObj.isValid()) console::print(SS"The number is: " + myObj->getNumber());
}