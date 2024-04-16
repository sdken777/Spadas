
#include "spadas.h"

using namespace spadas;

Bool ITimeServer::createTimestamp(FullTimestamp& outputTimestamp, SessionIdentifier session, ULong cpuTick, NanoPosix hostPosix, NanoPosix guestPosix, NanoPosix gnssPosix, String protocol)
{
    return FALSE;
}

FullTimestamp ITimeServer::resyncTimestamp(FullTimestamp srcTimestamp, NanoPosix guestPosix, NanoPosix gnssPosix, String protocol)
{
    return srcTimestamp;
}

NanoPosix ITimeServer::calcPosixTime(ShortTimestamp srcTimestamp)
{
    return 0;
}

ULong ITimeServer::calcCPUTick(SessionIdentifier session, Double timeOffset)
{
    return 0;
}