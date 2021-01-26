// gsl_sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

#include <vector>
#include <algorithm>
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

void Eaxmple_not_null(not_null<int*> Ptr1, int* Ptr2) noexcept
{
	{	// Ok
		*Ptr1 = 10;
	}

	{	// Error : Symbol 'Ptr2' is never tested for nullness, it can be marked as not_null(f.23).
		*Ptr2 = 10;
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