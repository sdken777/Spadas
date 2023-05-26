
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

// MyObject声明
class MyObject : public Object<class MyObjectVars>
{
public:
	static const String TypeName; // 声明TypeName以支持BaseObject类型转换
	MyObject();
	MyObject(Double number);
	Double getNumber();
};

// MyObject实现
const String MyObject::TypeName = "MyObject"; // 实现TypeName以支持BaseObject类型转换

class MyObjectVars : public Vars
{
public:
	SPADAS_VARS_DEF(MyObject, Vars); // 定义Vars关系以支持BaseObject类型转换
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

// 主函数
void exampleBaseObject()
{
	// 用BaseObject数组装入不同类型对象
	console::print("- 1 -");

	auto objs = Array<BaseObject>(3);
	objs[0] = String("Hello world").as<BaseObject>();
	objs[1] = Timer().as<BaseObject>();
	objs[2] = MyObject(100).as<BaseObject>();

	console::print(String::merge(objs, ", "));

	// 转换回MyObject
	console::print("- 2 -");

	if (objs[2].is<MyObject>())
	{
		MyObject myObj = objs[2].as<MyObject>();
		console::print(SS"The number is: " + myObj.getNumber());
	}
}