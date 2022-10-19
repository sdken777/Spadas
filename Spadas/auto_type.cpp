
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