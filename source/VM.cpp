#include "easyvm/VM.h"
#include "easyvm/OpCodes.h"

#include <stdio.h>
#include <stdlib.h>

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

bool VM::Load(int reg, Value& val)
{
    if (reg < 0 || reg > REGISTER_COUNT) {
        return false;
    }

    val = m_registers[reg];

    return true;
}

void VM::InitOpcodes()
{
    OpCodeImpl::OpCodeInit(this);
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

}