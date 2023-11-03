#include "easyvm/VM.h"
#include "easyvm/OpCodes.h"

#include <stdexcept>

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

const Value& VM::GetRegister(int reg) const
{
    if (reg < 0 || reg >= REGISTER_COUNT) {
        throw std::runtime_error("Error reg!");
    } else {
        return m_registers[reg];
    }
}

void VM::SetRegister(int reg, const Value& val)
{
    if (reg < 0 || reg >= REGISTER_COUNT) {
        throw std::runtime_error("Error reg!");
    } else {
        m_registers[reg] = val;
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

void VM::JumpTo(int ip)
{
    m_ip = std::max(0, std::min(ip, static_cast<int>(m_size - 1)));
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