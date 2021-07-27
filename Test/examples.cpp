
#include "spadas.h"

using namespace spadas::console;

void exampleEmptyObject();
void exampleString();
void exampleCreate();
void exampleBaseObject();
void exampleForElement();
void exampleArrayX();
void examplePointTo();
void exampleEqual();
void exampleMap();
void exampleTrim();
void exampleFile();
void exampleTask();

void runExamples()
{
    print("-- Example: Empty Object --");
    exampleEmptyObject();

    print("\n-- Example: String --");
    exampleString();

    print("\n-- Example: Create --");
    exampleCreate();

    print("\n-- Example: Base Object --");
    exampleBaseObject();

    print("\n-- Example: For Element --");
    exampleForElement();

    print("\n-- Example: ArrayX --");
    exampleArrayX();

    print("\n-- Example: Point To --");
    examplePointTo();

    print("\n-- Example: Equal --");
    exampleEqual();

    print("\n-- Example: Map --");
    exampleMap();

    print("\n-- Example: Trim --");
    exampleTrim();

    print("\n-- Example: File --");
    exampleFile();

    print("\n-- Example: Task --");
    exampleTask();
}
