
#include "spadas.h"

using namespace spadas;

// OptionalBool
OptionalBool::OptionalBool(GeneralElement elem) : valid(elem.valid && !elem.isText), value(elem.value != 0)
{}

// OptionalInt
OptionalInt::OptionalInt(GeneralElement elem) : valid(elem.valid && !elem.isText), value((Int)elem.value)
{}

// OptionalDouble
OptionalDouble::OptionalDouble(GeneralElement elem) : valid(elem.valid && !elem.isText), value(elem.value)
{}

// OptionalSignedDouble
OptionalDouble OptionalSignedDouble::toOptionalDouble(Bool signOptional)
{
	if (signOptional)
	{
		if (valueValid)
		{
			if (signValid)
			{
				if (positive) return value;
				else return -value;
			}
			return value;
		}
		else return OptionalDouble();
	}
	else
	{
		if (signValid && valueValid)
		{
			if (positive) return value;
			else return -value;
		}
		else return OptionalDouble();
	}
}

// ShortTimestamp
String ShortTimestamp::toString()
{
	Array<String> comps(2);
	comps[0] = session.toString();
	comps[1] = String(offset, 6);
	return String::merge(comps, "-");
}

// FullTimestamp
String FullTimestamp::toString()
{
	Array<String> comps(2);
	comps[0] = session.toString();
	comps[1] = String(offset, 6);
	return String::merge(comps, "-");
}

// IStandaloneTaskCallback
void IStandaloneTaskCallback::setTaskProgress(Enum<StandaloneTaskState> state, String description, Double progress)
{
}

void IStandaloneTaskCallback::setTaskReturnValue(String value)
{
}

// ICrossTransmitter
void ICrossTransmitter::sendToApp(String id, Binary data)
{
}

Bool ICrossTransmitter::sendToNative(String pluginType, String id, Binary data)
{
	return FALSE;
}

// ICrossCaller
Bool ICrossCaller::callAppFunction(String id, Binary input, Binary& output)
{
	return FALSE;
}

Bool ICrossCaller::callNativeFunction(String pluginType, String id, BaseObject context)
{
	return FALSE;
}

// SessionIdentifier
SessionIdentifier::SessionIdentifier() : value(0)
{}

SessionIdentifier::SessionIdentifier(Word year, Byte month, Byte day, Byte hour, Byte minute, Byte second) : value(0)
{
	if (year >= 1900 && year < 10000 && month != 0 && month <= 12 && day != 0 && day <= 31 && hour < 24 && minute < 60 && second < 60)
	{
		value = ((ULong)year << 48) | ((ULong)month << 40) | ((ULong)day << 32) | ((ULong)hour << 16) | ((ULong)minute << 8) | (ULong)second;
	}
}

SessionIdentifier::SessionIdentifier(Time time) : value(0)
{
	if (time.year >= 1900 && time.year < 10000 && time.month != 0 && time.month <= 12 && time.day != 0 && time.day <= 31 && time.hour < 24 && time.minute < 60 && time.second < 60)
	{
		value = ((ULong)time.year << 48) | ((ULong)time.month << 40) | ((ULong)time.day << 32) | ((ULong)time.hour << 16) | ((ULong)time.minute << 8) | (ULong)time.second;
	}
}

SessionIdentifier::SessionIdentifier(String idString) : value(0)
{
	Array<StringSpan> comps = idString.split("-");
	if (comps.size() != 6) return;

	Int year0 = -1, month0 = -1, day0 = -1, hour0 = -1, minute0 = -1, second0 = -1;
	comps[0].toNumber(year0);
	comps[1].toNumber(month0);
	comps[2].toNumber(day0);
	comps[3].toNumber(hour0);
	comps[4].toNumber(minute0);
	comps[5].toNumber(second0);
	if (year0 >= 1900 && year0 < 10000 && month0 >= 1 && month0 <= 12 && day0 >= 1 && day0 <= 31 &&
		hour0 >= 0 && hour0 < 24 && minute0 >= 0 && minute0 < 60 && second0 >= 0 && second0 < 60)
	{
		value = ((ULong)year0 << 48) | ((ULong)month0 << 40) | ((ULong)day0 << 32) | ((ULong)hour0 << 16) | ((ULong)minute0 << 8) | (ULong)second0;
	}
}

Bool SessionIdentifier::isValid()
{
	return value != 0;
}

Bool SessionIdentifier::operator ==(SessionIdentifier id)
{
	return this->value == id.value;
}

Bool SessionIdentifier::operator !=(SessionIdentifier id)
{
	return this->value != id.value;
}

Bool SessionIdentifier::operator >(SessionIdentifier id)
{
	return this->value > id.value;
}

Bool SessionIdentifier::operator <(SessionIdentifier id)
{
	return this->value < id.value;
}

Word SessionIdentifier::getHash()
{
	return (Word)value;
}

String SessionIdentifier::toString()
{
	if (value == 0) return "1900-01-01-00-00-00";

	Time time = toTime();

	Binary buffer(19, (Byte)'0');
	buffer[4] = buffer[7] = buffer[10] = buffer[13] = buffer[16] = (Byte)'-';
	Byte *data = buffer.data();
	data[0] = (Byte)'0' + (Byte)(time.year / 1000);
	data[1] = (Byte)'0' + (Byte)((time.year / 100) % 10);
	data[2] = (Byte)'0' + (Byte)((time.year / 10) % 10);
	data[3] = (Byte)'0' + (Byte)(time.year % 10);
	data[5] = (Byte)'0' + (Byte)(time.month / 10);
	data[6] = (Byte)'0' + (Byte)(time.month % 10);
	data[8] = (Byte)'0' + (Byte)(time.day / 10);
	data[9] = (Byte)'0' + (Byte)(time.day % 10);
	data[11] = (Byte)'0' + (Byte)(time.hour / 10);
	data[12] = (Byte)'0' + (Byte)(time.hour % 10);
	data[14] = (Byte)'0' + (Byte)(time.minute / 10);
	data[15] = (Byte)'0' + (Byte)(time.minute % 10);
	data[17] = (Byte)'0' + (Byte)(time.second / 10);
	data[18] = (Byte)'0' + (Byte)(time.second % 10);
	return buffer;
}

Time SessionIdentifier::toTime()
{
	if (value == 0) return Time();

	UInt year = (UInt)((value & 0xffff000000000000) >> 48);
	UInt month = (UInt)((value & 0x0000ff0000000000) >> 40);
	UInt day = (UInt)((value & 0x000000ff00000000) >> 32);
	UInt hour = (UInt)((value & 0x0000000000ff0000) >> 16);
	UInt minute = (UInt)((value & 0x000000000000ff00) >> 8);
	UInt second = (UInt)(value & 0x00000000000000ff);
	return Time(year, month, day, hour, minute, second);
}

// IVideoPreviewExpress
void IVideoPreviewExpress::outputPreview(ULong cpuTick, UInt channel, ImagePointer preview, NanoPosix guestPosix, NanoPosix gnssPosix)
{
}

// IGeneralDeviceDataOutput
void IGeneralDeviceDataOutput::outputGeneralDeviceData(GeneralDeviceData data)
{
}

// IGeneralDataTransmitter
TransmitResult::Value IGeneralDataTransmitter::transmitNow(String protocol, Array<Double> vector, Binary binary)
{
	return TransmitResult::Value::Unknown;
}

TransmitResult::Value IGeneralDataTransmitter::transmitAtServerPosix(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance, String guestSyncID)
{
	return TransmitResult::Value::Unknown;
}

// IBusMessageTransmitter
TransmitResult::Value IBusMessageTransmitter::transmitNow(UInt channel, UInt id, Binary binary)
{
	return TransmitResult::Value::Unknown;
}

TransmitResult::Value IBusMessageTransmitter::transmitRepeatedly(UInt channel, UInt id, Binary binary, UInt interval)
{
	return TransmitResult::Value::Unknown;
}

TransmitResult::Value IBusMessageTransmitter::transmitAtServerPosix(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance)
{
	return TransmitResult::Value::Unknown;
}

// IVideoFrameTransmitter
TransmitResult::Value IVideoFrameTransmitter::transmitNow(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data)
{
	return TransmitResult::Value::Unknown;
}

TransmitResult::Value IVideoFrameTransmitter::transmitAtServerPosix(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance)
{
	return TransmitResult::Value::Unknown;
}