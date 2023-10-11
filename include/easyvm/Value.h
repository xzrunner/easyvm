#pragma once

#include <memory>

#ifdef _DEBUG
#include <string>
#endif // _DEBUG

namespace evm
{

enum ValueType
{
	V_NIL,

	V_BOOLEAN,
	V_NUMBER,
	V_STRING,

	V_HANDLE = 16,
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
	int type = ValueType::V_NIL;
	union
	{
		bool   boolean;
		double number;
		char*  string;
		void*  handle;
	} as;
};

}