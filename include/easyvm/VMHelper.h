#pragma once

#include <stdint.h>

#include <memory>
#include <vector>

namespace evm
{

class VM;
struct Value;

class VMHelper
{
public:
	template<typename T>
	static T ReadData(evm::VM* vm)
	{
		T ret = 0;

		const int sz = sizeof(T);
		uint8_t data[sz];
		for (int i = 0; i < sz; ++i) {
			data[i] = vm->NextByte();
		}
		memcpy(&ret, data, sz);

		return ret;
	}

	static bool   GetRegBool(VM* vm, int reg);
	static double GetRegNumber(VM* vm, int reg);
	static char*  GetRegString(VM* vm, int reg);

	template<typename T>
	static std::shared_ptr<T> GetRegHandler(evm::VM* vm, int reg)
	{
		evm::Value val;
		if (!vm->GetRegister(reg, val))
		{
			return nullptr;
		}

		if (val.type < evm::ValueType::V_HANDLE) {
			vm->Error("The register doesn't contain a handler.");
			return nullptr;
		}

		return static_cast<evm::Handle<T>*>(val.as.handle)->obj;
	}

}; // VMHelper

}