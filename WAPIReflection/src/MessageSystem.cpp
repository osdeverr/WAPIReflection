#include "MessageSystem.h"
#include <cstdio>

void MessageSystem::OnTextMessage(const char* message)
{
	std::printf("[MessageSystem] Incoming message: %s\n", message);
}
