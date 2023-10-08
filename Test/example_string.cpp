
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

#include <stdio.h>
#include <wchar.h>

using namespace spadas;

void exampleString()
{
	// 数据统一使用UTF8存储
	console::print("- 1 -");

	String str = "Hello天气不错";
	console::print("console::print: " cat str);

	Array<Char> strChars = str.chars(); // UTF8字符
	console::print("chars字符: " cat String::merge(Array<Byte>((Byte*)strChars.data(), strChars.size())));

	Array<WChar> strWChars = str.wchars(); // Windows下为UTF16字符，Linux下为UTF32字符
	if (sizeof(WChar) == 4) console::print("wchars字符: " cat String::merge(Array<UInt>((UInt*)strWChars.data(), strWChars.size())));
	else if (sizeof(WChar) == 2) console::print("wchars字符: " cat String::merge(Array<Word>((Word*)strWChars.data(), strWChars.size())));

	// 部分结构体和所有类实现toString函数
	console::print("- 2 -");

	console::print(Point2D(3, 4).toString());
	console::print(str.toString());
	console::print(Timer().toString());

	// 通过merge函数合并成字符串
	console::print("- 3 -");

	Array<Double> numbers(2);
	numbers[0] = 3.4;
	numbers[1] = 5.6;
	console::print(String::merge(numbers));

	Array<Point2D> points(2);
	points[0] = Point2D(3, 4);
	points[1] = Point2D(5, 6);
	console::print(String::merge(points, " | "));

	// 用cat连接字符串
	console::print("- 4 -");

	console::print("Number: " cat 123);
	console::print(1 cat "+" cat 2 cat "=" cat 3);

	Point2D pt1(1, 2), pt2(3, 5);
	console::print(pt1 cat "+" cat pt2 cat "=" cat (pt1 + pt2));
}