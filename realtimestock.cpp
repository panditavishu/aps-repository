#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class InventoryManager {
private:
    unordered_map<string, int> stock; // Hash map to store product IDs and their quantities

public:
    // Constructor to initialize inventory
    InventoryManager() {}

    // Function to add or update stock quantity for a product
    void updateStock(const string& productId, int quantity) {
        stock[productId] = quantity; // Insert or update product quantity in hash map
        cout << "Updated stock for product " << productId << " to " << quantity << endl;
    }

    // Function to get current stock quantity for a product
    int getStock(const string& productId) {
        if (stock.find(productId) != stock.end()) {
            return stock[productId];
        } else {
            return 0; // Return 0 if product not found (or handle accordingly)
        }
    }
};

int main() {
    InventoryManager manager;

    // Updating stock quantities
    manager.updateStock("P001", 100);
    manager.updateStock("P002", 50);
    manager.updateStock("P003", 200);

    // Displaying current stock
    cout << "Current stock for P001: " << manager.getStock("P001") << endl;
    cout << "Current stock for P002: " << manager.getStock("P002") << endl;
    cout << "Current stock for P003: " << manager.getStock("P003") << endl;
    cout << "Current stock for P004: " << manager.getStock("P004") << endl; // Product not added, so returns 0

    // Updating stock again
    manager.updateStock("P002", 75); // Updating existing stock

    // Displaying updated stock
    cout << "Updated stock for P002: " << manager.getStock("P002") << endl;

    return 0;
}
