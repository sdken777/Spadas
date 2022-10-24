
#include "spadas.h"

using namespace spadas;

Bool ITimeServer::createTimestamp(FullTimestamp& outputTimestamp, SessionIdentifier session, ULong cpuTick, ULong hostPosix, ULong guestPosix, ULong gnssPosix, String protocol)
{
    return FALSE;
}

FullTimestamp ITimeServer::resyncTimestamp(FullTimestamp srcTimestamp, ULong guestPosix, ULong gnssPosix, String protocol)
{
    return srcTimestamp;
}

ULong ITimeServer::calcTime(ShortTimestamp srcTimestamp, TimeType timeType)
{
    return 0;
}