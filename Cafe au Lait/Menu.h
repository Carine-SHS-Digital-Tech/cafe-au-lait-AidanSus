#pragma once

class menu_item
{

	uint16_t times_ordered{ 0 };
	const float item_price;
	const char* item_time;
public:
	//__forceinline makes the compiler put the function's asm code where it is called instead of having a function call.
	__forceinline const float price()
	{
		return 	item_price; 
	}

};