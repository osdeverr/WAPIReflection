#pragma once
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