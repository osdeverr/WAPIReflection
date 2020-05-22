#include "SystemManager.h"

int main()
{
	SystemManager mgr;

	mgr.AddSystemsFromAssembly(WAPIReflection::Assembly::local());
	mgr.OnInitFinished();

	mgr.OnTextMessage("Messaging!");
}