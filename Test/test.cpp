
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void runExamples();

int main(int argc, char* argv[])
{
	// runExamples();

	XML xml = XML::createFromFile(SS"/home/ken/Desktop/test.xml").value();
	console::print(xml.globalRoot()->attributes[0].value);

	console::print("over");
	console::waitKey();
	return 0;
}