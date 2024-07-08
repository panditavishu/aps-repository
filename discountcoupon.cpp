#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
using namespace std;

// Rule-Based Engine to define discount rules
class RuleBasedEngine {
public:
    // Function to evaluate if a discount applies based on rules
    bool evaluateDiscount(int orderId, double orderAmount, string customerType) {
        // Example rule: Apply 10% discount if order amount exceeds $100
        if (orderAmount > 100.0) {
            return true;
        }
        return false;
    }
};

// Trie (Prefix Tree) for efficient coupon code management
class Trie {
private:
    struct TrieNode {
        unordered_map<char, TrieNode*> children;
        bool isEndOfWord;

        TrieNode() : isEndOfWord(false) {}
    };

    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a coupon code into the Trie
    void insert(string couponCode) {
        TrieNode* current = root;
        for (char c : couponCode) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    // Check if a coupon code exists in the Trie
    bool search(string couponCode) {
        TrieNode* current = root;
        for (char c : couponCode) {
            if (current->children.find(c) == current->children.end()) {
                return false; // Coupon code not found
            }
            current = current->children[c];
        }
        return current != nullptr && current->isEndOfWord;
    }
};

// Priority Queue for managing active discounts
class DiscountPriorityQueue {
private:
    struct Discount {
        string couponCode;
        double discountAmount;
        int priority;

        Discount(string code, double amount, int prio) : couponCode(code), discountAmount(amount), priority(prio) {}

        // Operator overloading for priority queue comparison
        bool operator<(const Discount& other) const {
            return priority < other.priority; // Lower priority value means higher priority
        }
    };

    priority_queue<Discount> pq;

public:
    // Add a discount to the priority queue
    void addDiscount(string couponCode, double discountAmount, int priority) {
        pq.emplace(couponCode, discountAmount, priority);
    }

    // Apply the highest priority discount
    double applyHighestPriorityDiscount() {
        if (pq.empty()) return 0.0;

        Discount topDiscount = pq.top();
        pq.pop();
        return topDiscount.discountAmount;
    }

    // Check if the priority queue is empty
    bool isEmpty() {
        return pq.empty();
    }
};

// Example usage of Discount and Coupon Management system
int main() {
    // Initialize components
    RuleBasedEngine ruleEngine;
    Trie couponTrie;
    DiscountPriorityQueue discountQueue;

    // Insert some example coupon codes into the Trie
    couponTrie.insert("SUMMER2024");
    couponTrie.insert("SAVE10");
    couponTrie.insert("FREESHIP");

    // Add discounts to the priority queue
    discountQueue.addDiscount("SUMMER2024", 10.0, 2); // Priority 2
    discountQueue.addDiscount("SAVE10", 5.0, 1);     // Priority 1

    // Simulate order details
    int orderId = 1234;
    double orderAmount = 120.0;
    string customerType = "Regular";

    // Evaluate discount eligibility
    bool applyDiscount = ruleEngine.evaluateDiscount(orderId, orderAmount, customerType);

    // Apply discounts based on priority
    if (applyDiscount) {
        double appliedDiscount = discountQueue.applyHighestPriorityDiscount();
        cout << "Applied discount amount: $" << appliedDiscount << endl;
    } else {
        cout << "No discounts applied." << endl;
    }

    // Check if a coupon code is valid
    string couponCode = "SUMMER2024";
    bool validCoupon = couponTrie.search(couponCode);
    if (validCoupon) {
        cout << "Coupon code " << couponCode << " is valid." << endl;
    } else {
        cout << "Coupon code " << couponCode << " is invalid." << endl;
    }

    return 0;
}
