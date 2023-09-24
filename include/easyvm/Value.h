#pragma once

#include <memory>

namespace evm
{

enum class ValueType
{
	BOOLEAN,
	NUMBER,
	STRING,

	HANDLE,
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
	ValueType type;
	union
	{
		bool   boolean;
		double number;
		char*  string;
		void*  handle;
	} as;
};

}