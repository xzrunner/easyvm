#include "easyvm/OpCodes.h"
#include "easyvm/VM.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define CHECK_REGISTER_BOUNDS(reg) \
	assert(reg >= 0 && reg < REGISTER_COUNT);

#define NEXT_INSTRUCTION(vm) \
	vm->m_ip++;

#define BINARY_MATH_OPERATION(func, op)						\
void OpCodeImpl::func(VM* vm)								\
{															\
	uint8_t reg_dst = vm->NextByte();						\
	CHECK_REGISTER_BOUNDS(reg_dst);							\
															\
	uint8_t reg_src1 = vm->NextByte();						\
	CHECK_REGISTER_BOUNDS(reg_src1);						\
															\
	uint8_t reg_src2 = vm->NextByte();						\
	CHECK_REGISTER_BOUNDS(reg_src2);						\
															\
	FreeReg(vm, reg_dst);									\
															\
	double src1 = GetNumberReg(vm, reg_src1);				\
	double src2 = GetNumberReg(vm, reg_src2);				\
															\
	vm->m_registers[reg_dst].type = ValueType::NUMBER;		\
	vm->m_registers[reg_dst].as.number = src1 + src2;		\
}

namespace evm
{

void OpCodeImpl::OpCodeInit(VM* vm)
{
	vm->m_opcodes[OP_EXIT] = Exit;

	vm->m_opcodes[OP_BOOL_STORE] = BoolStore;
	vm->m_opcodes[OP_BOOL_PRINT] = BoolPrint;

	vm->m_opcodes[OP_NUMBER_STORE] = NumberStore;
	vm->m_opcodes[OP_NUMBER_PRINT] = NumberPrint;

	vm->m_opcodes[OP_ADD] = Add;
	vm->m_opcodes[OP_SUB] = Sub;
	vm->m_opcodes[OP_MUL] = Mul;
	vm->m_opcodes[OP_DIV] = Div;

	vm->m_opcodes[OP_STRING_STORE] = StringStore;
	vm->m_opcodes[OP_STRING_PRINT] = StringPrint;
}

void OpCodeImpl::Exit(VM* vm)
{
	vm->m_running = false;
	NEXT_INSTRUCTION(vm);
}

void OpCodeImpl::BoolStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	bool val = ReadData<bool>(vm);

	FreeReg(vm, reg);

	CHECK_REGISTER_BOUNDS(reg);
	vm->m_registers[reg].type = ValueType::BOOLEAN;
	vm->m_registers[reg].as.boolean = val;
}

void OpCodeImpl::BoolPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	bool val = GetBoolReg(vm, reg);
	printf("%d", val);
	NEXT_INSTRUCTION(vm);
}

void OpCodeImpl::NumberStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	double val = ReadData<double>(vm);

	FreeReg(vm, reg);

	CHECK_REGISTER_BOUNDS(reg);
	vm->m_registers[reg].type = ValueType::NUMBER;
	vm->m_registers[reg].as.number = val;
}

void OpCodeImpl::NumberPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	double val = GetNumberReg(vm, reg);
	printf("%f", val);
	NEXT_INSTRUCTION(vm);
}

BINARY_MATH_OPERATION(Add, +)
BINARY_MATH_OPERATION(Sub, -)
BINARY_MATH_OPERATION(Mul, *)
BINARY_MATH_OPERATION(Div, /)

void OpCodeImpl::StringStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	uint16_t len = ReadData<uint16_t>(vm);

	char* val = new char(len + 1);
	if (!val) {
		vm->Error("alloc fail.");
	}
	memset(val, 0, len + 1);
	for (int i = 0; i < (int)len; i++) {
		val[i] = vm->NextByte();
	}

	FreeReg(vm, reg);
	CHECK_REGISTER_BOUNDS(reg);
	vm->m_registers[reg].type = ValueType::STRING;
	vm->m_registers[reg].as.string = val;
}

void OpCodeImpl::StringPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	const char* val = GetStringReg(vm, reg);
	printf("%s", val);
	NEXT_INSTRUCTION(vm);
}

template<typename T>
T OpCodeImpl::ReadData(evm::VM* vm)
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

bool OpCodeImpl::GetBoolReg(evm::VM* vm, int reg)
{
	CHECK_REGISTER_BOUNDS(reg);
	if (vm->m_registers[reg].type == ValueType::BOOLEAN) {
		return vm->m_registers[reg].as.boolean;
	} else {
		vm->Error("The register doesn't contain a boolean.");
		return false;
	}
}

double OpCodeImpl::GetNumberReg(evm::VM* vm, int reg)
{
	CHECK_REGISTER_BOUNDS(reg);
	if (vm->m_registers[reg].type == ValueType::NUMBER) {
		return vm->m_registers[reg].as.number;
	} else {
		vm->Error("The register doesn't contain a number.");
		return false;
	}
}

char* OpCodeImpl::GetStringReg(evm::VM* vm, int reg)
{
	CHECK_REGISTER_BOUNDS(reg);
	if (vm->m_registers[reg].type == ValueType::STRING) {
		return vm->m_registers[reg].as.string;
	} else {
		vm->Error("The register doesn't contain a string.");
		return false;
	}
}

void OpCodeImpl::FreeReg(VM* vm, uint8_t reg)
{
	CHECK_REGISTER_BOUNDS(reg);
	auto& v = vm->m_registers[reg];
	
	if (v.type == ValueType::STRING && v.as.string) {
		delete(v.as.string);
	} 
	// todo user data
}

}