#include <iostream>

#include <string>
#include <vector>
#include <list>

#include <chrono>
#include <random>

int cinInt() {
    std::string lineIn;
    std::cin >> lineIn;
    return std::stoi(lineIn);
}

typedef struct {
    int src;
    int dest;
} Edge;

class UnionFind {
 private:
    std::vector<int> parent;
    std::vector<int> ranks;
    std::list<Edge> edges;
    int nodeCount;
    int edgeCount;

    std::mt19937 generator;

 public:
    UnionFind(int nodeCt, int edgeCt) {
        this->nodeCount = nodeCt;
        this->edgeCount = edgeCt;
        for (int i = 0; i < edgeCount; i++) {
            this->edges.push_back(Edge{cinInt(), cinInt()});
        }
        for (int i = 0; i < nodeCount; i++) {
            this->parent.push_back(i);
            this->ranks.push_back(1);
        }
        
        this->generator.seed((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    }

    int find(int i) {
        while (this->parent[i] != i) {
            this->parent[i] = this->parent[this->parent[i]];
            i = this->parent[i];
        }
        return i;
    }

    // inline bool find(int a, int b) { return find(a) == find(b); }

    void unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        // a tree smaller than b tree
        // make a tree child of b tree
        if (this->ranks[rootA] <= this->ranks[rootB]) {
            this->parent[rootA] = rootB;
            this->ranks[rootB] += this->ranks[rootA];
        } else {  // vice versa
            this->parent[rootB] = rootA;
            this->ranks[rootA] += this->ranks[rootB];
        }
    }

    Edge getEdge(int n) {
        std::list<Edge>::iterator it = this->edges.begin();
        for (int i = 0; i < n; i++) {
            ++it;
        }
        return *it;
    }

    void collapse() {
        Edge edge = this->getEdge(this->generator() % (this->edgeCount - 1));
        // collapse edge
        unite(edge.src, edge.dest);
        // std::cout << "\n" << edge.src << " " << edge.dest << "\n";
        // set everything to its parents
        // delete every edge that maps to itself
        for (auto it = edges.begin(); it != edges.end();) {
            it->src = find(it->src);
            it->dest = find(it->dest);
            if (it->src == it->dest) {
                it = this->edges.erase(it);
                this->edgeCount--;
            } else {
                it++;
            }
        }
        this->nodeCount--;
    }

    int karger() {
        // save original state
        std::vector<int> parentTemp = this->parent;
        std::vector<int> ranksTemp = this->ranks;
        std::list<Edge> edgesTemp = this->edges;
        int nodeCountTemp = this->nodeCount;
        int edgeCountTemp = this->edgeCount;

        while (nodeCount > 2) {
            collapse();
        }
        for (int i = 0; i < (int)this->parent.size() - 1; i++) {
            find(i);
        }
        int cut = edges.size();
        // reset state
        this->parent = parentTemp;
        this->ranks = ranksTemp;
        this->edges = edgesTemp;
        this->nodeCount = nodeCountTemp;
        this->edgeCount = edgeCountTemp;
        return cut;
    }

    int minCut() {
        int n = this->nodeCount;
        int k = .5 * n * (n - 1) * log(n) / log(2);
        int min = this->karger();
        int cut;
        for (int i = 1; i < k; i++) {
            cut = this->karger();
            min = (cut < min) ? cut : min;
        }
        return min;
    }

    void printUF() {
        std::cout << "Par:\t";
        for (auto p : this->parent) {
            std::cout << p << " ";
        }
        std::cout << "\nRanks:\t";
        for (auto r : this->ranks) {
            std::cout << r << " ";
        }
        std::cout << "\n";
    }

    void printEdges() {
        std::cout << "Src:\t";
        for (auto edge : this->edges) {
            std::cout << edge.src << " ";
        }
        std::cout << "\nDest:\t";
        for (auto edge : this->edges) {
            std::cout << edge.dest << " ";
        }
        std::cout << "\n";
    }
};

// main program
int main() {
    int nodeCount = cinInt();
    int edgeCount = cinInt();
    UnionFind uf(nodeCount, edgeCount);
    // uf.print();
    // uf.printEdges();
    std::cout << uf.minCut() << "\n";
}