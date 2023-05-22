
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

typedef IPluginV105 IPluginLatest;

class NoVarsObject : public EmptyObject, public IPluginLatest
{
public:
	String getPluginType() { return "MyPlugin"; }
	String getPluginVersion() { return "1.0.0"; }
};

void exampleEmptyObject()
{
	Interface<IPluginLatest> plugin = NoVarsObject();
	console::print(plugin->getPluginType() + " " + plugin->getPluginVersion());
}