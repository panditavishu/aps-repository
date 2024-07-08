#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Constants for B+ Tree
const int ORDER = 4;  // Order of the B+ Tree (degree)

// Node structure for B+ Tree
struct BPlusTreeNode {
    vector<int> keys;  // Keys in the node
    vector<void*> pointers;  // Pointers to child nodes or data
    BPlusTreeNode* parent;  // Parent node
    bool isLeaf;  // Indicates if the node is a leaf
    BPlusTreeNode* next;  // Pointer to the next leaf node

    BPlusTreeNode() {
        parent = nullptr;
        isLeaf = false;
        next = nullptr;
    }
};

// B+ Tree class
class BPlusTree {
private:
    BPlusTreeNode* root;

public:
    BPlusTree() {
        root = nullptr;
    }

    // Function to insert a key-value pair into the B+ Tree
    void insert(int key, void* data) {
        if (root == nullptr) {
            root = new BPlusTreeNode();
            root->isLeaf = true;
            root->keys.push_back(key);
            root->pointers.push_back(data);
        } else {
            BPlusTreeNode* leafNode = findLeafNode(root, key);

            // Insert the key and data into the leaf node
            insertIntoLeaf(leafNode, key, data);

            // Check if the leaf node needs to be split
            if (leafNode->keys.size() >= ORDER) {
                splitLeafNode(leafNode);
            }
        }
    }

    // Function to find the leaf node where the key should be inserted
    BPlusTreeNode* findLeafNode(BPlusTreeNode* node, int key) {
        if (node->isLeaf) {
            return node;
        }

        // Find the child node where the key should go
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        return findLeafNode((BPlusTreeNode*)node->pointers[i], key);
    }

    // Function to insert key-value pair into a leaf node
    void insertIntoLeaf(BPlusTreeNode* leafNode, int key, void* data) {
        auto it = lower_bound(leafNode->keys.begin(), leafNode->keys.end(), key);
        int pos = it - leafNode->keys.begin();
        leafNode->keys.insert(it, key);
        leafNode->pointers.insert(leafNode->pointers.begin() + pos, data);
    }

    // Function to split a leaf node when it exceeds the ORDER
    void splitLeafNode(BPlusTreeNode* leafNode) {
        int splitIndex = ORDER / 2;

        BPlusTreeNode* newLeafNode = new BPlusTreeNode();
        newLeafNode->isLeaf = true;
        newLeafNode->next = leafNode->next;
        leafNode->next = newLeafNode;

        newLeafNode->keys.assign(leafNode->keys.begin() + splitIndex, leafNode->keys.end());
        newLeafNode->pointers.assign(leafNode->pointers.begin() + splitIndex, leafNode->pointers.end());
        leafNode->keys.resize(splitIndex);
        leafNode->pointers.resize(splitIndex);

        // Adjust parent pointers
        newLeafNode->parent = leafNode->parent;
        insertIntoParent(leafNode, newLeafNode->keys[0], newLeafNode);
    }

    // Function to insert a new node into the parent of two nodes
    void insertIntoParent(BPlusTreeNode* leftNode, int key, BPlusTreeNode* rightNode) {
        BPlusTreeNode* parent = leftNode->parent;

        if (parent == nullptr) {
            parent = new BPlusTreeNode();
            parent->isLeaf = false;
            root = parent;
            leftNode->parent = parent;
            rightNode->parent = parent;
        }

        auto it = lower_bound(parent->keys.begin(), parent->keys.end(), key);
        int pos = it - parent->keys.begin();

        parent->keys.insert(it, key);
        parent->pointers.insert(parent->pointers.begin() + pos + 1, rightNode);

        // Check if parent needs to split
        if (parent->keys.size() >= ORDER) {
            splitInternalNode(parent);
        }
    }

    // Function to split an internal node when it exceeds the ORDER
    void splitInternalNode(BPlusTreeNode* node) {
        int splitIndex = ORDER / 2;

        BPlusTreeNode* newInternalNode = new BPlusTreeNode();
        newInternalNode->isLeaf = false;

        newInternalNode->keys.assign(node->keys.begin() + splitIndex + 1, node->keys.end());
        newInternalNode->pointers.assign(node->pointers.begin() + splitIndex + 1, node->pointers.end());

        node->keys.resize(splitIndex);
        node->pointers.resize(splitIndex + 1);

        // Move the first key of the new internal node up to the parent node
        int key = node->keys[splitIndex];
        BPlusTreeNode* parent = node->parent;
        insertIntoParent(node, key, newInternalNode);

        // Adjust parent pointers
        newInternalNode->parent = parent;
        for (auto child : newInternalNode->pointers) {
            if (!child->isLeaf) {
                child->parent = newInternalNode;
            }
        }
    }

    // Function to search for keys within a specific range
    void searchRange(int low, int high) {
        vector<void*> results;
        searchRange(root, low, high, results);
        cout << "Products within range [" << low << ", " << high << "]: ";
        for (auto ptr : results) {
            // Assuming ptr points to product data or object
            cout << *((int*)ptr) << " ";
        }
        cout << endl;
    }

    // Helper function for range search
    void searchRange(BPlusTreeNode* node, int low, int high, vector<void*>& results) {
        if (node == nullptr) return;

        int i = 0;
        while (i < node->keys.size() && node->keys[i] < low) {
            i++;
        }

        if (node->isLeaf) {
            while (i < node->keys.size() && node->keys[i] <= high) {
                results.push_back(node->pointers[i]);
                i++;
            }
        } else {
            while (i < node->keys.size()) {
                searchRange((BPlusTreeNode*)node->pointers[i], low, high, results);
                i++;
            }
        }
    }
};

// Main function to test B+ Tree implementation
int main() {
    BPlusTree bplusTree;

    // Example of inserting products with their IDs
    bplusTree.insert(10, (void*)new int(10));
    bplusTree.insert(20, (void*)new int(20));
    bplusTree.insert(15, (void*)new int(15));
    bplusTree.insert(25, (void*)new int(25));
    bplusTree.insert(5, (void*)new int(5));

    // Example of range search
    bplusTree.searchRange(15, 25);

    return 0;
}
