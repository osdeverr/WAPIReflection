#include <iostream>
#include <Windows.h>
#include <DbgHelp.h>
#include <codecvt>
#include <filesystem>

#include "Assembly.h"
#include "Type.h"
#include <map>

#include "Enum.h"
#include "Class.h"

#include <d3d9.h>

#include <cassert>

using namespace WAPIReflection;

struct IPlakable
{
	virtual ~IPlakable() {}

	virtual bool Plak(int a, int b) = 0;
};
class PlakableImpl : public IPlakable
{
public:
	bool Plak(int a, int b) {
		std::cout << mFak << ": " << a + b << std::endl;
		return true;
	}
private:
	int mPlak;
	const char* mEbalak;
	std::string mFak;
};

enum ElayEnum
{
	Elay,
	Is,
	Durak,
	Ebanat,
	Dolboeb
};

bool IsTypesSame(const Type& t1, const Type& t2)
{
	return t1 == t2;
}

int main()
{
	_wchdir(L"E:\\WAPIReflection\\Debug");

	PlakableImpl e;

	const Type& t = typeof(e);
	auto methods = t.asClass().methods();
	auto fields = t.asClass().fields();

	assert(typeof<HWND>() == typeof<HWND>());

	std::cout << "class " << t.name();
	if (t.asClass().baseClass())
		std::cout << " : " << t.asClass().baseClass()->name();
	std::cout << std::endl << "{" << std::endl;
	for (auto& m : methods)
	{
		std::cout << "\t" << m.signature().returnType().name() << " " << m.name() << "(";

		auto params = m.signature().parameters();
		size_t i = 0;
		for (auto& pt : params)
		{
			std::cout << pt.name();
			if (i < params.size() - 1)
				std::cout << ", ";
			i++;
		}

		std::cout << ");" << std::endl;
	}
	for (auto& f : fields)
	{
		std::cout << "\t" << f.type().name() << " " << f.name() << ";" << std::endl;
	}
	std::cout << "}" << std::endl;
}