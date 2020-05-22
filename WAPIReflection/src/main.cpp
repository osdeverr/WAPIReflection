#include "SystemManager.h"

class Plak : public ISystem
{
public:
	void OnTextMessage(const char* mesg);
};

void Plak::OnTextMessage(const char* mesg)
{
	std::printf("[Plak] Message received: %s\n", mesg);
}

int main()
{
	SystemManager mgr;

	mgr.AddFromAssembly(WAPIReflection::Assembly::local());
	//mgr.AddSystem<Plak>();

	mgr.OnInitFinished();

	mgr.OnTextMessage("Messaging!");
}