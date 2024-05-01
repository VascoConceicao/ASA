#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <chrono>
using namespace std;

int v, e;
vector<vector<int>> g;
vector<vector<int>> gT;
vector<int> order;
int res;

const int WHITE = 0;
const int GREY = 1;
const int BLACK = 2;

void parseDimensions() {
    scanf("%d %d", &v, &e);
    g = vector<vector<int>>(v + 1);
    gT = vector<vector<int>>(v + 1);
}

void parseEdges() {
    for (int n = 1; n <= e; n++) {
        int i, j;
        scanf("%d %d", &i, &j);
        g[i].push_back(j);
        gT[j].push_back(i);
    }
}

int DFSVisit(vector<vector<int>> &g, vector<int> &colors, vector<int> &jumps, int i) {  
    int max = 0;
    stack<int> s;
    s.push(i);
    while (!s.empty()) {
        int j = s.top();
        s.pop();
        if (colors[j] == WHITE) {
            colors[j] = GREY;
            s.push(j);
            int size = g[j].size();
            for (int k = 0; k < size; k++) {
                if (colors[g[j][k]] == WHITE) {
                    s.push(g[j][k]);
                } else if (colors[g[j][k]] == BLACK && max < jumps[g[j][k]] + 1) {
                    max = jumps[g[j][k]] + 1;
                }
            }
        } else if (colors[j] == GREY) {
            if (jumps[j] > max)
                max = jumps[j];
            jumps[j] = max;
            order.push_back(j);
            colors[j] = BLACK;
        }
    }
    return max;
}

void firstDFS() {
    order = vector<int>();
    vector<int> jumps = vector<int>(v + 1, 0);
    vector<int> colors = vector<int>(v + 1, 0);
    for (int i = 1; i <= v; i++)
        DFSVisit(g, colors, jumps, i);
}

void secondDFS() {
    vector<int> jumps = vector<int>(v + 1, 0);
    vector<int> colors = vector<int>(v + 1, 0);
    while (order.size() != 0) {
        res = max(res, DFSVisit(gT, colors, jumps, order.back()));
        order.pop_back();
    }
}

int main() {

    auto start_time = std::chrono::high_resolution_clock::now();

    parseDimensions();
    parseEdges();

    firstDFS();
    secondDFS();
    printf("%d\n", res);


    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Execution Time: " << duration.count() << " microseconds" << std::endl;


}