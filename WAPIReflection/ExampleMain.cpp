#include <iostream>
#include <Windows.h>
#include <DbgHelp.h>
#include <codecvt>
#include <filesystem>
#include <fstream>

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
		std::cout << mFak << " (" << mFloats[4] << "): " << a + b << std::endl;
		return true;
	}
private:
	int mPlak = 228;
	const char* mEbalak = "elamm4";
	std::string mFak = "Super fak result";

	float mFloats[64] = { 0 };
};

bool IsTypesSame(const Type& t1, const Type& t2)
{
	return t1 == t2;
}

void ShowType(const Type& t)
{
	auto methods = t.asClass().methods();
	auto fields = t.asClass().fields();

	std::cout << "class " << t.name();
	if (t.asClass().baseClass())
		std::cout << " : " << t.asClass().baseClass()->name();
	std::cout << std::endl << "{" << std::endl;
	for (auto& m : methods)
	{
		std::cout << "\t";
		
		if (m.isPure())
			std::cout << "virtual ";

		std::cout << m.signature().returnType().name() << " ";

		std::cout << m.name() << "(";

		auto params = m.signature().parameters();
		size_t i = 0;
		for (auto& pt : params)
		{
			std::cout << pt.name();
			if (i < params.size() - 1)
				std::cout << ", ";
			i++;
		}

		std::cout << ")";

		if (m.isPure())
			std::cout << " = 0";

		std::cout << ";" << std::endl;
	}
	std::cout << std::endl;
	for (auto& f : fields)
	{
		std::cout << "\t" << f.type().name() << " " << f.name() << "; // +0x" << std::hex << f.offset() << std::dec << std::endl;
	}
	std::cout << "};" << std::endl;

	if (t.asClass().baseClass())
	{
		std::cout << "Press any key to navigate to base!" << std::endl;
		std::cin.get();

		ShowType(*t.asClass().baseClass());
	}
}

enum class IAmAEnum
{
	One,
	Two,
	Three,
	Four,
	Six,
	Seven,
	Eight
};

struct ISystem {
	virtual ~ISystem() {}

	virtual void OnKy() = 0;
};

class AuthSystem : public ISystem {
public:
	void OnKy() {
		std::cout << "Auth" << std::endl;
	}
};
class PlayerSystem : public ISystem {
public:
	void OnKy() {
		std::cout << "Player" << std::endl;
	}
};
class DebugSystem : public ISystem {
public:
	void OnKy() {
		std::cout << "Debug" << std::endl;
	}
};

void printoutAny(Value val)
{
	std::cout << "Value: " << (uint64_t) val << std::endl;
}

int main()
{
	_wchdir(L"E:\\WAPIReflection\\Debug");

	try {
		auto& types = Assembly::local().getAllTypes();
		for (auto& t : types)
		{
			if (t.kind() == TypeKind::Class)
			{
				if (t.asClass().baseClass())
				{
					if (*t.asClass().baseClass() == typeof<ISystem>())
					{
						std::cout << t.name() << std::endl;
						void* pClass = t.asClass().constructNoParams();

						//auto pKy = t.asClass().findMethod("OnKy");
						//if(pKy)
						//	t.asClass().findMethod("OnKy")->invoke<void>(pClass);
					}
				}
			}
		}

		for (auto& e : typeof(TI_FINDCHILDREN).asEnum().enumValues())
		{
			std::cout << e.first << " = " << e.second << std::endl;
		}

		PlakableImpl plak;
		auto c = typeof(plak).asClass();
		auto fPlak = c.findField("mPlak")->bind(plak);
		fPlak = 255;

		bind_field<PlakableImpl>(plak, "mFloats")[4] = 24.5f;

		WNDCLASSEXA wcl;
		wcl.cbSize = 666;

		printoutAny(fPlak);
		printoutAny(typeof(wcl).asClass().findField("cbSize")->bind(wcl));

		c.findMethod("Plak")->invoke<bool>(&plak, 13, 12);

		ShowType(typeof<PlayerSystem>());

		Assembly ass("AssemblyTest.dll");
		ShowType(*ass.findType("ImAPlakStruct"));
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		throw;
	}
}