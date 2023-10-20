#include "easyvm/VM.h"
#include "easyvm/OpCodes.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace
{

void FreeReg(evm::VM* vm, uint8_t reg)
{
    evm::Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return;
	}

	if (val.type == evm::ValueType::V_STRING && val.as.string) 
    {
		delete(val.as.string);
        val.as.string = nullptr;
	} 
    else if (val.type >= evm::ValueType::V_HANDLE && val.as.handle)
    {
		delete(val.as.handle);
        val.as.handle = nullptr;
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

void VM::Run(int begin, int end)
{
    if (end == 0) {
        end = m_size;
    } else {
        end = std::min(static_cast<size_t>(end), m_size);
    }

    if (begin >= end) {
        return;
    }

    m_ip = begin;

    int iterations = 0;
    while (m_running)
    {
        if (m_ip >= end) {
            break;
        }

        int opcode = m_code[m_ip];

        if (m_opcodes[opcode]) {
            m_opcodes[opcode](this);
        }

        if (m_ip == end - 1) {
            break;
        }

        NextByte();
    }
}

bool VM::GetRegister(int reg, Value& val)
{
    if (reg < 0 || reg >= REGISTER_COUNT) 
    {
        return false;
    }
    else
    {
        val = m_registers[reg];
        return true;
    }
}

bool VM::SetRegister(int reg, const Value& val)
{
    if (reg < 0 || reg >= REGISTER_COUNT)
    {
        return false;
    }
    else
    {
        FreeReg(this, reg);
        m_registers[reg] = val;
        return true;
    }
}

bool VM::MoveRegister(int reg, Value& val)
{
    if (reg < 0 || reg >= REGISTER_COUNT)
    {
        return false;
    }
    else
    {
        val = m_registers[reg];

        if (m_registers[reg].type == ValueType::V_STRING) {
            m_registers[reg].as.string = nullptr;
        } else if (m_registers[reg].type >= ValueType::V_HANDLE) {
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

void VM::Jump(int offset)
{
    int pos = m_ip + offset;
    pos = std::max(0, std::min(pos, static_cast<int>(m_size)));
    m_ip = pos;
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