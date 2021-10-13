
#include "spadas.h"

using namespace spadas;

VideoRawDataTable::VideoRawDataTable() : channelDatas(24)
{}

void VideoRawDataTable::clear()
{
	base = Time();
	for (UInt i = 0; i < channelDatas.size(); i++) channelDatas[i] = Array<VideoRawData>();
}

Bool VideoRawDataTable::isEmpty()
{
	for (UInt i = 0; i < channelDatas.size(); i++)
	{
		if (!channelDatas[i].isEmpty()) return FALSE;
	}
	return TRUE;
}

VideoProcDataTable::VideoProcDataTable() : channelDatas(24)
{}

void VideoProcDataTable::clear()
{
	for (UInt i = 0; i < channelDatas.size(); i++) channelDatas[i] = Array<VideoProcData>();
}