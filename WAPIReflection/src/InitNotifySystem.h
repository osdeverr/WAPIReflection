#pragma once
#include "ISystem.h"

class InitNotifySystem : public ISystem
{
public:
	void OnInitFinished();
};