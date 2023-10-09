#pragma once

#include <stdint.h>

namespace evm
{

enum OpCode
{
	OP_EXIT = 0,

	OP_BOOL_STORE,
	OP_BOOL_PRINT,

	OP_NUMBER_STORE,
	OP_NUMBER_PRINT,
	OP_NUMBER_NEGATE,

	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,

	OP_INC,
	OP_DEC,

	OP_CMP,

	OP_JUMP_IF_NOT,

	OP_STRING_STORE,
	OP_STRING_PRINT,
};

class VM;

class OpCodeImpl
{
public:
	static void OpCodeInit(VM* vm);

private:
	static void Exit(VM* vm);

	static void BoolStore(VM* vm);
	static void BoolPrint(VM* vm);

	static void NumberStore(VM* vm);
	static void NumberPrint(VM* vm);
	static void NumberNegate(VM* vm);

	static void Add(VM* vm);
	static void Sub(VM* vm);
	static void Mul(VM* vm);
	static void Div(VM* vm);

	static void Inc(VM* vm);
	static void Dec(VM* vm);

	static void Cmp(VM* vm);

	static void JumpIfNot(VM* vm);

	static void StringStore(VM* vm);
	static void StringPrint(VM* vm);

}; // OpCodeImpl

}