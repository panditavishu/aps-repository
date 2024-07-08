#include <iostream>
using namespace std;

enum Color { RED, BLACK };

// Node structure for Red-Black Tree
struct Node {
    int transactionID;
    double amount;
    string timestamp;
    int userID;
    Color color;
    Node *left, *right, *parent;

    // Constructor
    Node(int id, double amt, string ts, int uid) : transactionID(id), amount(amt), timestamp(ts), userID(uid) {
        parent = left = right = nullptr;
        color = RED;
    }
};

// Red-Black Tree class
class RedBlackTree {
private:
    Node* root;

    // Function to perform left rotation
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Function to perform right rotation
    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    // Function to fix violations after insertion
    void insertFixup(Node* z) {
        while (z != root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Function to recursively insert a node
    Node* insertRecursive(Node* root, Node* z) {
        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            y = x;
            if (z->transactionID < x->transactionID) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        z->parent = y;
        if (y == nullptr) {
            root = z;
        } else if (z->transactionID < y->transactionID) {
            y->left = z;
        } else {
            y->right = z;
        }
        z->left = z->right = nullptr;
        z->color = RED;
        insertFixup(z);
        return root;
    }

    // Function to find the node with minimum transaction ID
    Node* findMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Function to fix violations after deletion
    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Function to recursively delete a node
    Node* deleteRecursive(Node* root, int key) {
        if (root == nullptr) return root;

        if (key < root->transactionID) {
            root->left = deleteRecursive(root->left, key);
        } else if (key > root->transactionID) {
            root->right = deleteRecursive(root->right, key);
        } else {
            if (root->left == nullptr || root->right == nullptr) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                Node* temp = findMin(root->right);
                root->transactionID = temp->transactionID;
                root->amount = temp->amount;
                root->timestamp = temp->timestamp;
                root->userID = temp->userID;
                root->right = deleteRecursive(root->right, temp->transactionID);
            }
        }

        if (root == nullptr) return root;

        if (root->left == nullptr || root->right == nullptr) {
            return root;
        }

        Node* x = root->right;
        while (x->left != nullptr) {
            x = x->left;
        }

        Node* w = x;
        deleteFixup(w);

        return root;
    }

    // Function to recursively search for a node by transaction ID
    Node* searchRecursive(Node* root, int key) {
        if (root == nullptr || root->transactionID == key) {
            return root;
        }

        if (key < root->transactionID) {
            return searchRecursive(root->left, key);
        }

        return searchRecursive(root->right, key);
    }

public:
    RedBlackTree() {
        root = nullptr;
    }

    // Function to insert a new transaction into the Red-Black Tree
    void insertTransaction(int transactionID, double amount, string timestamp, int userID) {
        Node* z = new Node(transactionID, amount, timestamp, userID);
        root = insertRecursive(root, z);
    }

    // Function to delete a transaction from the Red-Black Tree by transaction ID
    void deleteTransaction(int transactionID) {
        root = deleteRecursive(root, transactionID);
    }

    // Function to search for a transaction by transaction ID
    Node* searchTransaction(int transactionID) {
        return searchRecursive(root, transactionID);
    }
};

// Main function to test Red-Black Tree implementation
int main() {
    RedBlackTree rbTree;

    // Insert transactions
    rbTree.insertTransaction(1001, 45.67, "2024-07-09 10:15:00", 101);
    rbTree.insertTransaction(1002, 56.78, "2024-07-09 11:30:00", 102);
    rbTree.insertTransaction(1003, 32.45, "2024-07-09 14:45:00", 103);
    rbTree.insertTransaction(1004, 78.90, "2024-07-09 16:00:00", 104);

    // Search for a transaction
    int searchID = 1003;
    Node* result = rbTree.searchTransaction(searchID);
    if (result != nullptr) {
        cout << "Transaction found - ID: " << result->transactionID << ", Amount: $" << result->amount
             << ", Timestamp: " << result->timestamp << ", User ID: " << result->userID << endl;
    } else {
        cout << "Transaction with ID " << searchID << " not found." << endl;
    }

    // Delete a transaction
    int deleteID = 1002;
    rbTree.deleteTransaction(deleteID);
    cout << "Transaction with ID " << deleteID << " deleted." << endl;

    return 0;
}
