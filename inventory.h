/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef inventory_h
#define inventory_h

#include <cstdint>
#include <vector>

class Inventory{
public:

    enum class Item_Type : std::uint8_t{
        WHEAT,
        TREE
    };

private:

    std::uint32_t& get_item_reference(Item_Type type);

public:

    std::uint32_t wheat;
    std::uint32_t tree;

    Inventory();

    static std::vector<Item_Type> get_item_types();

    std::uint32_t get_item_count() const;
    std::uint32_t get_item_count(Item_Type type) const;

    //Returns the amount of the passed item type that COULD NOT be added
    std::uint32_t add_item(Item_Type type,std::uint32_t amount);

    void remove_item(Item_Type type,std::uint32_t amount);
};

#endif
