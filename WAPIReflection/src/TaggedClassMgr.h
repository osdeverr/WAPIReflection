#pragma once
#include <wapirefl/Assembly.h>
#include <wapirefl/Class.h>

// Represents a reference to a class instance.
// Provides methods to invoke arbitrary methods of said class.
class InstanceRef
{
public:
	// Constructs an InstanceRef from the specified class.
	InstanceRef(const WAPIReflection::Class& class_)
		: mClass(class_), mInstance(class_.constructNoParams())
	{}

	// Invokes a method of the class this instance is bound to.
	// Does nothing if the specified method does not exist.
	// NOTE: you are responsible for checking arguments' validity!
	template<typename... Args>
	void InvokeIfValid(const std::string& method, Args... args)
	{
		auto pMethod = mClass.findMethod(method);
		if (pMethod)
			pMethod->invoke<void, Args...>(mInstance, args...);
	}

private:
	WAPIReflection::Class mClass;
	void* mInstance;
};

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
		for (auto& type : assembly.getAllTypes())
		{
			if (type.kind() == WAPIReflection::TypeKind::Class)
			{
				auto& cl = type.asClass();

				if (cl.baseClass() && *cl.baseClass() == typeof<TClassTag>())
					AddInternal(cl);
			}
		}
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
		std::printf(" => %s\n", type.name().c_str());
		mInstances.push_back(InstanceRef(type));
	}

	std::vector<InstanceRef> mInstances;
};
