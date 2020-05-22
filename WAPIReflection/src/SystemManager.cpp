#include "SystemManager.h"

void SystemManager::OnTextMessage(const std::string& msg)
{
	InvokeOnAll("OnTextMessage", msg.c_str());
}

void SystemManager::OnInitFinished()
{
	InvokeOnAll("OnInitFinished");
}
