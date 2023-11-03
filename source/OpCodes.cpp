#include "easyvm/OpCodes.h"
#include "easyvm/VM.h"
#include "easyvm/VMHelper.h"

#include <string.h>
#include <stdio.h>

#define BINARY_MATH_OPERATION(func, op)						\
void OpCodeImpl::func(VM* vm)								\
{															\
	uint8_t r_dst  = vm->NextByte();						\
	uint8_t r_src1 = vm->NextByte();						\
	uint8_t r_src2 = vm->NextByte();						\
															\
	t_num src1 = VMHelper::GetRegNumber(vm, r_src1);		\
	t_num src2 = VMHelper::GetRegNumber(vm, r_src2);		\
															\
	Value val;                                              \
	val.type = ValueType::V_NUMBER;                         \
	val.as.number = src1 op src2;                           \
                                                            \
	vm->SetRegister(r_dst, val);							\
}

namespace evm
{

void OpCodeImpl::OpCodeInit(VM* vm)
{
	vm->RegistOperator(OP_EXIT, Exit);

	vm->RegistOperator(OP_IS_NIL, IsNil);
	vm->RegistOperator(OP_MOVE_VAL, MoveVal);

	vm->RegistOperator(OP_BOOL_STORE, BoolStore);
	vm->RegistOperator(OP_BOOL_PRINT, BoolPrint);
	vm->RegistOperator(OP_AND, And);
	vm->RegistOperator(OP_OR, Or);

	vm->RegistOperator(OP_NUMBER_STORE, NumberStore);
	vm->RegistOperator(OP_NUMBER_PRINT, NumberPrint);
	vm->RegistOperator(OP_NUMBER_NEGATE, NumberNegate);

	vm->RegistOperator(OP_SQRT, Sqrt);

	vm->RegistOperator(OP_ADD, Add);
	vm->RegistOperator(OP_SUB, Sub);
	vm->RegistOperator(OP_MUL, Mul);
	vm->RegistOperator(OP_DIV, Div);

	vm->RegistOperator(OP_INC, Inc);
	vm->RegistOperator(OP_DEC, Dec);

	vm->RegistOperator(OP_EQUAL, Equal);
	vm->RegistOperator(OP_LESS, Less);

	vm->RegistOperator(OP_JUMP, Jump);
	vm->RegistOperator(OP_JUMP_IF, JumpIf);
	vm->RegistOperator(OP_JUMP_IF_NOT, JumpIfNot);

	vm->RegistOperator(OP_STRING_STORE, StringStore);
	vm->RegistOperator(OP_STRING_PRINT, StringPrint);
}

void OpCodeImpl::Exit(VM* vm)
{
	vm->Stop();
}

void OpCodeImpl::IsNil(VM* vm)
{
	uint8_t r_dst = vm->NextByte();
	uint8_t r_src = vm->NextByte();

	bool is_nil = true;
	if (r_src != 0xff)
	{
		auto& val = vm->GetRegister(r_src);
		is_nil = val.type == V_NIL;
	}

	Value val;
	val.type = ValueType::V_BOOLEAN;
	val.as.boolean = is_nil;

	vm->SetRegister(r_dst, val);
}

void OpCodeImpl::MoveVal(VM* vm)
{
	uint8_t r_dst = vm->NextByte();
	uint8_t r_src = vm->NextByte();

	if (r_src == 0xff) 
	{
		vm->SetRegister(r_dst, Value());
	}
	else
	{
		auto& val = vm->GetRegister(r_src);
		vm->SetRegister(r_dst, val);
	}
}

void OpCodeImpl::BoolStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	Value val;
	val.type = ValueType::V_BOOLEAN;
	val.as.boolean = VMHelper::ReadData<bool>(vm);

	vm->SetRegister(reg, val);
}

void OpCodeImpl::BoolPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	bool val = VMHelper::GetRegBool(vm, reg);
	printf("%d", val);
}

void OpCodeImpl::And(VM* vm)
{
	uint8_t r_dst = vm->NextByte();
	uint8_t r_src1 = vm->NextByte();
	uint8_t r_src2 = vm->NextByte();

	bool src1 = r_src1 == 0xff ? false : VMHelper::GetRegBool(vm, r_src1);
	bool src2 = r_src2 == 0xff ? false : VMHelper::GetRegBool(vm, r_src2);

	Value val;
	val.type = ValueType::V_BOOLEAN;
	val.as.boolean = src1 && src2;

	vm->SetRegister(r_dst, val);
}

void OpCodeImpl::Or(VM* vm)
{
	uint8_t r_dst = vm->NextByte();
	uint8_t r_src1 = vm->NextByte();
	uint8_t r_src2 = vm->NextByte();

	bool src1 = r_src1 == 0xff ? false : VMHelper::GetRegBool(vm, r_src1);
	bool src2 = r_src2 == 0xff ? false : VMHelper::GetRegBool(vm, r_src2);

	Value val;
	val.type = ValueType::V_BOOLEAN;
	val.as.boolean = src1 || src2;

	vm->SetRegister(r_dst, val);
}

void OpCodeImpl::NumberStore(VM* vm)
{
	uint8_t reg = vm->NextByte();

	Value val;
	val.type = ValueType::V_NUMBER;
	val.as.number = VMHelper::ReadData<t_num>(vm);

	vm->SetRegister(reg, val);
}

void OpCodeImpl::NumberPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	t_num val = VMHelper::GetRegNumber(vm, reg);
	printf("%f", val);
}

void OpCodeImpl::NumberNegate(VM* vm)
{
	uint8_t r_dst = vm->NextByte();

	uint8_t r_src = vm->NextByte();
	t_num num = VMHelper::GetRegNumber(vm, r_src);

	Value val;
	val.type = ValueType::V_NUMBER;
	val.as.number = -num;

	vm->SetRegister(r_dst, val);
}

void OpCodeImpl::Sqrt(VM* vm)
{
	uint8_t r_dst = vm->NextByte();

	uint8_t r_src = vm->NextByte();
	t_num num = VMHelper::GetRegNumber(vm, r_src);

	Value val;
	val.type = ValueType::V_NUMBER;
	val.as.number = sqrt(num);

	vm->SetRegister(r_dst, val);
}

BINARY_MATH_OPERATION(Add, +)
BINARY_MATH_OPERATION(Sub, -)
BINARY_MATH_OPERATION(Mul, *)
BINARY_MATH_OPERATION(Div, /)

void OpCodeImpl::Inc(VM* vm)
{
	uint8_t r_num = vm->NextByte();

	t_num num = VMHelper::GetRegNumber(vm, r_num);

	Value val;
	val.type = ValueType::V_NUMBER;
	val.as.number = num + 1;

	vm->SetRegister(r_num, val);
}

void OpCodeImpl::Dec(VM* vm)
{
	uint8_t r_num = vm->NextByte();

	t_num num = VMHelper::GetRegNumber(vm, r_num);

	Value val;
	val.type = ValueType::V_NUMBER;
	val.as.number = num - 1;

	vm->SetRegister(r_num, val);
}

void OpCodeImpl::Equal(VM* vm)
{
	uint8_t r_dst = vm->NextByte();
	uint8_t r_src1 = vm->NextByte();
	uint8_t r_src2 = vm->NextByte();

	t_num src1 = VMHelper::GetRegNumber(vm, r_src1);
	t_num src2 = VMHelper::GetRegNumber(vm, r_src2);

	Value val;
	val.type = ValueType::V_BOOLEAN;
	val.as.boolean = src1 == src2;

	vm->SetRegister(r_dst, val);
}

void OpCodeImpl::Less(VM* vm)
{
	uint8_t r_dst = vm->NextByte();
	uint8_t r_src1 = vm->NextByte();
	uint8_t r_src2 = vm->NextByte();

	t_num src1 = VMHelper::GetRegNumber(vm, r_src1);
	t_num src2 = VMHelper::GetRegNumber(vm, r_src2);

	Value val;
	val.type = ValueType::V_BOOLEAN;
	val.as.boolean = src1 < src2;

	vm->SetRegister(r_dst, val);
}

void OpCodeImpl::Jump(VM* vm)
{
	int ptr = VMHelper::ReadData<int>(vm);
	vm->JumpTo(ptr - 1);
}

void OpCodeImpl::JumpIf(VM* vm)
{
	int ptr = VMHelper::ReadData<int>(vm);
	uint8_t r_bool = vm->NextByte();

	bool b = VMHelper::GetRegBool(vm, r_bool);
	if (b) {
		vm->JumpTo(ptr - 1);
	}
}

void OpCodeImpl::JumpIfNot(VM* vm)
{
	int ptr = VMHelper::ReadData<int>(vm);
	uint8_t r_bool = vm->NextByte();

	bool b = VMHelper::GetRegBool(vm, r_bool);
	if (!b) {
		vm->JumpTo(ptr - 1);
	}
}

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
	val.type = ValueType::V_STRING;
	val.as.string = str;

	vm->SetRegister(reg, val);
}

void OpCodeImpl::StringPrint(VM* vm)
{
	uint8_t reg = vm->NextByte();
	const char* val = VMHelper::GetRegString(vm, reg);
	printf("%s", val);
}

}