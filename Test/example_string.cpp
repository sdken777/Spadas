
#include <stdio.h>
#include <wchar.h>
#include "spadas.h"

using namespace spadas;

void exampleString()
{
	// Data is stored in UTF8 / 数据统一使用UTF8存储
	console::print("- 1 -");

	String str = "Hello天气不错";
	console::print("console::print: " cat str);

	Array<Char> strChars = str.chars(); // Always UTF8 characters / UTF8字符
	console::print("chars字符: " cat String::merge(Array<Byte>((Byte*)strChars.data(), strChars.size()), ", "));

	Array<WChar> strWChars = str.wchars(); // It is UTF16 for Windows and UTF32 for other OS / Windows下为UTF16字符，其他操作系统都为UTF32字符
	if (sizeof(WChar) == 4) console::print("wchars字符: " cat String::merge(Array<UInt>((UInt*)strWChars.data(), strWChars.size()), ", "));
	else if (sizeof(WChar) == 2) console::print("wchars字符: " cat String::merge(Array<Word>((Word*)strWChars.data(), strWChars.size()), ", "));

	// Some structures and all classes implemented the "toString" method / 部分结构体和所有类实现toString函数
	console::print("- 2 -");

	console::print(Point2D(3, 4).toString());
	console::print(str.toString());
	console::print(Timer().toString());

	// Use "merge" method to merge into a string / 通过merge函数合并成字符串
	console::print("- 3 -");

	Array<Double> numbers(2);
	numbers[0] = 3.4;
	numbers[1] = 5.6;
	console::print(String::merge(numbers, ", "));

	Array<Point2D> points(2);
	points[0] = Point2D(3, 4);
	points[1] = Point2D(5, 6);
	console::print(String::merge(points, " | "));

	// Use "cat" and "catAll" macro to concatenate / 用cat和catAll连接字符串
	console::print("- 4 -");

	console::print("Number: " cat 123);
	console::print(catAll(1, "+", 2, "=", 3));

	Point2D pt1(1, 2), pt2(3, 5);
	console::print(catAll(pt1, "+", pt2, "=", pt1 + pt2));

	// Use StringViewer to parse large string / 用StringViewer解析大字符串
	console::print("- 5 -");

	String source = "1,2,3,4,5";
	StringViewer viewer(source);
	while (viewer.hasNext())
	{
		console::print(viewer.next(',').toInt(0) + 1);
	}
}