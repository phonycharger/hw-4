#include <exception>
#include <iomanip>                                                        // setprecision()
#include <iostream>                                                       // cout, fixed(), showpoint()

#include "GroceryStore.hpp"






int main()
{
  try
  {
    std::cout << std::fixed << std::setprecision( 2 ) << std::showpoint;


    ///////////////////////// TO-DO (1) //////////////////////////////
GroceryStore store;
    /////////////////////// END-TO-DO (1) ////////////////////////////


    ///////////////////////// TO-DO (2) //////////////////////////////
auto carts = store.makeShoppingCarts();
    /////////////////////// END-TO-DO (2) ////////////////////////////


    ///////////////////////// TO-DO (3) //////////////////////////////
auto soldToday = store.ringUpCustomers(carts);
    /////////////////////// END-TO-DO (3) ////////////////////////////


    ///////////////////////// TO-DO (4) //////////////////////////////
auto & inventory = store.inventory();
inventory.erase("00041331092609");
    /////////////////////// END-TO-DO (4) ////////////////////////////


    ///////////////////////// TO-DO (5) //////////////////////////////
store.reorderItems(soldToday);
    /////////////////////// END-TO-DO (5) ////////////////////////////
  }

  catch( const std::exception & ex )
  {
    std::cout << "FAILED - Unhandled exception:\n"  << ex.what() << '\n';
  }

  return 0;
}
