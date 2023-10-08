
#include "string_spadas.h"

using namespace spadas;

StringAppender::StringAppender(String origin) : str(origin)
{}

String StringAppender::toString()
{
	return str;
}

StringAppender& spadas::operator +(StringAppender appender, String str)
{
	return appender.operator +(str);
}