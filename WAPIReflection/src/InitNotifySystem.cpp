#include "InitNotifySystem.h"
#include <cstdio>

InitNotifySystem::InitNotifySystem()
{
	std::printf("Consturcted InitNotifySys\n");
}

void InitNotifySystem::OnInitFinished()
{
	std::printf(" == All systems initialized! ==\n\n");
}
