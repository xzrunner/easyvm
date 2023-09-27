#include "easyvm/OpCodes.h"
#include "easyvm/VM.h"
#include "easyvm/VMHelper.h"

#include <string.h>
#include <stdio.h>

#define BINARY_MATH_OPERATION(func, op)						\
void OpCodeImpl::func(VM* vm)								\
{															\
	uint8_t reg_dst = vm->NextByte();						\
	uint8_t reg_src1 = vm->NextByte();						\
	uint8_t reg_src2 = vm->NextByte();						\
															\
	double src1 = VMHelper::GetRegNumber(vm, reg_src1);		\
	double src2 = VMHelper::GetRegNumber(vm, reg_src2);		\
															\
	Value val;                                              \
	val.type = ValueType::NUMBER;                           \
	val.as.number = src1 op src2;                           \
                                                            \
	vm->SetRegister(reg_dst, val);                          \
}

namespace evm
{

void OpCodeImpl::OpCodeInit(VM* vm)
{
	vm->RegistOperator(OP_EXIT, Exit);

	vm->RegistOperator(OP_BOOL_STORE, BoolStore);
	vm->RegistOperator(OP_BOOL_PRINT, BoolPrint);

	vm->RegistOperator(OP_NUMBER_STORE, NumberStore);
	vm->RegistOperator(OP_NUMBER_PRINT, NumberPrint);

	vm->RegistOperator(OP_ADD, Add);
	vm->RegistOperator(OP_SUB, Sub);
	vm->RegistOperator(OP_MUL, Mul);
	vm->RegistOperator(OP_DIV, Div);

	vm->RegistOperator(OP_STRING_STORE, StringStore);
	vm->RegistOperator(OP_STRING_PRINT, StringPrint);
}

void OpCodeImpl::Exit(VM* vm)
{
	vm->Stop();
	vm->NextInst();
}

void OpCodeImpl::BoolStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	Value val;
	val.type = ValueType::BOOLEAN;
	val.as.boolean = VMHelper::ReadData<bool>(vm);

	vm->SetRegister(reg, val);
}

void OpCodeImpl::BoolPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	bool val = VMHelper::GetRegBool(vm, reg);
	printf("%d", val);
	vm->NextInst();
}

void OpCodeImpl::NumberStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	Value val;
	val.type = ValueType::NUMBER;
	val.as.number = VMHelper::ReadData<double>(vm);

	vm->SetRegister(reg, val);
}

void OpCodeImpl::NumberPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	double val = VMHelper::GetRegNumber(vm, reg);
	printf("%f", val);
	vm->NextInst();
}

BINARY_MATH_OPERATION(Add, +)
BINARY_MATH_OPERATION(Sub, -)
BINARY_MATH_OPERATION(Mul, *)
BINARY_MATH_OPERATION(Div, /)

void OpCodeImpl::StringStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	uint16_t len = VMHelper::ReadData<uint16_t>(vm);

	char* str = new char(len + 1);
	if (!str) {
		vm->Error("alloc fail.");
	}
	memset(str, 0, len + 1);
	for (int i = 0; i < (int)len; i++) {
		str[i] = vm->NextByte();
	}

	Value val;
	val.type = ValueType::STRING;
	val.as.string = str;

	vm->SetRegister(reg, val);
}

void OpCodeImpl::StringPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	const char* val = VMHelper::GetRegString(vm, reg);
	printf("%s", val);
	vm->NextInst();
}

}