#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <climits>

using namespace std;

// Struct to represent each edge in the graph
struct Edge {
    int destination;
    int weight;
    
    Edge(int dest, int w) : destination(dest), weight(w) {}
};

// Graph class with adjacency list representation
class Graph {
private:
    unordered_map<char, vector<Edge>> edges;
    
public:
    void add_edge(char node1, char node2, int weight) {
        edges[node1].push_back(Edge(node2, weight));
        edges[node2].push_back(Edge(node1, weight));
    }
    
    // A* (A-Star) algorithm implementation
    vector<char> a_star(char start, char goal, unordered_map<char, int>& heuristic) {
        priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq; // Min-heap
        unordered_map<char, char> came_from;
        unordered_map<char, int> g_score;
        unordered_map<char, int> f_score;
        
        for (auto& pair : edges) {
            g_score[pair.first] = INT_MAX;
            f_score[pair.first] = INT_MAX;
        }
        
        g_score[start] = 0;
        f_score[start] = heuristic[start];
        pq.push({f_score[start], start});
        
        while (!pq.empty()) {
            char current = pq.top().second;
            pq.pop();
            
            if (current == goal) {
                vector<char> path;
                while (current != start) {
                    path.push_back(current);
                    current = came_from[current];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                return path;
            }
            
            for (Edge& edge : edges[current]) {
                int tentative_g_score = g_score[current] + edge.weight;
                
                if (tentative_g_score < g_score[edge.destination]) {
                    came_from[edge.destination] = current;
                    g_score[edge.destination] = tentative_g_score;
                    f_score[edge.destination] = tentative_g_score + heuristic[edge.destination];
                    pq.push({f_score[edge.destination], edge.destination});
                }
            }
        }
        
        return {}; // No path found
    }
};

int main() {
    Graph city_map;
    city_map.add_edge('A', 'B', 5);  // Example edges (nodes, weight)
    city_map.add_edge('B', 'C', 7);
    city_map.add_edge('A', 'D', 3);
    city_map.add_edge('D', 'F', 10);
    city_map.add_edge('B', 'E', 4);
    city_map.add_edge('E', 'F', 2);
    
    char start_node = 'A';
    char goal_node = 'F';
    
    // Heuristic function (Manhattan distance for simplicity)
    unordered_map<char, int> heuristic = {
        {'A', 10}, {'B', 7}, {'C', 8}, {'D', 6}, {'E', 5}, {'F', 0}
    };
    
    vector<char> path = city_map.a_star(start_node, goal_node, heuristic);
    if (!path.empty()) {
        cout << "Optimal path from " << start_node << " to " << goal_node << ": ";
        for (char node : path) {
            cout << node << " ";
        }
        cout << endl;
    } else {
        cout << "No path found from " << start_node << " to " << goal_node << endl;
    }
    
    return 0;
}
