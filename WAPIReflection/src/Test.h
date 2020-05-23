#pragma once
#include <wapirefl/Assembly.h>
#include <wapirefl/Type.h>

#include <sstream>

class Test
{
protected:
	bool Assert(bool value)
	{
		if (!value)
			throw std::invalid_argument("Test case failed");
		else
			return true;
	}
private:
	WAPIReflection::Type mTestType;
};
