// gsl_sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <gsl/gsl>

using namespace gsl;

[[gsl::suppress(con.4)]]
[[gsl::suppress(r.11)]]
[[gsl::suppress(r.5)]]
void Example_owner()
{
	{	// Ok
		owner<int*> Resource = new int;
		delete Resource;
	}

	{	// Error : Do not assign the result of an allocation or a function call with 
		//		   an owner<T> return value to a raw pointer, use owner<T> instead (i.11)
		int* Resource = new int;
		delete Resource;
	}
}

void Example_not_null() noexcept
{
	{	// Ok
		int Value;
		auto PtrAccepter = [](not_null<int*> Ptr) { *Ptr = 10; };
		PtrAccepter(&Value);
	}

	{	// Ok
		int Value;
		auto PtrAccepter = [](strict_not_null<int*> Ptr) { *Ptr = 10; };
		PtrAccepter(strict_not_null{ &Value });
	}

	{	// Error : Symbol 'Ptr2' is never tested for nullness, it can be marked as not_null(f.23).
		int Value;
		auto PtrAccepter = [](int* Ptr) { *Ptr = 10; };
		PtrAccepter(&Value);
	}
}

[[gsl::suppress(r.5)]]
[[gsl::suppress(f.6)]]
void Example_span()
{
	const auto SpanAccepter = [](span<int> Values)
	{
		if (Values.empty() || Values.size() < 5)
			return;

		at(Values, 1) = 1;

		for (auto& Val : Values)
			++Val;

		const auto Fn = [](auto) noexcept {};
		std::for_each(Values.begin(), Values.end(), Fn);

		const size_t Offset = 1;
		const size_t Count = 3;
		const auto SubValues = Values.subspan(Offset, Count);
	};

	int Values[] = { 1, 2, 3, 4 };
	SpanAccepter(Values);

	const size_t Size = 10;
	auto Values2 = std::make_unique<int[]>(Size);
	SpanAccepter({Values2.get(), Size});

	std::vector<int> Values3;
	SpanAccepter(Values3);
}

[[gsl::suppress(con.4)]]
void Example_zstring()
{
	const auto AcceptNullTerminatedString = [](czstring<>, zstring<>)	{};
	AcceptNullTerminatedString("Test Message", std::make_unique<char[]>(20).get());
}

int Example_assertion(int Value)
{
	Expects(Value != 0);

	int Result = Value * 0;
	Ensures(Result == 0);

	return Result;
}

void Example_index()
{
	int RawValueArray[] = { 0, 1, 2, 3 };
	const span<int> ValueSpan = RawValueArray;

	for (index i = 0; i < ValueSpan.size(); ++i)
	{
		// do something
	}

	std::vector<int> ValueVector = { 0, 1, 2, 3 };
	
	for (index i = 0; i < ValueVector.size(); ++i)
	{
		// do something
	}
}

void Example_final_action()
{
	{
		auto LambdaWork = []() { std::cout << "[lambda] end of scope\n"; };
		final_action action(LambdaWork);
	}

	{
		std::function<void()> FunctionWork = []() { std::cout << "[function] end of scope\n"; };
		final_action action(FunctionWork);
	}

	{
		struct Functor
		{
			void operator()() { std::cout << "[functor] end of scope\n"; }
		};

		final_action action(std::move(Functor()));
	}

	{
		auto action = finally([]() { std::cout << "[lambda] finally end of scope\n"; });
	}
}

int main()
{
	Example_not_null();
	Example_final_action();
}