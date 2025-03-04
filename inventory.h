/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef inventory_h
#define inventory_h

#include <cstdint>
#include <vector>
#include <string>

class Inventory {
    public:
        enum class Item_Type : std::uint8_t {
            WHEAT,
            TREE
        };

    private:
        std::uint32_t& get_item_reference(Item_Type type);

    public:
        std::uint32_t wheat;
        std::uint32_t tree;

        Inventory ();

        static std::vector<Item_Type> get_item_types();

        static std::string get_item_type_string(Item_Type type);

        std::uint32_t get_item_count() const;
        std::uint32_t get_item_count(Item_Type type) const;

        // Returns the amount of the passed item type that COULD NOT be added
        std::uint32_t add_item(Item_Type type, std::uint32_t amount);

        void remove_item(Item_Type type, std::uint32_t amount);
};

#endif
