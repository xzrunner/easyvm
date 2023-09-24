#pragma once

#include <stdint.h>

namespace evm
{

class VM;

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

	static bool GetBoolReg(VM* vm, int reg);
	static double GetNumberReg(VM* vm, int reg);
	static char* GetStringReg(VM* vm, int reg);

}; // VMHelper

}