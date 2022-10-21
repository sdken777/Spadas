
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleTrim()
{
	// 使用trim提高Array密集生成时的效率（Binary类似）
	Array<UInt> buffer(100);
	buffer[0] = 555;
	buffer[1] = 666;
	buffer.trim(2);
	console::print(String::merge(buffer, ", "));
}