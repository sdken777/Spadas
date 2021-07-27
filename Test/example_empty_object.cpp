
#include "spadas.h"

using namespace spadas;

class NoVarsObject : public EmptyObject, public IPlugin
{
public:
	String getPluginType() { return "MyPlugin"; }
	String getPluginVersion() { return "1.0.0"; }
};

void exampleEmptyObject()
{
	Interface<IPlugin> plugin = NoVarsObject();
	console::print(plugin->getPluginType() + " " + plugin->getPluginVersion());
}