
#include "spadas.h"

using namespace spadas;

ULong BusMessageInfo::indicator()
{
	return ((ULong)channel << 32) | (ULong)localID;
}

BusMessageID::BusMessageID() : message(0)
{

}
BusMessageID::BusMessageID(BusProtocolID protocol, UInt message) : protocol(protocol), message(message)
{
	text = String::createWithSize(128);
	text += protocol;
	text += ":";
	text += message;
}
BusMessageID::BusMessageID(String text) : message(0), text(text)
{
	if (text.isEmpty()) return;

	Array<StringSpan> comps = text.split(":");
	if (comps.size() != 2) return;

	protocol = comps[0];

	Int id = 0;
	if (comps[1].toNumber(id)) message = id;
}

BusSignalID::BusSignalID() : message(0)
{

}
BusSignalID::BusSignalID(BusProtocolID protocol, UInt message, String signal) : protocol(protocol), message(message), signal(signal)
{
	text = String::createWithSize(128);
	text += protocol;
	text += ":";
	text += message;
	text += ":";
	text += signal;
}
BusSignalID::BusSignalID(String text) : message(0), text(text)
{
	if (text.isEmpty()) return;

	Array<StringSpan> comps = text.split(":");
	if (comps.size() != 3) return;

	protocol = comps[0];
	signal = comps[2];

	Int id = 0;
	if (comps[1].toNumber(id)) message = id;
}

BusSignedSignalConfig::BusSignedSignalConfig() : positiveAsZero(FALSE)
{

}

Bool BusSignedSignalConfig::isSignOptional()
{
	return signSignalID.isEmpty() || signSignalID == "null";
}