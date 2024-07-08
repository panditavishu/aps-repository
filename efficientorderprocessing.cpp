#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Define a structure for Order
struct Order {
    string orderId;
    int priority; // Higher priority means more urgent

    // Constructor
    Order(string id, int p) : orderId(id), priority(p) {}

    // Operator overload for comparison
    bool operator<(const Order& other) const {
        // Priority queue is a max-heap by default, so higher priority should have lesser value
        return priority > other.priority;
    }
};

class OrderProcessor {
private:
    priority_queue<Order> pq; // Priority queue to manage orders

public:
    // Add an order to the priority queue
    void addOrder(string orderId, int priority) {
        Order newOrder(orderId, priority);
        pq.push(newOrder);
        cout << "Order " << orderId << " added with priority " << priority << endl;
    }

    // Process the highest priority order
    void processNextOrder() {
        if (!pq.empty()) {
            Order nextOrder = pq.top();
            pq.pop();
            cout << "Processing order " << nextOrder.orderId << " with priority " << nextOrder.priority << endl;
        } else {
            cout << "No orders to process." << endl;
        }
    }

    // Get the highest priority order without removing it
    void peekNextOrder() {
        if (!pq.empty()) {
            Order nextOrder = pq.top();
            cout << "Next order to process: " << nextOrder.orderId << " with priority " << nextOrder.priority << endl;
        } else {
            cout << "No orders in the queue." << endl;
        }
    }
};

int main() {
    OrderProcessor processor;

    // Adding orders with different priorities
    processor.addOrder("ORD001", 3);
    processor.addOrder("ORD002", 1);
    processor.addOrder("ORD003", 5);
    processor.addOrder("ORD004", 2);

    // Processing orders based on priority
    processor.processNextOrder();
    processor.processNextOrder();
    processor.processNextOrder();
    processor.processNextOrder();
    processor.processNextOrder(); // No more orders

    return 0;
}
