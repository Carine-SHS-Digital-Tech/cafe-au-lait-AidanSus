#pragma once

struct menu_item
{

private:
	std::string item_name;
	float item_price{ 0 };
	unsigned int current_order_total{ 0 };
	unsigned int total_orders{ 0 };
public:
	//Basic constructor
	menu_item(const char* name, float _price)
		: item_name(name), item_price(_price)
	{
	}

	//Inline the getters as they are small functions
	//Inline keyword just copies the compiled asm to where the function is called
	inline const std::string& name()
	{
		return item_name;
	}
		
	//Get a reference to the total
	inline unsigned int& order_total()
	{
		return current_order_total;
	}
	
	inline unsigned int& total()
	{
		return total_orders;
	}

	inline float price()
	{
		return item_price;
	}

	inline float gst()
	{
		return item_price * 0.10;
	}

	inline float price_gst()
	{
		return item_price * 1.10;
	}

};

namespace menu
{

	std::list<menu_item> menu_items = {
		{ "cappuccino", 3.00 },
		{ "espresso", 2.25 },
		{ "latte", 2.50 },
		{ "iced coffee", 2.50 },
	};

}
