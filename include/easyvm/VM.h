#pragma once

#include "Value.h"

#include <array>

namespace evm
{

#define REGISTER_COUNT 10

class VM
{
public:
	VM(const char* code, size_t size);

	void Run();

	bool Load(int reg, Value& val);

private:
	void InitOpcodes();

	unsigned char NextByte();

	void Error(const char* msg);

private:
	std::array<Value, REGISTER_COUNT> m_registers;

	unsigned int m_ip = 0;

	const char* m_code = nullptr;
	size_t m_size = 0;

	typedef void OpcodeImpl(VM* vm);
	OpcodeImpl* m_opcodes[256];

	bool m_running = true;

	friend class OpCodeImpl;

}; // VM

}