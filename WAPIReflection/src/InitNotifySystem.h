#pragma once
#include "ISystem.h"

class InitNotifySystem : public ISystem
{
public:
	InitNotifySystem();
	void OnInitFinished();
};