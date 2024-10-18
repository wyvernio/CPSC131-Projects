#include "GroceryItem.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "Welcome to Kroger. Place grocery items into your shopping cart by entering each item's information." << std::endl
              << "   Enclose string entries in quotes, separate fields with commas." << std::endl
              << "   For example: \"00016000306707\", \"Betty Crocker\", \"Betty Crocker Double Chocolate Chunk Cookie Mix\", 17.19" << std::endl
              << "   Enter CTL-Z (Windows) or CTL-D (Linux) to quit." << std::endl << std::endl;

    std::vector<std::unique_ptr<GroceryItem>> groceryItems;


    GroceryItem item;


    while (std::cout << "Enter UPC, Product Brand, Product Name, and Price\n" && std::cin >> item >> std::ws) {
        groceryItems.push_back(std::make_unique<GroceryItem>(std::move(item)));
        std::cout << "Item added to shopping cart: " << *groceryItems.back() << "\n";
        std::cout << std::endl;
    }

    std::cout << std::endl << std:: endl << "Here is an itemized list of the items in your shopping cart (in reverse order):\n";


    std::for_each(groceryItems.rbegin(), groceryItems.rend(), [](const auto &itemPtr) {
        std::cout << *itemPtr << "\n";
    });


    groceryItems.clear(); 

    return 0;
}
