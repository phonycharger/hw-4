#pragma once

#include <compare>                                                              // *_ordering
#include <cstddef>                                                              // size_t
#include <string>
#include <map>

#include "GroceryItem.hpp"



// Singleton Design Pattern
class GroceryItemDatabase
{
  public:
    // Public Types
    class Key;

    // Database Access
    static GroceryItemDatabase & instance();
                                                                                // found, nullptr otherwise
    // Queries
    GroceryItem * find( const Key & upc );                                      // Locates and returns a pointer to the item in the database if
    std::size_t   size() const;                                                 // Returns the number of items in the database

  private:
    GroceryItemDatabase( const std::string & filename );

    GroceryItemDatabase( const GroceryItemDatabase & )             = delete;    // intentionally prohibit making copies
    GroceryItemDatabase & operator=( const GroceryItemDatabase & ) = delete;    // intentionally prohibit copy assignments


    std::map<Key /*UPC*/, GroceryItem> _data;                                   // Collection of grocery items indexed by UPC
};




class GroceryItemDatabase::Key
{
  public:                                                                       // No implicit conversion from string to Key.
    explicit Key( std::string value );                                          // Causes the default constructor to not be synthesized (intentional), the other 5 are synthesized and just what we want
    std::string const & to_string() const;

    auto operator<=>( Key const & rhs ) const = default;                        // Endow objects with the ability to compare themselves
    bool operator== ( Key const & rhs ) const = default;

  private:
    std::string _upc;                                                           // The key of the Grocery Item is its UPC
};
