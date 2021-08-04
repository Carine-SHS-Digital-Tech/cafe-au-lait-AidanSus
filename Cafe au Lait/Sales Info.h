#pragma once

enum order_type : uint8_t
{
	take_away,
	dine_in
};

namespace sales_info
{

	uint16_t takeaway_ordered{ 0 }, dinein_ordered{ 0 };

	void send_order(std::vector<std::string> order_items, order_type order_type)
	{

		//Set up a pointer to an empty class
		menu_item *current_item = new menu_item();

		float total_order{ 0 };
		float total_order_gst{ 0 };

		int cap_orders{ 0 };
		int latte_orders{ 0 };
		int icedcof_orders{ 0 };
		int espress_orders{ 0 };

		for(std::string order_item: order_items)
		{ 

			//See if the current item name is the same as the orders name
			//This is here to cut down on get_item calls
			if (current_item->name() != order_item)
				current_item = menu::get_item(order_item);
		
			total_order += current_item->price();
			total_order_gst += current_item->gst();			
			current_item->order();

		}

		printf_s("================================\n");

		menu::iterate_items([&](menu_item* item) {

			if (item->current_orders())
			{

				char buffer[64];
				sprintf_s(buffer, "| %ix %s                        ", item->current_orders(), item->name());
				buffer[31] = '|';
				buffer[32] = '\n';
				buffer[33] = '\0';
				
				printf_s(buffer);
				
				item->order_reset();
			}

		});

		printf_s("================================\n");

		printf("Press enter to continue.\n");
		getchar();

	}

}