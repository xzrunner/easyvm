#pragma once

#include "VM.h"

#include <stdint.h>

#include <memory>
#include <vector>

namespace evm
{

class VM;
class Value;

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

	static bool  GetRegBool(VM* vm, int reg);
	static t_num GetRegNumber(VM* vm, int reg);
	static char* GetRegString(VM* vm, int reg);

	template<typename T>
	static std::shared_ptr<T> GetHandleValue(const evm::Value& val)
	{
		return static_cast<evm::Handle<T>*>(val.as.handle)->obj;
	}

	template<typename T>
	static std::shared_ptr<T> GetRegHandler(evm::VM* vm, int reg)
	{
		if (reg < 0 || reg >= REGISTER_COUNT) {
			return nullptr;
		}

		auto& val = vm->GetRegister(reg);
		if (val.type >= evm::ValueType::V_HANDLE) {
			return GetHandleValue<T>(val);
		} else {
			vm->Error("Is not a handler!");
			return nullptr;
		}
	}

}; // VMHelper

}