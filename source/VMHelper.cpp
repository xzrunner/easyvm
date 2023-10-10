#include "easyvm/VMHelper.h"
#include "easyvm/VM.h"

namespace evm
{

bool VMHelper::GetRegBool(VM* vm, int reg)
{
	Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return false;
	}

	if (val.type != ValueType::V_BOOLEAN) {
		vm->Error("The register doesn't contain a boolean.");
		return false;
	}

	return val.as.boolean;
}

double VMHelper::GetRegNumber(VM* vm, int reg)
{
	Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return 0.0;
	}

	if (val.type != ValueType::V_NUMBER) {
		vm->Error("The register doesn't contain a number.");
		return 0.0;
	}

	return val.as.number;
}

char* VMHelper::GetRegString(VM* vm, int reg)
{
	Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return nullptr;
	}

	if (val.type != ValueType::V_STRING) {
		vm->Error("The register doesn't contain a string.");
		return nullptr;
	}

	return val.as.string;
}

}