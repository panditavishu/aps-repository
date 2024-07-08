#include <iostream>
#include <unordered_map>
using namespace std;

// Structure for a Suffix Tree Node
struct SuffixTreeNode {
    unordered_map<char, SuffixTreeNode*> children;
    SuffixTreeNode* suffixLink;
    int start;
    int* end;
    int suffixIndex;

    SuffixTreeNode(int start, int* end) {
        this->start = start;
        this->end = end;
        suffixLink = nullptr;
        suffixIndex = -1;
    }
};

// Function to create and return a new Suffix Tree Node
SuffixTreeNode* createNewNode(int start, int* end) {
    return new SuffixTreeNode(start, end);
}

// Function to extend suffix tree with the given character
void extendSuffixTree(char text[], int& remainingSuffixCount,
                      SuffixTreeNode*& root, int currentEnd,
                      int* lastSuffixIndex, int i) {
    *lastSuffixIndex = -1;
    remainingSuffixCount++;
    SuffixTreeNode* lastNewNode = nullptr;

    while (remainingSuffixCount > 0) {
        if (currentEnd > i) {
            if (root->children[text[i + remainingSuffixCount]] == nullptr) {
                root->children[text[i + remainingSuffixCount]] = createNewNode(i + remainingSuffixCount, &currentEnd);
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = root->children[text[i + remainingSuffixCount]];
                    lastNewNode = nullptr;
                }
            }
            else {
                SuffixTreeNode* next = root->children[text[i + remainingSuffixCount]];
                if (walkDown(next, &i, currentEnd) && (next->start - i < currentEnd - next->start)) {
                    return;
                }
                currentEnd++;
                continue;
            }
            *lastSuffixIndex = i;
            remainingSuffixCount--;

            if (root->suffixLink != nullptr) {
                root = root->suffixLink;
            }
            else {
                i++;
                currentEnd--;
            }
        }
    }
}

// Function to traverse down the suffix tree
bool walkDown(SuffixTreeNode* currentNode, int* start, int end) {
    int length = currentNode->end == nullptr ? end - *start + 1 : *(currentNode->end) - currentNode->start + 1;
    if (*start + length - 1 > end) {
        return false;
    }
    *start += length;
    return true;
}

// Function to build suffix tree for the given text
SuffixTreeNode* buildSuffixTree(char text[], int n) {
    int rootEnd = -1;
    SuffixTreeNode* root = createNewNode(-1, &rootEnd);
    SuffixTreeNode* activeNode = root;
    int activeEdge = -1;
    int activeLength = 0;
    int* lastSuffixIndex = nullptr;
    int remainingSuffixCount = 0;

    for (int i = 0; i < n; i++) {
        extendSuffixTree(text, remainingSuffixCount, activeNode, i, lastSuffixIndex, i);
    }

    return root;
}

// Function to traverse and print the suffix tree
void traverseAndPrint(SuffixTreeNode* node, char* text) {
    if (node == nullptr) return;

    if (node->start != -1) {
        for (int i = node->start; i <= *(node->end); i++) {
            cout << text[i];
        }
        cout << endl;
    }

    unordered_map<char, SuffixTreeNode*>::iterator it;
    for (it = node->children.begin(); it != node->children.end(); it++) {
        traverseAndPrint(it->second, text);
    }
}

// Main function to test suffix tree construction and traversal
int main() {
    char text[] = "banana";
    int n = strlen(text);

    SuffixTreeNode* root = buildSuffixTree(text, n);
    cout << "Suffix Tree built successfully for text: " << text << endl;
    cout << "Suffixes of the given text are:" << endl;
    traverseAndPrint(root, text);

    return 0;
}
