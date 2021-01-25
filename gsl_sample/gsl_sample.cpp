// gsl_sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

#include <gsl/gsl>

using namespace gsl;

[[gsl::suppress(con.4)]]
[[gsl::suppress(r.11)]]
[[gsl::suppress(r.5)]]
void Example_26400()
{
	{	// Ok
		owner<int*> Resource = new int;
		delete Resource;
	}

	{	// Error : Do not assign the result of an allocation or a function call with 
		//		   an owner<T> return value to a raw pointer, use owner<T> instead (i.11)
		int* Resource = new int;
	}
}


[[gsl::suppress(r.11)]]
void Example_C26401(owner<int*> Ptr1, not_null<int*> Ptr2) noexcept
{
	{	// Ok
		delete Ptr1;
	}

	{	// Error : Do not delete a raw pointer that is not an owner<T> (i.11).
		delete Ptr2;
	}
}

[[gsl::suppress(i.11)]]
[[gsl::suppress(r.5)]]
void Example_C26409()
{
	{	// Ok
		auto Resource = std::make_unique<int>();
		auto SharedResource = std::make_shared<int>();
	}

	{	// Error : Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
		int* Resource = new int;
		delete Resource;
	}

	{	// Error : Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
		std::unique_ptr<int> Resource(new int);
		std::shared_ptr<int> SharedResource(new int);
	}
}

void Eaxmple_C26429(not_null<int*> Ptr1, int* Ptr2) noexcept
{
	{	// Ok
		*Ptr1 = 10;
	}

	{	// Error : Symbol 'Ptr2' is never tested for nullness, it can be marked as not_null(f.23).
		*Ptr2 = 10;
	}
}


