#include "Common.h"

void new_order()
{
    //Issue the command cls as C++ offers no way to handle console clearance. This is the windows "solution".
    system("cls");

    printf("New Order\n");
    printf("Dine-in or take-away? ");
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

    printf("Enter order items below.\n");
    
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
      
    printf_s("Total take-away orders: %i\n", sales_info::takeaway_ordered);
    printf_s("Total dine-in   orders: %i\n",   sales_info::dinein_ordered);
    printf_s("Total orders: %i\n", sales_info::total_orders());
    printf_s("Total cups ordered: %i\n", sales_info::total_cups());


    getchar();

}

int main()
{
    
    std::string operation{ " " };
        
    while (operation != "")
    {
        system("cls");

        printf("New Order(1) or Daily Summary(2): ");
        operation = util::get_lower_input();
                
        if (operation == "1")
            new_order();
        else if (operation == "2")
            daily_summary();
        else
        {
            printf_s("Invalid operation: %s\n", operation.c_str());
            getchar();
        }

    }

}