#include "easyvm/VMHelper.h"
#include "easyvm/VM.h"

namespace evm
{

bool VMHelper::GetRegBool(VM* vm, int reg)
{
	auto& val = vm->GetRegister(reg);
	if (val.type == ValueType::V_NIL) {
		return false;
	} else if (val.type == ValueType::V_BOOLEAN) {
		return val.as.boolean;
	} else {
		vm->Error("The register doesn't contain a boolean.");
		return false;
	}
}

t_num VMHelper::GetRegNumber(VM* vm, int reg)
{
	auto& val = vm->GetRegister(reg);
	if (val.type == ValueType::V_NIL) {
		return 0;
	} else if (val.type == ValueType::V_NUMBER) {
		return val.as.number;
	} else {
		vm->Error("The register doesn't contain a number.");
		return 0;
	}
}

char* VMHelper::GetRegString(VM* vm, int reg)
{
	auto& val = vm->GetRegister(reg);
	if (val.type == ValueType::V_STRING) {
		return val.as.string;
	} else {
		vm->Error("The register doesn't contain a string.");
		return nullptr;
	}
}

}