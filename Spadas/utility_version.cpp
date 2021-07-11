
#include "spadas.h"

using namespace spadas;

String spadas::system::getSpadasVersion()
{
	return (String)SPADAS_VERSION_MAJOR + "." + SPADAS_VERSION_MINOR + "." + SPADAS_VERSION_BUILD;
}