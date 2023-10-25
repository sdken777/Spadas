
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleXML()
{
	// Efficiency test of XML content creation / XML创建内容效率测试
	Timer timer;
	XML xml;
	UInt counter = 0;
	Array<String> attribNames(100); // One hundred attributes per node / 每个节点一百个属性
	for (UInt n = 0; n < attribNames.size(); n++)
	{
		attribNames[n] = "number" cat (n + 1);
	}
	for (UInt i = 0; i < 10000; i++) // Ten thousand nodes / 一万个节点
	{
		XMLElement elem;
		elem.tag = "numbers";
		elem.attributes = Array<XMLAttribute>(attribNames.size());
		for (UInt n = 0; n < attribNames.size(); n++)
		{
			elem.attributes[n].name = attribNames[n];
			elem.attributes[n].value = counter++;
		}
		xml.globalRoot().addLeaf(elem);
	}
	console::print("Create contents: " cat timer.check());

	// Efficiency test of XML serialization / XML序列化效率测试
	timer.start();
	Binary xmlBinary = xml.toBinary();
	console::print("To binary: " cat timer.check());

	// Efficiency test of XML deserialization / XML反序列化效率测试
	timer.start();
	auto parsedXML = XML::createFromBinary(xmlBinary).value(XML());
	console::print("From binary: " cat timer.check());

	// Validate result / 验证结果
	UInt attribCount = 0;
	XMLNode rootNode = parsedXML.globalRoot();
	for (auto e = rootNode.leaves().firstElem(); e.valid(); ++e)
	{
		attribCount += e->value().attributes.size();
	}
	console::print("Attribute count: " cat attribCount);
	console::print("Binary size: " cat xmlBinary.size() cat "=" cat parsedXML.toBinary().size());

	// Efficiency test of XML releasing / XML析构效率测试
	timer.start();
	parsedXML = XML();
	rootNode = XMLNode();
	console::print("Release: " cat timer.check());
}