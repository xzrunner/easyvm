#pragma once

#include "Value.h"

#include <array>

namespace evm
{

#define REGISTER_COUNT 16

class VM;
typedef void OpcodeImpl(VM* vm);

class VM
{
public:
	VM(const char* code, size_t size);

	void Run();

	bool GetRegister(int reg, Value& val);
	bool SetRegister(int reg, const Value& val);
	bool MoveRegister(int reg, Value& val);

	void NextInst();
	unsigned char NextByte();
	void Jump(int offset);

	void Error(const char* msg);

	void RegistOperator(int opcode, OpcodeImpl* func);

	void Stop();

private:
	void InitOpcodes();

private:
	std::array<Value, REGISTER_COUNT> m_registers;

	unsigned int m_ip = 0;

	const char* m_code = nullptr;
	size_t m_size = 0;

	OpcodeImpl* m_opcodes[256];

	bool m_running = true;

}; // VM

}