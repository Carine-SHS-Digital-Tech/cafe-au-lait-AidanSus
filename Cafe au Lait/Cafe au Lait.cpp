#include "Common.h"

void new_order()
{
    //Issue the command cls as C++ offers no way to handle console clearance. This is the windows "solution".
    system("cls");

    printf("New Order\n");
    printf("\tDine-in or take-away? ");
    std::string order_type_str = util::get_lower_input();
    order_type order_type = dine_in;


    if (order_type_str == "dine in")
        order_type = dine_in;
    else if (order_type_str == "take away")
        order_type = take_away;
    else 
    {
        printf("Invalid order type\n");
        return;
    };

    printf("\tEnter order items below.\n");
    
    std::vector<std::string> order_items;

    while (true)
    {

        const std::string& current_item = util::get_lower_input();
        
        if (current_item == "")
            break;

        if (!menu::is_valid(current_item))
        {
            printf("Invalid item: %s\n", current_item.c_str());
            continue;
        }

        order_items.push_back(current_item);

    }

    sales_info::send_order(order_items, order_type);

}

void daily_summary()
{
      
    menu::iterate_items([&](menu_item* item) {

        printf("%s ordered %i times.", item->name(), item->order_count());

    });

    getchar();

}

int main()
{
    
    std::string operation{ " " };

    while (operation != "")
    {
        system("cls");

        printf("New Order or Daily Summary: ");
        operation = util::get_lower_input();
                
        if (operation == "new order")
            new_order();
        else if (operation == "daily summary")
            daily_summary();
        else break;
    }

}