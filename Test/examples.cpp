
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas::console;

void exampleEmptyObject();
void exampleString();
void exampleCreate();
void exampleBaseObject();
void exampleForElement();
void exampleLambda();
void exampleArrayX();
void examplePointTo();
void exampleEqual();
void exampleMap();
void exampleTrim();
void exampleConsole();
void exampleFile();
void exampleTask();
void exampleMatrix();

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

    print("\n-- Example: Lambda --");
    exampleLambda();

    print("\n-- Example: ArrayX --");
    exampleArrayX(); // GDB调试时析构可能耗时特别长

    print("\n-- Example: Point To --");
    examplePointTo();

    print("\n-- Example: Equal --");
    exampleEqual();

    print("\n-- Example: Map --");
    exampleMap();

    print("\n-- Example: Trim --");
    exampleTrim();

    print("\n-- Example: Console --");
    exampleConsole(); // 需要控制台输入

    print("\n-- Example: File --");
    exampleFile();

    print("\n-- Example: Task --");
    exampleTask();

    print("\n-- Example: Matrix --");
    exampleMatrix();
}
