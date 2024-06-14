
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

// Declaration of MyObject / MyObject声明
class MyObject : public Object<class MyObjectVars>
{
public:
	SPADAS_TYPE("MyObject") // Declare type name to support type conversion / 声明类型名称以支持类型转换
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

	if (objs[2].is<MyObject>())
	{
		MyObject myObj = objs[2].as<MyObject>();
		console::print("The number is: " cat myObj.getNumber());
	}
}