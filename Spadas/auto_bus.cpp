
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

	Array<String> comps = text.split(":");
	if (comps.size() != 2) return;

	protocol = comps[0];
	Optional<Int> msgO = comps[1].toInt();
	if (msgO.isValid()) message = msgO.value();
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

	Array<String> comps = text.split(":");
	if (comps.size() != 3) return;

	protocol = comps[0];
	Optional<Int> msgO = comps[1].toInt();
	if (msgO.isValid()) message = msgO.value();
	signal = comps[2];
}

BusSignedSignalConfig::BusSignedSignalConfig() : positiveAsZero(FALSE)
{

}

Bool BusSignedSignalConfig::isSignOptional()
{
	return signSignalID.isEmpty() || signSignalID == "null";
}

void BusRawDataTable::clear()
{
	for (UInt i = 0; i < this->channelDatas.size(); i++) this->channelDatas[i] = Array<BusRawData>();
}

//////////////////////////////////////////////////////////////////////////////////////////

namespace spadas
{
	class BusDataSorterVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(BusDataSorter, Vars)

		Optional<Time> currentBase;
		ArrayX<Array<Signal> > signalBuffer;
		ArrayX<Array<BusMessage> > messageBuffer;
		Array<Array<Signal> > signals;
		Array<Array<BusMessage> > messages;
	};
}

const String spadas::BusDataSorter::TypeName = "spadas.BusDataSorter";

BusDataSorter::BusDataSorter() : Object<BusDataSorterVars>(new BusDataSorterVars, TRUE)
{
	reset();
}

Int BusDataSorter::addSignals(Array<Signal> signals)
{
	if (signals.isEmpty()) return -1;
	Int id = vars->signalBuffer.size();
	vars->signalBuffer.append(signals);
	return id;
}

Int BusDataSorter::addBusMessages(Array<BusMessage> messages)
{
	if (messages.isEmpty()) return -1;
	Int id = vars->messageBuffer.size();
	vars->messageBuffer.append(messages);
	return id;
}

Bool BusDataSorter::process(Array<SortedObject>& sorted)
{
	sorted = Array<SortedObject>();

	vars->signals = vars->signalBuffer.toArray();
	vars->messages = vars->messageBuffer.toArray();

	vars->signalBuffer = ArrayX<Array<Signal> >();
	vars->messageBuffer = ArrayX<Array<BusMessage> >();

	const UInt signalTypes = vars->signals.size();
	const UInt messageTypes = vars->messages.size();

	Bool timeInit = FALSE;
	Time time;
	Array<UInt> signalProc(signalTypes, 0);
	Array<UInt> messageProc(messageTypes, 0);
	ArrayX<SortedObject> buf;

	while (TRUE)
	{
		SortedObject target;
		target.id = target.index = -1;
		Double targetTime = DINF;

		// 遍历各类信号中首个信号，更新targetTime
		for (UInt i = 0; i < signalTypes; i++)
		{
			Array<Signal>& signals = vars->signals[i];
			UInt n = signalProc[i];
			if (n >= signals.size()) continue;

			GlobalTimestamp& gt = signals[n].timestamp;
			if (timeInit)
			{
				if (gt.base != time) return FALSE;
			}
			else
			{
				time = gt.base;
				timeInit = TRUE;
			}
			if (gt.offset < targetTime)
			{
				target.signal = TRUE;
				target.id = i;
				target.index = n;
				targetTime = gt.offset;
			}
		}

		// 遍历各类CAN报文中首个报文，更新targetTime
		for (UInt i = 0; i < messageTypes; i++)
		{
			Array<BusMessage>& messages = vars->messages[i];
			UInt n = messageProc[i];
			if (n >= messages.size()) continue;

			GlobalTimestamp& gt = messages[n].timestamp;
			if (timeInit)
			{
				if (gt.base != time) return FALSE;
			}
			else
			{
				time = gt.base;
				timeInit = TRUE;
			}
			if (gt.offset < targetTime)
			{
				target.signal = FALSE;
				target.id = i;
				target.index = n;
				targetTime = gt.offset;
			}
		}

		if (targetTime == DINF) break; // targetTime为DINF说明已遍历完毕
		else
		{
			buf.append(target);
			if (target.signal) signalProc[target.id] = target.index + 1;
			else messageProc[target.id] = target.index + 1;
		}
	}

	// 判断base是否发生切换
	if (timeInit)
	{
		if (vars->currentBase.isValid())
		{
			if (vars->currentBase.value() != time)
			{
				vars->currentBase = time;
				return FALSE;
			}
		}
		else
		{
			vars->currentBase = time;
		}
	}
	else return TRUE; // 未add任何报文或信号

	// 输出
	sorted = buf.toArray();
	return TRUE;
}

Bool BusDataSorter::getSignal(SortedObject obj, Signal& signal)
{
	if (!obj.signal) return FALSE;
	if (obj.id >= (Int)vars->signals.size()) return FALSE;
	Array<Signal>& arr = vars->signals[obj.id];
	if (obj.index >= (Int)arr.size()) return FALSE;
	signal = arr[obj.index];
	return TRUE;
}

Bool BusDataSorter::getMessage(SortedObject obj, BusMessage& message)
{
	if (obj.signal) return FALSE;
	if (obj.id >= (Int)vars->messages.size()) return FALSE;
	Array<BusMessage>& arr = vars->messages[obj.id];
	if (obj.index >= (Int)arr.size()) return FALSE;
	message = arr[obj.index];
	return TRUE;
}

void BusDataSorter::reset()
{
	vars->currentBase = Optional<Time>();
	vars->signalBuffer = ArrayX<Array<Signal> >();
	vars->messageBuffer = ArrayX<Array<BusMessage> >();
	vars->signals = Array<Array<Signal> >();
	vars->messages = Array<Array<BusMessage> >();
}
