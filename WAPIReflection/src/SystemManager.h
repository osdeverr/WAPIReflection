#pragma once
#include "TaggedClassMgr.h"
#include "ISystem.h"

class SystemManager : public TaggedClassMgr<ISystem>
{
public:
	void OnTextMessage(const std::string& msg);
	void OnInitFinished();
};