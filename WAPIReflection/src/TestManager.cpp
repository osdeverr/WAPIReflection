#include "TestManager.h"
#include <iostream>

namespace detail { // SO here to help
	template<typename TString>
	inline bool starts_with(const TString& str, const TString& start) {
		if (start.size() > str.size()) return false;
		return str.compare(0, start.size(), start) == 0;
	}
	template<typename TString>
	inline bool ends_with(const TString& str, const TString& end) {
		if (end.size() > str.size()) return false;
		return std::equal(end.rbegin(), end.rend(), str.rbegin());
	}
}

void TestManager::RunTests()
{
	std::cout << " --- Running tests ---" << std::endl;

	for (auto& inst : mInstances)
	{
		auto& cl = inst.GetClass();
		std::cout << "     [+] Test block: " << cl.name() << std::endl;
		for (auto& m : cl.methods())
		{
			auto name = m.name();
			std::string suff = "Test";
			if (detail::ends_with(name, suff) && m.signature().parameters().empty())
			{
				try {
					std::cout << "         ==> Test case: " << name << std::endl;
					std::cout << std::endl << " --- TEST OUTPUT BEGIN ---" << std::endl << std::endl;
					inst.InvokeIfValid(name);
					std::cout << std::endl << " --- TEST OUTPUT END ---" << std::endl << std::endl;
				}
				catch (...)
				{
					std::cerr << "Test case " << name << " @ " + cl.name() << " failed.";
					throw;
				}
			}
		}
	}
}
