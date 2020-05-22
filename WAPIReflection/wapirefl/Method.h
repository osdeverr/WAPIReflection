#pragma once
#include "Symbol.h"
#include "FuncSignature.h"
#include <functional>

namespace WAPIReflection {
	class Method : public Symbol
	{
	public:
		Method(const Symbol& sym);

		FuncSignature signature() const;

		void* rawCodePtr() const;
		bool isPure() const;

		template<typename Class, typename ReturnType, typename... Args>
		std::function<ReturnType(Class&, Args...)> functionPtr() const
		{
			return [this](Class& pThis, Args... args) -> ReturnType {
				return ((ReturnType(__thiscall*)(Class&, Args...))(this->rawCodePtr()))(pThis, args...);
			};
		}

		template<typename ReturnType = void, class ...Args>
		ReturnType invoke(void* pThis, Args... args) const
		{
			return ((ReturnType(__thiscall*)(void*, Args...))mpCode)(pThis, args...);
		}

		template<typename Class, typename ReturnType, class ...Args>
		ReturnType invoke(Class& pThis, Args... args) const
		{
			((ReturnType(__thiscall*)(Class&, Args...))mpCode)(pThis, args...);
		}

		template<typename Class, typename ReturnType, class ...Args>
		ReturnType invoke(const Class& pThis, Args... args) const
		{
			((ReturnType(__thiscall*)(const Class&, Args...))mpCode)(pThis, args...);
		}

	private:
		void* mpCode;
	};
}