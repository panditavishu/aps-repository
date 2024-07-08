#include <iostream>
#include <unordered_map>
#include <map>
#include <string>

using namespace std;

// Define a product structure
struct Product {
    string name;
    double price;
    int stock;

    Product(string n, double p, int s) : name(n), price(p), stock(s) {}
};

class InventoryManager {
private:
    unordered_map<int, Product> inventory;  // Hash map for quick lookups by ID
    map<double, int> price_index;           // Red-Black Tree for price indexing

public:
    // Add a product to the inventory
    void addProduct(int id, string name, double price, int stock) {
        Product new_product(name, price, stock);
        inventory[id] = new_product;
        price_index[price] = id;
    }

    // Update stock of a product
    void updateStock(int id, int new_stock) {
        if (inventory.find(id) != inventory.end()) {
            inventory[id].stock = new_stock;
        } else {
            cout << "Product with ID " << id << " not found." << endl;
        }
    }

    // Get product details by ID
    void getProductDetails(int id) {
        if (inventory.find(id) != inventory.end()) {
            Product& prod = inventory[id];
            cout << "Product ID: " << id << ", Name: " << prod.name << ", Price: $" << prod.price << ", Stock: " << prod.stock << endl;
        } else {
            cout << "Product with ID " << id << " not found." << endl;
        }
    }

    // Get products in a price range
    void getProductsInPriceRange(double min_price, double max_price) {
        auto start = price_index.lower_bound(min_price);
        auto end = price_index.upper_bound(max_price);

        cout << "Products in price range $" << min_price << " to $" << max_price << ":" << endl;
        for (auto it = start; it != end; ++it) {
            int id = it->second;
            Product& prod = inventory[id];
            cout << "Product ID: " << id << ", Name: " << prod.name << ", Price: $" << prod.price << ", Stock: " << prod.stock << endl;
        }
    }
};

int main() {
    InventoryManager manager;

    // Adding products to inventory
    manager.addProduct(1, "Apple", 1.5, 100);
    manager.addProduct(2, "Banana", 0.75, 150);
    manager.addProduct(3, "Orange", 1.0, 120);
    manager.addProduct(4, "Grapes", 2.0, 80);
    manager.addProduct(5, "Pineapple", 3.5, 60);

    // Update stock of a product
    manager.updateStock(3, 110);

    // Get details of a product
    manager.getProductDetails(2);

    // Get products in a price range
    manager.getProductsInPriceRange(1.0, 2.0);

    return 0;
}
