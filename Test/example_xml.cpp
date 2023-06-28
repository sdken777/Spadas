
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleXML()
{
	// XML创建内容效率测试
	Timer timer;
	XML xml;
	UInt counter = 0;
	Array<String> attribNames(100);
	for (UInt n = 0; n < attribNames.size(); n++)
	{
		attribNames[n] = SS"number" + (n + 1);
	}
	for (UInt i = 0; i < 10000/* 一万个节点 */; i++)
	{
		XMLElement elem;
		elem.tag = "numbers";
		elem.attributes = Array<XMLAttribute>(100);
		for (UInt n = 0; n < 100/* 一百个属性 */; n++)
		{
			elem.attributes[n].name = attribNames[n];
			elem.attributes[n].value = counter++;
		}
		xml.globalRoot().addLeaf(elem);
	}
	console::print(SS"Create contents: " + timer.check());

	// XML序列化效率测试
	timer.start();
	Binary xmlBinary = xml.toBinary();
	console::print(SS"To binary: " + timer.check());

	// XML反序列化效率测试
	timer.start();
	XML::createFromBinary(xmlBinary);
	console::print(SS"From binary: " + timer.check());
}