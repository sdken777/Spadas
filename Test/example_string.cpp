
#include "spadas.h"

#include <stdio.h>
#include <wchar.h>

using namespace spadas;

void exampleString()
{
	// 数据统一使用UTF8存储
	console::print("- 1 -");

	String str = "Hello天气不错";

	Byte *strBytes = str.bytes(); // 任何平台都为UTF8
	console::print(SS"bytes: " + String::merge(Array<Byte>(strBytes, str.byteSize()), ","));

	Array<Char> strChars = str.chars(); // Windows下为ANSI，Linux下为UTF8
	console::print(SS"chars: " + String::merge(Array<Byte>((Byte*)strChars.data(), strChars.size()), ","));

	Array<WChar> strWChars = str.wchars(); // Windows下为UTF16，Linux下为UTF32
	if (sizeof(WChar) == 4) console::print(SS"wchars: " + String::merge(Array<UInt>((UInt*)strWChars.data(), strWChars.size()), ","));
	else if (sizeof(WChar) == 2) console::print(SS"wchars: " + String::merge(Array<Word>((Word*)strWChars.data(), strWChars.size()), ","));
	
	console::print(SS"console::print: " + str); // Spadas打印无需考虑字符编码问题

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
	console::print(String::merge(numbers, ", "));

	Array<Point2D> points(2);
	points[0] = Point2D(3, 4);
	points[1] = Point2D(5, 6);
	console::print(String::merge(points, " | "));

	// 用+连接字符串时，通过SS初始化第一个字符串
	console::print("- 4 -");

	console::print(SS"Number: " + 123);
	console::print(SS 1 + "+" + 2 + "=" + 3);

	Point2D pt1(1, 2), pt2(3, 5);
	console::print(SS pt1 + "+" + pt2 + "=" + (pt1 + pt2));
}