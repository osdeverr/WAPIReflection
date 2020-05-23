#include <iostream>
#include <chrono>
#include <wapirefl/Object.h>

#include "SystemManager.h"
#include "Attributes.h"
#include "TestManager.h"

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

struct IPlakable
{
	virtual ~IPlakable() {}

	virtual void DoPlak() = 0;
};
class Plakable : public IPlakable
{
public:
	void DoPlak() {
		std::cout << "Plaking!!" << std::endl;
	}
};

class ElayAttribute : public attribs::Attribute
{
public:
	ElayAttribute(float durakLevel) : mDurakLvl(durakLevel)
	{
		if (durakLevel < 1.f)
			throw std::system_error(
				std::error_code(666, std::system_category()),
				"Elay must be at least a level 1 durak!");
	}

	float level() const {
		return mDurakLvl;
	}

private:
	float mDurakLvl;
};

class AttributeableTest
{
public:
	attribs::List lst{
		attribs::KY(),
		ElayAttribute(65.f)
	};
};

int main()
{
	TestManager testMgr;
	testMgr.AddFromAssembly(WAPIReflection::Assembly::local());
	testMgr.RunTests();

	/*
	try {
		using namespace std::literals;
		using namespace WAPIReflection;

		//typeof<FakMP>().asClass().construct({ 50, "elaymm4"s, 5.f });
		AttributeableTest a;
		for (auto& attr : a.lst)
		{
			if (attr.type() == typeof<attribs::KY>())
				std::cout << "We found KY!" << std::endl;
			else
				std::cout << attr.type().name() << std::endl;
		}

		SystemManager sysMgr;
		sysMgr.AddFromAssembly(Assembly::local());
		sysMgr.OnInitFinished();

		std::string msg;
		while (std::getline(std::cin, msg))
			sysMgr.OnTextMessage(msg);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		throw;
	}*/
}