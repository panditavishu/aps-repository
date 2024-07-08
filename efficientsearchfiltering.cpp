#include <iostream>
#include <unordered_map>
using namespace std;

// TrieNode class representing each node in the Trie
class TrieNode {
public:
    unordered_map<char, TrieNode*> children; // Map to store child nodes
    bool isEndOfWord; // Flag to indicate end of a word

    // Constructor
    TrieNode() {
        isEndOfWord = false;
    }
};

// Trie class for storing and querying strings
class Trie {
private:
    TrieNode* root; // Root node of the Trie

public:
    // Constructor
    Trie() {
        root = new TrieNode();
    }

    // Function to insert a word into the Trie
    void insert(string word) {
        TrieNode* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true; // Mark end of word
    }

    // Function to search if a word exists in the Trie
    bool search(string word) {
        TrieNode* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return false; // Character not found
            }
            current = current->children[c];
        }
        return current != nullptr && current->isEndOfWord; // Return true if end of word
    }

    // Function to perform autocomplete based on a prefix
    void autocomplete(string prefix) {
        TrieNode* current = root;
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                cout << "No words found with prefix " << prefix << endl;
                return;
            }
            current = current->children[c];
        }
        // Print all words with the given prefix
        printAllWords(current, prefix);
    }

private:
    // Recursive function to print all words starting from a given node
    void printAllWords(TrieNode* node, string prefix) {
        if (node == nullptr) return;

        if (node->isEndOfWord) {
            cout << prefix << endl;
        }

        for (auto& pair : node->children) {
            printAllWords(pair.second, prefix + pair.first);
        }
    }
};

// Example usage of the Trie class
int main() {
    Trie trie;

    // Insert some example words
    trie.insert("apple");
    trie.insert("applesauce");
    trie.insert("apricot");
    trie.insert("banana");
    trie.insert("blueberry");

    // Search for words
    cout << "Searching for 'apple': " << (trie.search("apple") ? "Found" : "Not found") << endl;
    cout << "Searching for 'grape': " << (trie.search("grape") ? "Found" : "Not found") << endl;

    // Autocomplete example
    cout << "Autocomplete suggestions for prefix 'app':" << endl;
    trie.autocomplete("app");

    return 0;
}
