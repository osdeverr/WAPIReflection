#pragma once
#include "TaggedClassMgr.h"
#include "Test.h"

class TestManager : public TaggedClassMgr<Test>
{
public:
	void RunTests();
};
