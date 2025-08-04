
#include "spadas.h"

using namespace spadas;

BaseObject::BaseObject()
{}

BaseObject::BaseObject(Vars* newVars) : Object<Vars>(newVars, TRUE)
{}

EmptyObject::EmptyObject() : BaseObject(new Vars)
{}