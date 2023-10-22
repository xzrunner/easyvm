#pragma once

#include <memory>
#include <functional>

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

class Value
{
public:
	Value();
	~Value();
	Value(const Value& v);
	Value& operator = (const Value& v);

	static void SetFreeCb(std::function<void(Value& val)> cb) { free_cb = cb; }

public:
	int type = ValueType::V_NIL;
	union
	{
		bool   boolean;
		double number;
		char*  string;
		void*  handle;
	} as;

private:
	void Reset();
	void Free();

private:
	static std::function<void(Value& val)> free_cb;

};

}