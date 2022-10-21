
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

class NoVarsObject : public EmptyObject, public IPluginV102
{
public:
	String getPluginType() { return "MyPlugin"; }
	String getPluginVersion() { return "1.0.0"; }
};

void exampleEmptyObject()
{
	Interface<IPluginV102> plugin = NoVarsObject();
	console::print(plugin->getPluginType() + " " + plugin->getPluginVersion());
}