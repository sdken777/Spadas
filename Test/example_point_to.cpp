
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void examplePointTo()
{
	// Optional::operator ->
	console::print("- 1 -");

	Optional<String> string = SS"ABC";
	console::print(string->toLower());

	// ArrayElem::operator -> (ListElem类似)
	console::print("- 2 -");

	Array<String> arr = Array<String>::create(2, "DEF", "XYZ");
	for (auto e = arr.firstElem(); e.valid(); ++e)
	{
		console::print(e->toLower());
	}

	// TreeNode::operator -> (ListNode类似)
	console::print("- 3 -");

	XML xml;
	xml.globalRoot()->tag = "test_xml";
	console::print(xml.toBinary());
}