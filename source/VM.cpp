#include "easyvm/VM.h"
#include "easyvm/OpCodes.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CHECK_REGISTER_BOUNDS(reg) \
	assert(reg >= 0 && reg < REGISTER_COUNT);

namespace
{

void FreeReg(evm::VM* vm, uint8_t reg)
{
    evm::Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return;
	}

	if (val.type == evm::ValueType::STRING && val.as.string) {
		delete(val.as.string);
	} else if (val.type == evm::ValueType::HANDLE && val.as.handle) {
		delete(val.as.handle);
	}
}

}

namespace evm
{

VM::VM(const char* code, size_t size)
    : m_code(code)
    , m_size(size)
{
	InitOpcodes();
}

void VM::Run()
{
    m_ip = 0;

    int iterations = 0;
    while (m_running)
    {
        if (m_ip >= m_size) {
            break;
        }

        int opcode = m_code[m_ip];

        if (m_opcodes[opcode]) {
            m_opcodes[opcode](this);
        }

        if (m_ip == m_size - 1) {
            break;
        }

        NextByte();
    }
}

bool VM::GetRegister(int reg, Value& val)
{
    CHECK_REGISTER_BOUNDS(reg);

    if (reg < 0 || reg > REGISTER_COUNT) 
    {
        return false;
    }
    else
    {
        val = m_registers[reg];
        return true;
    }
}

void VM::SetRegister(int reg, const Value& val)
{
    CHECK_REGISTER_BOUNDS(reg);

    if (reg >= 0 && reg < REGISTER_COUNT) 
    {
        FreeReg(this, reg);
        m_registers[reg] = val;
    }
}

bool VM::MoveRegister(int reg, Value& val)
{
    CHECK_REGISTER_BOUNDS(reg);

    if (reg < 0 || reg > REGISTER_COUNT)
    {
        return false;
    }
    else
    {
        val = m_registers[reg];

        if (m_registers[reg].type == ValueType::STRING) {
            m_registers[reg].as.string = nullptr;
        } else if (m_registers[reg].type == ValueType::HANDLE) {
            m_registers[reg].as.handle = nullptr;
        }

        return true;
    }
}

void VM::NextInst()
{
    m_ip++;
}

unsigned char VM::NextByte()
{
    m_ip += 1;

    if (m_ip >= m_size) {
        m_ip = 0;
    }

    return m_code[m_ip];
}

void VM::Error(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void VM::RegistOperator(int opcode, OpcodeImpl* func)
{
    m_opcodes[opcode] = func;
}

void VM::Stop()
{
    m_running = false;
}

void VM::InitOpcodes()
{
    OpCodeImpl::OpCodeInit(this);
}

}