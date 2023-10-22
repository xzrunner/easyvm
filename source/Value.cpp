#include "easyvm/Value.h"

namespace evm
{

std::function<void(Value& val)> Value::free_cb = nullptr;

Value::Value()
{
	Reset();
}

Value::~Value()
{
	Free();
}

Value::Value(const Value& v)
{
	Free();

	type = v.type;
	as = v.as;

	const_cast<Value&>(v).Reset();
}

Value& Value::operator = (const Value& v)
{
	if (this == &v) {
		return *this;
	}

	Free();

	type = v.type;
	as = v.as;

	const_cast<Value&>(v).Reset();

	return *this;
}

void Value::Reset()
{
	memset(this, 0, sizeof(Value));
	type = ValueType::V_NIL;
}

void Value::Free()
{
	if (type == ValueType::V_STRING) 
	{
		delete(as.string);
	} 
	else if (type >= evm::ValueType::V_HANDLE) 
	{
		if (free_cb) {
			free_cb(*this);
		}
		delete(as.handle);
	}

	Reset();
}

}