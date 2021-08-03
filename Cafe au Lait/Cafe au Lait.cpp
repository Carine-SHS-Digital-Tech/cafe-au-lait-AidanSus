#include "Common.h"

void new_order()
{
    //Issue the command cls as C++ offers no way to handle console clearance. This is the windows "solution".
    system("cls");

    printf("New Order\n");
    printf("\tDine-in or take-away? ");
    std::string order_type = util::get_lower_input();

    if (order_type == "dine in")
        sales_info::dinein_ordered++;
    else if (order_type == "take away")
        sales_info::takeaway_ordered++;
    else 
    {
        printf("Invalid order type\n");
        return;
    };

    printf("\tEnter order items below.\n");
    
    while (true)
    {

        std::string current_item = util::get_lower_input();

        if (!menu::is_valid(current_item))
            return;

        menu_item &ordered_item = menu::get_item(current_item);
        ordered_item.order();

    }

}

void daily_summary()
{
      
    menu::iterate_items([&](menu_item& item) {

        printf("%s ordered %i times.", item.name(), item.order_count());

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