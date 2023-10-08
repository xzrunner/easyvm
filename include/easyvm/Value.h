#pragma once

#include <memory>

#ifdef _DEBUG
#include <string>
#endif // _DEBUG

namespace evm
{

enum class ValueType
{
	NIL,

	BOOLEAN,
	NUMBER,
	STRING,

	HANDLE,

	ARRAY,
};

template<typename T>
struct Handle
{
	Handle() {}
	Handle(const std::shared_ptr<T>& o) : obj(o) {}
	~Handle() {}

	std::shared_ptr<T> obj = nullptr;
};

struct Value
{
	ValueType type = ValueType::NIL;
	union
	{
		bool   boolean;
		double number;
		char*  string;
		void*  handle;
	} as;

#ifdef _DEBUG
	std::string handle_type;
#endif // _DEBUG
};

}