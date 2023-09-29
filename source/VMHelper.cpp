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

	if (val.type != ValueType::BOOLEAN) {
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
		return false;
	}

	if (val.type != ValueType::NUMBER) {
		vm->Error("The register doesn't contain a number.");
		return false;
	}

	return val.as.number;
}

char* VMHelper::GetRegString(VM* vm, int reg)
{
	Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return false;
	}

	if (val.type != ValueType::STRING) {
		vm->Error("The register doesn't contain a string.");
		return false;
	}

	return val.as.string;
}

std::shared_ptr<std::vector<Value>> VMHelper::GetRegArray(VM* vm, int reg)
{
	Value val;
	if (!vm->GetRegister(reg, val)) {
		vm->Error("Error reg.");
		return nullptr;
	}

	if (val.type != ValueType::ARRAY) {
		vm->Error("The register doesn't contain a array.");
		return nullptr;
	}

	return static_cast<evm::Handle<std::vector<Value>>*>(val.as.handle)->obj;
}

}