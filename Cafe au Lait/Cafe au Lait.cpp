#include "Common.h"

void new_order()
{

    //Call the iterator with our lambda
    menu::iterate_items([&](menu_item& item) {

        item.order();
        printf("%s ordered %i times.\n", item.name(), item.order_count());

    });

}

void daily_summary()
{

    

}

int main()
{
    
    daily_summary();
    daily_summary();

}