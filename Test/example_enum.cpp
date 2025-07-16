#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

void exampleEnum()
{
    // Create many enumeration objects / 创建多个枚举对象
    Enum<MessageLevel> enum1; // The "Default" value is default / 默认按Default赋值
    Enum<MessageLevel> enum2 = (MessageLevel::Value)999; // Force creation based on an invalid value / 强制基于无效值创建
    Enum<MessageLevel> enum3 = MessageLevel::Value::Warning;

    // Print the objects / 打印对象
    see(enum1, enum2, enum3);
}