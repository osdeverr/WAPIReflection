#pragma once
#include <wapirefl/Assembly.h>
#include <wapirefl/Class.h>

#include "InstanceRef.h"

// Manages a list of instances of classes distinguished by inheritance from the Tag type.
// Provides methods to add new classes, find them in WAPIRefl assemblies and invoke methods on all instances.
template<class TClassTag>
class TaggedClassMgr
{
public:
	// Checks if T inherits from TClassTag: if it does, constructs an instance of T
	// and adds it to the instance list. If it doesn't, does nothing.
	template<typename T>
	void Add() {
		auto& type = typeof<T>();
		if (type.kind() == WAPIReflection::TypeKind::Class)
		{
			auto& cl = type.asClass();

			if (cl.baseClass() && *cl.baseClass() == typeof<TClassTag>())
				AddInternal(cl);
		}
	}

	// Constructs all classes inheriting from TClassTag in the specified assembly.
	// Adds the resulting instances to the internal list.
	void AddFromAssembly(WAPIReflection::Assembly& assembly)
	{
		auto tagType = typeof<TClassTag>();
		std::printf(" [TCM:%s] Loading assembly types...\n", tagType.name().c_str());

		/*

		assembly.forEachType([&](const WAPIReflection::Symbol& sym) {
			if (sym.tag() == WAPIReflection::SymTag::UDT)
			{
				auto children = sym.children();
				for(auto& c : children)
					if(c.tag() == WAPIReflection::SymTag::BaseClass)
						if (c == tagType)
							AddInternal(WAPIReflection::Class(sym));
			}
			});*/

		char chars[] { '-', '\\', '|', '/' };
		
		auto& types = assembly.getAllTypes();
		size_t i = 0;

		std::string oldName = "";

		for (auto& type : types)
		{
			if (type.kind() == WAPIReflection::TypeKind::Class)
			{
				auto cl = type.asClass();

				if (cl.baseClass() && *cl.baseClass() == tagType)
					AddInternal(cl);
			}
		}

		/*std::putchar('\r');
		for (size_t j = 0; j < oldName.length() + 3; j++)
			std::putchar(' ');
		std::putchar('\r');*/
	}

protected:
	// Invokes a method on all instances with the specified parameters.
	// If an instance's class does not contain such method, it's ignored.
	template<typename... Args>
	void InvokeOnAll(const std::string& method, Args... args)
	{
		for (auto& sys : mInstances)
			sys.InvokeIfValid(method, args...);
	}

	// Constructs a class instance without TClassTag checks.
	void AddInternal(const WAPIReflection::Class& type)
	{
		auto tagType = typeof<TClassTag>();
		std::printf(" [TCM:%s] Loading type %s\n", tagType.name().c_str(), type.name().c_str());
		mInstances.push_back(InstanceRef(type));
	}

	std::vector<InstanceRef> mInstances;
};
