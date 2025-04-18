///////////////////////// TO-DO (1) //////////////////////////////
#include <fstream>
#include <iomanip>
#include <iterator>
#include <algorithm>

#include "GroceryStore.hpp"
#include "GroceryItemDatabase.hpp"
/////////////////////// END-TO-DO (1) ////////////////////////////






GroceryStore::GroceryStore( const std::string & persistentInventoryDB )
{
  std::ifstream fin( persistentInventoryDB );                     // Creates the stream object, and then opens the file if it can
                                                                  // The file is closed as fin goes out of scope
  if( !fin.is_open() ) std::cerr << "Warning:  Could not open persistent inventory database file \"" << persistentInventoryDB << "\".  Proceeding with empty inventory\n\n";

  // The file's inventory contents consists of a quoted UPC string followed by a quantity on hand unsigned integer separated by
  // whitespace, like this:
  //     "00044100117428"     8
  //     "00041780001566"    46
  //     "00021000043309"    35
  //     "00021000043309"     9

  ///////////////////////// TO-DO (2) //////////////////////////////
using std::quoted;
std::string upc;
unsigned int qty{};
while (fin >> std::ws >> quoted(upc) >> qty)
  _inventoryDB[upc] += qty;  
  /////////////////////// END-TO-DO (2) ////////////////////////////
}                                                                 // File is closed as fin goes out of scope (RAII)







GroceryStore::Inventory_DB & GroceryStore::inventory()
{ return _inventoryDB; }







GroceryStore::GroceryItemsSold GroceryStore::ringUpCustomer( const ShoppingCart & shoppingCart, std::ostream & receipt )
{
  auto & worldWideGroceryDatabase = GroceryItemDatabase::instance();        // Get a reference to the world wide database of all
                                                                            // groceries in the world. The database will contains a
                                                                            // full description of the item and the item's price.

  GroceryItemsSold purchasedGroceries;                                      // a collection of UPCs of the groceries purchased by this customer


  ///////////////////////// TO-DO (3) //////////////////////////////
double amountDue{0.0};
for (const auto & [upc, cartItem] : shoppingCart)
{
  auto * dbItem = worldWideGroceryDatabase.find(GroceryItemDatabase::Key{upc});
  if (!dbItem)
  {
    receipt << '\t' << std::quoted(upc) << " not found, the item is free!\n";
    continue;
  }

  receipt << '\t' << *dbItem << '\n';
  amountDue += dbItem->price();
  if (auto itr = _inventoryDB.find(upc); itr != _inventoryDB.end() && itr->second)
    --itr->second;

  purchasedGroceries.insert(upc);
}
receipt << "\t-------------------------\n\tTotal  $" << amountDue << "\n\n";
return purchasedGroceries;
  /////////////////////// END-TO-DO (3) ////////////////////////////

  return purchasedGroceries;
} // ringUpCustomer







GroceryStore::GroceryItemsSold GroceryStore::ringUpCustomers( const ShoppingCarts & shoppingCarts, std::ostream & receipt )
{
  GroceryItemsSold todaysSales;                                   // a collection of unique UPCs of grocery items sold

  ///////////////////////// TO-DO (4) //////////////////////////////
for (const auto & [customer, cart] : shoppingCarts)
{
  receipt << customer << "'s shopping cart contains:\n";
  GroceryItemsSold bought = ringUpCustomer(cart, receipt);
  todaysSales.merge(std::move(bought));
}
return todaysSales;
  /////////////////////// END-TO-DO (4) ////////////////////////////

  return todaysSales;
} // ringUpCustomers







void GroceryStore::reorderItems( GroceryItemsSold & todaysSales, std::ostream & reorderReport )
{
  auto & worldWideGroceryDatabase = GroceryItemDatabase::instance();    // Get a reference to the world wide database of all
                                                                        // groceries in the world. The database will contains a
                                                                        // full description of the item and the item's price.

  ///////////////////////// TO-DO (5) //////////////////////////////
unsigned index{1};
for (auto it = todaysSales.begin(); it != todaysSales.end(); ++it, ++index)
{
  const std::string & upc = *it;
  auto * dbItem = worldWideGroceryDatabase.find(GroceryItemDatabase::Key{upc});
  auto invIt = _inventoryDB.find(upc);

  reorderReport << ' ' << index << ":  ";
  if (dbItem) reorderReport << '{' << *dbItem << "} \n";
  else        reorderReport << '"' << upc << "\" (description unavailable)\n";

  if (invIt == _inventoryDB.end())
  {
    reorderReport << "\t*** no longer sold in this store and will not be re-ordered\n\n";
    continue;
  }

  if (invIt->second >= REORDER_THRESHOLD)
  {
    reorderReport << '\n';   // nothing to do
    continue;
  }

unsigned int below = REORDER_THRESHOLD - invIt->second;
  reorderReport << "\tonly " << invIt->second << " remain in stock which is "
                << below << " unit(s) below reorder threshold (" << REORDER_THRESHOLD
                << "), re-ordering " << LOT_COUNT << " more\n\n";
  invIt->second += LOT_COUNT;
}
todaysSales.clear();
  /////////////////////// END-TO-DO (5) ////////////////////////////
}







GroceryStore::ShoppingCarts GroceryStore::makeShoppingCarts()
  {
    // Our store has many customers, and each (identified by name) is pushing a shopping cart. Shopping carts are structured as
    // trees of trees of grocery items.
    ShoppingCarts carts =
    {
      // first shopping cart
      { "Red Baron",        { {"00075457129000", {"milk"     }}, {"00038000291210", {"rice krispies"}}, {"00025317533003", {"hotdogs"}},
                              {"09073649000493", {"apple pie"}}, {"00835841005255", {"bread"        }}, {"00688267039317", {"eggs"   }},
                              {"00037466065908", {"Truffles" }}
                            }
      },

      // second shopping cart
      { "Peppermint Patty", { {"00813608012401", {"Cheese"      }}, {"00037466065908", {"Truffles"      }}, {"00631723300704", {"Pepperoncini"}},
                              {"00051500280645", {"Pancake Mix" }}, {"00792851355155", {"Soup"          }}, {"00898425001333", {"Sandwich"    }},
                              {"00021908501871", {"Potato Spuds"}}, {"00856414001092", {"Plantain Chips"}}, {"00036192122930", {"Applesauce"  }},
                              {"00763795931415", {"Paper Plates"}}, {"00070596000647", {"Soap"          }}, {"00079400266200", {"Deodorant"   }}
                            }
      },

      // third shopping cart
      { "Woodstock",        { {"00859013004327", {"Mint Tea"         }}, {"00041331092609", {"Spanish Omelet"}}, {"00217039300005", {"Fillet Steak"   }},
                              {"00636874220338", {"Face Exfoliate"   }}, {"00688267138973", {"Green Tea"     }}, {"00033674100066", {"Forskohlii"     }},
                              {"00891475001421", {"Candy Fruit Chews"}}, {"00688267133442", {"Coffee"        }}, {"00021000778690", {"Cheesy Potatoes"}},
                              {"00071758100823", {"Pepper"           }}, {"00041520010667", {"Mouth Wash"    }}, {"00216079600007", {"Lamb Chops"     }},
                              {"00790555051526", {"Black Beans"      }}, {"00070177862640", {"Tea Sampler"   }}, {"00018000001958", {"Cinnamon Rolls" }}
                            }
      },

      // forth shopping cart
      { "Schroeder",        { {"00072591002251", {"Peanut Butter"}}, {"00219019100001", {"Ground Beef"           }}, {"00890692002020", {"Stretch Cream" }},
                              {"00637876981630", {"Mozzarella"   }}, {"00716237183507", {"Facial Towelettes"     }}, {"00886911000052", {"Pastry Popover"}},
                              {"00084783499008", {"Massage Oil"  }}, {"00016055456747", {"Children's Fruit Drink"}}, {"00021908743325", {"Granola Cereal"}},
                              {"00688267138973", {"Green Tea"    }}
                            }
      },

      // fifth shopping cart
      { "Lucy van Pelt",    { {"00070327012116", {"Pineapple"     }}, {"00073377801839", {"Sleep Mask Blue"}}, {"00030768009670", {"Calcium Softgels"}},
                              {"00852697001422", {"Baby Food"     }}, {"00079471102414", {"Manzanillo"     }}, {"00072736013753", {"Dressing"        }},
                              {"00216210900003", {"Cheddar Cheese"}}, {"00780872310185", {"Dog Food"       }}, {"00710069086036", {"Crackers"        }},
                              {"00092657016985", {"Tea",          }}, {"00086449903065", {"Deodorant"      }}, {"00885229000198", {"Drinks"          }}
                            }
      },

      // sixth shopping cart
      { "Charlie Brown",    { {"00611508524006", {"Lump Of Coal"}}
                            }
      }
    };

    return carts;
  }  // makeShoppingCarts
