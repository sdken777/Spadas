
#include "spadas.h"

using namespace spadas;

OptionalBool::OptionalBool(GeneralElement elem) : valid(elem.valid && !elem.isText), value(elem.value != 0)
{}

OptionalBool& OptionalBool::operator =(Bool val)
{
	valid = TRUE;
	value = val;
	return *this;
}

Bool OptionalBool::isValidAndTrue()
{
	return valid && value;
}

Bool OptionalBool::isValidAndFalse()
{
	return valid && !value;
}

Bool OptionalBool::operator ==(OptionalBool val)
{
	if (!valid && !val.valid) return TRUE;
	else if (!valid || !val.valid) return FALSE;
	return value == val.value;
}

Bool OptionalBool::operator !=(OptionalBool val)
{
	return !(operator ==(val));
}

String OptionalBool::toString()
{
	return valid ? String(value) : "(invalid)";
}

OptionalInt::OptionalInt(GeneralElement elem) : valid(elem.valid && !elem.isText), value((Int)elem.value)
{}

OptionalInt& OptionalInt::operator =(Int val)
{
	valid = TRUE;
	value = val;
	return *this;
}

Bool OptionalInt::operator ==(OptionalInt val)
{
	if (!valid && !val.valid) return TRUE;
	else if (!valid || !val.valid) return FALSE;
	return value == val.value;
}

Bool OptionalInt::operator !=(OptionalInt val)
{
	return !(operator ==(val));
}

String OptionalInt::toString()
{
	return valid ? String(value) : "(invalid)";
}

OptionalDouble::OptionalDouble(GeneralElement elem) : valid(elem.valid && !elem.isText), value(elem.value)
{}

OptionalDouble& OptionalDouble::operator =(Double val)
{
	valid = TRUE;
	value = val;
	return *this;
}

OptionalDouble OptionalDouble::operator +(OptionalDouble val)
{
	return (valid && val.valid) ? (value + val.value) : OptionalDouble();
}

OptionalDouble OptionalDouble::operator -(OptionalDouble val)
{
	return (valid && val.valid) ? (value - val.value) : OptionalDouble();
}

OptionalDouble OptionalDouble::operator *(OptionalDouble val)
{
	return (valid && val.valid) ? (value * val.value) : OptionalDouble();
}

Bool OptionalDouble::operator ==(OptionalDouble val)
{
	if (!valid && !val.valid) return TRUE;
	else if (!valid || !val.valid) return FALSE;
	return value == val.value;
}

Bool OptionalDouble::operator !=(OptionalDouble val)
{
	return !(operator ==(val));
}

String OptionalDouble::toString()
{
	return valid ? String(value) : "(invalid)";
}

void OptionalSignedDouble::setValue(Double value)
{
	this->value = value;
	this->valueValid = TRUE;
}

void OptionalSignedDouble::setSign(Bool positive)
{
	this->positive = positive;
	this->signValid = TRUE;
}

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

Bool GlobalTimestamp::operator ==(GlobalTimestamp time)
{
	return base == time.base && offset == time.offset;
}

Bool GlobalTimestamp::operator !=(GlobalTimestamp time)
{
	return base != time.base || offset != time.offset;
}

String GlobalTimestamp::toString()
{
	return base.dateString(String()) + "-" + base.timeString("-") + "-" + String(offset, 6);
}

String ShortTimestamp::toString()
{
	Array<String> comps(7);
	comps[0] = session.year;
	comps[1] = session.month;
	comps[2] = session.day;
	comps[3] = session.hour;
	comps[4] = session.minute;
	comps[5] = session.second;
	comps[6] = String(offset, 6);
	return String::mergeStrings(comps, "-");
}

String FullTimestamp::toString()
{
	Array<String> comps(7);
	comps[0] = session.year;
	comps[1] = session.month;
	comps[2] = session.day;
	comps[3] = session.hour;
	comps[4] = session.minute;
	comps[5] = session.second;
	comps[6] = String(offset, 6);
	return String::mergeStrings(comps, "-");
}

ShortTimestamp FullTimestamp::toShort()
{
	return ShortTimestamp(session, offset);
}

String GeneralElement::toString()
{
	return valid ? (isText ? text : String(value)) : "(invalid)";
}

void IBusRawDataTransmitter::transmitMessage(BusRawData msg, UInt interval)
{
}

void IStandaloneTaskCallback::setTaskProgress(StandaloneTaskState state, String description, Double progress)
{
}

void IStandaloneTaskCallback::setTaskReturnValue(String value)
{
}

void IGeneralRawDataOutput::outputGeneralRawData(String protocol, GeneralRawData data)
{
}

void IVideoPreviewExpress::outputPreview(Double ts, UInt ch, ImagePointer preview)
{
}

void ICrossTransmitter::sendToApp(String id, Binary data)
{
}

Bool ICrossTransmitter::sendToNative(String pluginType, String id, Binary data)
{
	return FALSE;
}

Bool ICrossCaller::callAppFunction(String id, Binary input, Binary& output)
{
	return FALSE;
}

Bool ICrossCaller::callNativeFunction(String pluginType, String id, BaseObject context)
{
	return FALSE;
}

SessionIdentifier::SessionIdentifier(String idString) : year(0), month(0), day(0), hour(0), minute(0), second(0)
{
	Array<String> comps = idString.split("-");
	if (comps.size() != 6) return;

	auto year0 = comps[0].toInt().value(-1);
	auto month0 = comps[1].toInt().value(-1);
	auto day0 = comps[2].toInt().value(-1);
	auto hour0 = comps[3].toInt().value(-1);
	auto minute0 = comps[4].toInt().value(-1);
	auto second0 = comps[5].toInt().value(-1);
	if (year0 > 0 && year0 < 65536 && month0 >= 1 && month0 <= 12 && day0 >= 1 && day0 <= 31 &&
		hour0 >= 0 && hour0 < 24 && minute0 >= 0 && minute0 < 60 && second0 >= 0 && second0 < 60)
	{
		year = (Word)year0;
		month = (Byte)month0;
		day = (Byte)day0;
		hour = (Byte)hour0;
		minute = (Byte)minute0;
		second = (Byte)second0;
	}
}

Bool SessionIdentifier::operator ==(SessionIdentifier id)
{
	return this->year == id.year &&
		this->month == id.month &&
		this->day == id.day &&
		this->hour == id.hour &&
		this->minute == id.minute &&
		this->second == id.second;
}

Bool SessionIdentifier::operator !=(SessionIdentifier id)
{
	return !operator ==(id);
}

Bool SessionIdentifier::operator >(SessionIdentifier time)
{
	ULong t1 = ((ULong)this->year << 40) + ((ULong)this->month << 32) + ((ULong)this->day << 24) + 
		((ULong)this->hour << 16) + ((ULong)this->minute << 8) + (ULong)this->second;
	ULong t2 = ((ULong)time.year << 40) + ((ULong)time.month << 32) + ((ULong)time.day << 24) + 
		((ULong)time.hour << 16) + ((ULong)time.minute << 8) + (ULong)time.second;
	return t1 > t2;
}

Bool SessionIdentifier::operator <(SessionIdentifier time)
{
	ULong t1 = ((ULong)this->year << 40) + ((ULong)this->month << 32) + ((ULong)this->day << 24) + 
		((ULong)this->hour << 16) + ((ULong)this->minute << 8) + (ULong)this->second;
	ULong t2 = ((ULong)time.year << 40) + ((ULong)time.month << 32) + ((ULong)time.day << 24) + 
		((ULong)time.hour << 16) + ((ULong)time.minute << 8) + (ULong)time.second;
	return t1 < t2;
}

Word SessionIdentifier::getHash()
{
	return (((Byte)year << 8) | ((Word)month << 5) | ((Word)day)) ^ (((Word)hour << 11) | ((Word)minute << 6) | (Word)second);
}

String SessionIdentifier::toString()
{
	Array<String> comps(6);
	comps[0] = year;
	comps[1] = month;
	comps[2] = day;
	comps[3] = hour;
	comps[4] = minute;
	comps[5] = second;
	return String::mergeStrings(comps, "-");
}

Time SessionIdentifier::toTime()
{
	return Time(year, month, day, hour, minute, second);
}

void IGeneralDataTransmitter::transmitNow(String protocol, Array<Double> vector, Binary binary)
{
}

void IGeneralDataTransmitter::transmitAtTimeOffset(String protocol, Array<Double> vector, Binary binary, Double offset, UInt tolerance)
{
}

void IGeneralDataTransmitter::transmitAtServerPosix(String protocol, Array<Double> vector, Binary binary, ULong serverPosixMS, UInt serverPosixNS, UInt tolerance)
{
}

void IBusMessageTransmitter::transmitNow(UInt channel, UInt id, Binary binary)
{
}

void IBusMessageTransmitter::transmitRepeatedly(UInt channel, UInt id, Binary binary, UInt interval)
{
}

void IBusMessageTransmitter::transmitAtTimeOffset(UInt channel, UInt id, Binary binary, Double offset, UInt tolerance)
{
}

void IBusMessageTransmitter::transmitAtServerPosix(UInt channel, UInt id, Binary binary, ULong serverPosixMS, UInt serverPosixNS, UInt tolerance)
{
}

void IVideoPreviewExpressX::outputPreview(ULong cpuTick, UInt channel, ImagePointer preview)
{
}

void IVideoFrameTransmitter::transmitNow(UInt channel, VideoDataCodec codec, Size2D size, Binary data)
{
}

void IVideoFrameTransmitter::transmitAtTimeOffset(UInt channel, VideoDataCodec codec, Size2D size, Binary data, Double offset, UInt tolerance)
{
}

void IVideoFrameTransmitter::transmitAtServerPosix(UInt channel, VideoDataCodec codec, Size2D size, Binary data, ULong serverPosixMS, UInt serverPosixNS, UInt tolerance)
{
}

void IGeneralDeviceDataOutput::outputGeneralDeviceData(GeneralDeviceData data)
{
}