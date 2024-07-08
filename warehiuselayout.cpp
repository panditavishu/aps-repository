#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Structure to represent a node in the K-D Tree
struct Node {
    vector<double> point;
    int id;
    Node* left;
    Node* right;

    // Constructor
    Node(vector<double>& p, int i) : point(p), id(i), left(nullptr), right(nullptr) {}
};

// Comparator function for sorting points along a specific dimension
struct Comparator {
    int dimension;

    Comparator(int dim) : dimension(dim) {}

    bool operator()(const vector<double>& a, const vector<double>& b) const {
        return a[dimension] < b[dimension];
    }
};

// K-D Tree class
class KDTree {
private:
    Node* root;
    int dimensions;

    // Function to recursively build a K-D Tree
    Node* buildTree(vector<vector<double>>& points, int depth, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int dim = depth % dimensions;
        int mid = start + (end - start) / 2;

        nth_element(points.begin() + start, points.begin() + mid, points.begin() + end + 1, Comparator(dim));

        Node* node = new Node(points[mid], mid);
        node->left = buildTree(points, depth + 1, start, mid - 1);
        node->right = buildTree(points, depth + 1, mid + 1, end);

        return node;
    }

    // Function to calculate squared distance between two points
    double squaredDistance(const vector<double>& a, const vector<double>& b) {
        double dist = 0;
        for (int i = 0; i < dimensions; ++i) {
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return dist;
    }

    // Function to perform nearest neighbor search recursively
    void nearestNeighborSearch(Node* node, const vector<double>& target, Node*& best, double& bestDist, int depth) {
        if (node == nullptr) {
            return;
        }

        double dist = squaredDistance(node->point, target);

        if (dist < bestDist) {
            best = node;
            bestDist = dist;
        }

        int dim = depth % dimensions;
        double planeDist = target[dim] - node->point[dim];

        Node* first = planeDist < 0 ? node->left : node->right;
        Node* second = planeDist < 0 ? node->right : node->left;

        nearestNeighborSearch(first, target, best, bestDist, depth + 1);

        if (planeDist * planeDist < bestDist) {
            nearestNeighborSearch(second, target, best, bestDist, depth + 1);
        }
    }

public:
    KDTree(vector<vector<double>>& points) {
        if (points.empty()) {
            root = nullptr;
            dimensions = 0;
        } else {
            dimensions = points[0].size();
            root = buildTree(points, 0, 0, points.size() - 1);
        }
    }

    // Function to find nearest neighbor to a target point
    Node* findNearestNeighbor(const vector<double>& target) {
        if (root == nullptr) {
            return nullptr;
        }

        Node* best = root;
        double bestDist = squaredDistance(root->point, target);
        nearestNeighborSearch(root, target, best, bestDist, 0);

        return best;
    }
};

// Main function to test the K-D Tree implementation
int main() {
    // Example points (warehouse coordinates)
    vector<vector<double>> points = {
        {2.0, 3.5},
        {5.4, 1.2},
        {7.8, 4.3},
        {1.0, 6.5},
        {6.3, 2.1}
    };

    // Construct the K-D Tree
    KDTree kdTree(points);

    // Example query point (target for nearest neighbor search)
    vector<double> queryPoint = {3.0, 4.0};

    // Perform nearest neighbor search
    Node* nearestNeighbor = kdTree.findNearestNeighbor(queryPoint);

    // Display the nearest neighbor result
    if (nearestNeighbor != nullptr) {
        cout << "Nearest Neighbor ID: " << nearestNeighbor->id << ", Point: (";
        for (int i = 0; i < nearestNeighbor->point.size(); ++i) {
            cout << nearestNeighbor->point[i];
            if (i < nearestNeighbor->point.size() - 1) {
                cout << ", ";
            }
        }
        cout << ")" << endl;
    } else {
        cout << "No nearest neighbor found." << endl;
    }

    return 0;
}
