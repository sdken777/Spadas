#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

class NoVarsObject : public EmptyObject, public IPluginLatest
{
public:
	String getPluginType() { return "MyPlugin"; }
	String getPluginVersion() { return "1.0.0"; }
};

void exampleEmptyObject()
{
	Interface<IPluginLatest> plugin = NoVarsObject();
	console::print(catAll(plugin->getPluginType(), " ", plugin->getPluginVersion()));
}