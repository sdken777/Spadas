
#include "spadas.h"

using namespace spadas;

void ITask::onRunTask(Flag shouldEnd)
{

}

String IWorkflow::getWorkflowName()
{
	return String();
}

Array<String> IWorkflow::getThreadNames()
{
	SPADAS_ERROR_MSG("Invalid getThreadNames()");
	return Array<String>();
}

UInt IWorkflow::getTimeInterval(UInt threadIndex)
{
    return 1;
}

Bool IWorkflow::supportUserTimeInterval(UInt threadIndex)
{
	return FALSE;
}

Bool IWorkflow::onThreadBegin(UInt threadIndex)
{
	return TRUE;
}

void IWorkflow::onThreadLoop(UInt threadIndex, Flag shouldEnd)
{
}

void IWorkflow::onThreadEnd(UInt threadIndex)
{
}