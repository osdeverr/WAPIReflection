#pragma once
#include <wapirefl/Assembly.h>
#include <wapirefl/Class.h>

#include "ISystem.h"

class SystemRef
{
public:
	SystemRef(const WAPIReflection::Class& class_, void* instance_);

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

class SystemManager
{
public:
	template<typename T>
	void AddSystem() {
		auto& type = typeof<T>();
		if (type.kind() == WAPIReflection::TypeKind::Class)
		{
			auto& cl = type.asClass();

			if (cl.baseClass() && *cl.baseClass() == typeof<ISystem>())
				AddSystemInternal(cl);
		}
	}
	void AddSystemsFromAssembly(WAPIReflection::Assembly& assembly);

	void OnTextMessage(const std::string& message);
	void OnInitFinished();
private:
	void AddSystemInternal(const WAPIReflection::Class& type);
	std::vector<SystemRef> mSystems;
};
