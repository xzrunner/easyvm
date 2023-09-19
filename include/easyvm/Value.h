#pragma once

namespace evm
{

enum class ValueType
{
	BOOLEAN,
	NUMBER,
	STRING,

	USER_DATA,

	// RAW_PTR
	// SMART_PTR
};

struct Value
{
	ValueType type;
	union
	{
		bool   boolean;
		double number;
		char*  string;

		void*  user_data;
	} as;
};

}