#pragma once
#include "ISystem.h"
#include <string_view>

class MessageSystem : public ISystem
{
public:
	void OnTextMessage(const char* message);
};