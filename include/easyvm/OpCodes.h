#pragma once

#include <stdint.h>

namespace evm
{

enum OpCode
{
	OP_EXIT = 0,

	OP_SET_NIL,
	OP_IS_NIL,
	OP_MOVE_VAL,

	OP_BOOL_STORE,
	OP_BOOL_PRINT,

	OP_AND,
	OP_OR,

	OP_NUMBER_STORE,
	OP_NUMBER_PRINT,
	OP_NUMBER_NEGATE,

	OP_SQRT,

	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,

	OP_INC,
	OP_DEC,

	OP_EQUAL,
	OP_LESS,

	OP_JUMP,
	OP_JUMP_IF,
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

	static void SetNil(VM* vm);
	static void IsNil(VM* vm);

	static void MoveVal(VM* vm);

	static void BoolStore(VM* vm);
	static void BoolPrint(VM* vm);

	static void And(VM* vm);
	static void Or(VM* vm);

	static void NumberStore(VM* vm);
	static void NumberPrint(VM* vm);
	static void NumberNegate(VM* vm);

	static void Sqrt(VM* vm);

	static void Add(VM* vm);
	static void Sub(VM* vm);
	static void Mul(VM* vm);
	static void Div(VM* vm);

	static void Inc(VM* vm);
	static void Dec(VM* vm);

	static void Equal(VM* vm);
	static void Less(VM* vm);

	static void Jump(VM* vm);
	static void JumpIf(VM* vm);
	static void JumpIfNot(VM* vm);

	static void StringStore(VM* vm);
	static void StringPrint(VM* vm);

}; // OpCodeImpl

}