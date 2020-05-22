#include "SystemManager.h"
#include <iostream>
#include <wapirefl/Object.h>
#include <chrono>

class Plak : public ISystem
{
public:
	void OnTextMessage(const char* mesg);
};

void Plak::OnTextMessage(const char* mesg)
{
	std::printf("[Plak] Message received: %s\n", mesg);
}

class FakMP
{
public:
	FakMP(int numFaks, std::string numPlaks, float doKy);
};
FakMP::FakMP(int numFaks, std::string numPlaks, float doKy)
{
	// yes
}

int main()
{
	try {
		using namespace std::literals;
		using namespace WAPIReflection;

		typeof<FakMP>().asClass().construct({ 50, "elaymm4"s, 5.f });

		SystemManager mgr;
		mgr.AddFromAssembly(Assembly::local());
		mgr.OnInitFinished();

		std::string msg;
		while (std::getline(std::cin, msg))
			mgr.OnTextMessage(msg);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		throw;
	}
}