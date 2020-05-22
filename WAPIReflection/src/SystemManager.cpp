#include "SystemManager.h"
#include <cstdio>

SystemRef::SystemRef(const WAPIReflection::Class& class_, void* instance_)
	: mClass(class_), mInstance(instance_)
{
}

void SystemManager::AddSystemsFromAssembly(WAPIReflection::Assembly& assembly)
{
	for (auto& type : assembly.getAllTypes())
	{
		if (type.kind() == WAPIReflection::TypeKind::Class)
		{
			auto& cl = type.asClass();

			if (cl.baseClass() && *cl.baseClass() == typeof<ISystem>())
				AddSystemInternal(cl);
		}
	}
}

void SystemManager::OnTextMessage(const std::string& message)
{
	for (auto& sys : mSystems)
		sys.InvokeIfValid("OnTextMessage", message.c_str());
}

void SystemManager::OnInitFinished()
{
	for (auto& sys : mSystems)
		sys.InvokeIfValid("OnInitFinished");
}

void SystemManager::AddSystemInternal(const WAPIReflection::Class& type)
{
	std::printf(" => %s\n", type.name().c_str());

	mSystems.push_back(SystemRef(type, type.constructNoParams()));
}
