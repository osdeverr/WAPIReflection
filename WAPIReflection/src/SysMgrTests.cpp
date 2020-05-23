#include "SysMgrTests.h"
#include "SystemManager.h"

void SysMgrTests::CallbackTest()
{
	SystemManager sysMgr;
	sysMgr.AddFromAssembly(WAPIReflection::Assembly::local());
	sysMgr.OnInitFinished();

	sysMgr.OnTextMessage("Message 1");
	sysMgr.OnTextMessage("Message 2");
}
